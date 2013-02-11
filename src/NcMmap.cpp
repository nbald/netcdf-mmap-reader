/*
 * Copyright (c) 2013 the OpenMeteoData project
 * All rights reserved.
 *
 * Author: Nicolas BALDECK <nicolas.baldeck@openmeteodata.org>
 * 
 * This file is a part of MeteoDataServer
 * 
 * netcdf-mmap-reader library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * netcdf-mmap-reader is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <OpenMeteoData/NcMmap.h>

namespace OpenMeteoData {

  NcMmap::NcMmap(FileName const &fileName)
  {
    
    
    
    file_.name = fileName;
    
    
    file_.handle = open(file_.name.c_str(), O_RDONLY);
    if (file_.handle == -1)
    {
        throw FileException("ERROR : can't open " + file_.name);
    }
    
    #ifdef DEBUG
      std::cout << "open " << file_.name << std::endl;
    #endif
    
    struct stat statInfos;
    if (stat(file_.name.c_str(), &statInfos) == -1) {
        throw FileException("stat error");
    }
    
    file_.size = statInfos.st_size;
    
    #ifdef DEBUG
      std::cout << "filesize " << file_.size/1e6 << " Mb" << std::endl;
    #endif
      
      
    data_ = mmap(NULL, file_.size, PROT_READ, MAP_SHARED, file_.handle, 0);
    if (data_ == MAP_FAILED) {
        throw MmapException("mmap error");
    }

    parseFormat_();
    Offset offset;
    parseDimensions_(offset);
    
    
    int status = madvise(data_, file_.size, MADV_RANDOM|MADV_WILLNEED);
    if (status < 0) {
        throw MmapException("mmap advise error");
    }
  }

  
  void NcMmap::parseFormat_()
  {
    // check we have a NetCDF File
    if (memcmp("CDF", (Byte*)data_, 3) != 0)
    {
      throw NetCdfException("not a netcdf file");
    }
    
    // check if classic or 64bit format
    switch (*getByteP_(3))
    {
      case 1:
	file_.is64bit = false;
	#ifdef DEBUG
	  std::cout << "classic format" << std::endl;
	#endif
	break;
	
      case 2:
	file_.is64bit = true;
	#ifdef DEBUG
	  std::cout << "64bit offset format" << std::endl;
	#endif
	break;
	
      default:
	#ifdef DEBUG
	  std::cout << "format: " << (int)*getByteP_(3) << std::endl;
	#endif
	throw NetCdfException("unknown netcdf format");
    }
  }
  
  void NcMmap::parseDimensions_(Offset &offset)
  {
    
    dimensions_.nRecords = be32toh(*((Int*)data_+1));
    #ifdef DEBUG
      std::cout << "records : " << dimensions_.nRecords << std::endl;
    #endif
      
    offset = 11;
 
    switch (*getByteP_(offset))
    {
      case 10: // dimensions marker
	break;
      case 0:
	throw NetCdfException("this file has 0 dimensions");
	break;
      default:
	throw NetCdfException("unexpected dimensions marker");
    }
    
    ++offset;
    
    int nDims = be32toh(*(int*)getByteP_(offset));
    #ifdef DEBUG
      std::cout << "dimensions : " << nDims << std::endl;
    #endif
      
    offset += 4;
    
    for (int i=0; i<nDims; i++)
    {
      size_t nameLength = getInt_(offset);
      offset += 4;
      std::string name ((char*)((char*)data_+offset), nameLength);
      offset += nameLength;
      int mod = nameLength%4;
      if (mod != 0) offset += 4-mod;
      
      int nVals = getInt_(offset);
      #ifdef DEBUG
	std::cout << i << ": " << name << " (" << nVals << ")" << std::endl;
      #endif
      offset += 4;
      
      dimensions_.name.push_back(name);
      dimensions_.size.push_back(nVals);
    }    
    
  }
  
  
  NcMmap::Byte* NcMmap::getByteP_(Offset const &offset) {return (Byte *)data_+offset;}
  NcMmap::Int NcMmap::getInt_(Offset const &offset) {return be32toh(*(int*)getByteP_(offset));};
  
  NcMmap::~NcMmap()
  {
    
    munmap(data_, file_.size);
    close(file_.handle);
    
    #ifdef DEBUG
      std::cout << "close " << file_.name << std::endl;
    #endif
      
  }

}