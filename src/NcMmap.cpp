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
 * MeteoDataServer is distributed in the hope that it will be useful,
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
        throw std::string("ERROR : can't open " + file_.name);
    }
    
    #ifdef DEBUG
      std::cout << "open " << file_.name << std::endl;
    #endif
    
    struct stat statInfos;
    if (stat(file_.name.c_str(), &statInfos) == -1) {
        throw std::string("stat error");
    }
    
    file_.size = statInfos.st_size;
    
    #ifdef DEBUG
      std::cout << "filesize " << file_.size/1e6 << " Mb" << std::endl;
    #endif
      
      
    data_ = mmap(NULL, file_.size, PROT_READ, MAP_SHARED, file_.handle, 0);
    if (data_ == MAP_FAILED) {
        throw std::string("mmap error");
    }

    parseFormat_();
    parseRecordsCount_();
   
    int status = madvise(data_, file_.size, MADV_RANDOM|MADV_WILLNEED);
    if (status < 0) {
        throw std::string("mmap advise error");
    }
  }
  
  void NcMmap::parseRecordsCount_()
  {
    
    
    
    
    
    #ifdef DEBUG
      std::cout << "read " << *((char*)data_+1) << std::endl;
    #endif
    
  }
  
  
  void NcMmap::parseFormat_()
  {
    // check we have a NetCDF File
    if (memcmp("CDF", (char*)data_, 3) != 0)
    {
      throw std::string("not a netcdf file");
    }
    
    // check if classic or 64bit format
    switch (*getByte(3))
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
	  std::cout << "format: " << (int)*getByte(3) << std::endl;
	#endif
	throw std::string("unknow netcdf format");
    }
  }
  
  
  NcMmap::Byte* NcMmap::getByte(Offset offset) {return (uint8_t *)data_+offset;}
  
  
  NcMmap::~NcMmap()
  {
    
    munmap(data_, file_.size);
    close(file_.handle);
    
    #ifdef DEBUG
      std::cout << "close " << file_.name << std::endl;
    #endif
      
  }

}