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

#ifndef OMD_NCMMAP__NCMMAP__H
#define OMD_NCMMAP__NCMMAP__H

#include <unistd.h> // for close()
#include <fcntl.h>  // for open()
#include <sys/stat.h> // for stat
#include <sys/mman.h> // for mmap
#include <stdint.h> // for integers types
#include <cstring> 
#include <string>
#include <endian.h>

#ifdef DEBUG
#include <iostream>
#endif


namespace OpenMeteoData {

  
  class NcMmap {
    
  public:
    
    typedef uint8_t Byte;
    typedef uint8_t Char;
    typedef int16_t Short;
    typedef int32_t Int;
    typedef int64_t Int64;
    typedef float Float;
    typedef double Double;
    
    typedef std::string FileName;
    typedef uint64_t Offset;
    
    typedef std::string FileException;
    typedef std::string MmapException;
    typedef std::string NetCdfException;
    
    struct File {
      FileName name;
      int handle;
      size_t size;
      bool is64bit;
    };
    
    struct Infos {
      int nRecords;
    };
    
    
    NcMmap(FileName const &);
    ~NcMmap();
    
  protected:
  private:
    
    File file_;
    void *data_;
    Infos infos_;
        
    void parseFormat_();
    void parseRecordsCount_();
    void parseDimensions_(Offset &);
    Byte *getByteP_(Offset const &);
    Int getInt_(Offset const &);
  }; /* End of class NcMmap. */

} /* End of namespace OpenMeteoData. */

#endif  /* Undefined OMD_NCMMAP__NCMMAP__H. */