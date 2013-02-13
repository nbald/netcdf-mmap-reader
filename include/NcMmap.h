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
#include <vector>
#include <map>
#include <endian.h>

#ifdef DEBUG
#include <iostream>
#endif

  
  class NcMmap {
    
  public:
    
    typedef uint8_t Byte;
    typedef uint8_t Char;
    typedef int16_t Short;
    typedef int32_t Int;
    typedef int64_t Int64;
    typedef uint32_t UInt;
    typedef uint64_t UInt64;
    typedef float Float;
    typedef double Double;
    
    
    typedef std::string FileName;
    typedef std::string Name;
    typedef uint64_t Offset;
    typedef int Id;
    typedef int Count;
    
    typedef std::string FileException;
    typedef std::string MmapException;
    typedef std::string NetCdfException;
    
    enum Type {
     NC_BYTE      = 1, // 8-bit signed integers
     NC_CHAR      = 2, // text characters
     NC_SHORT     = 3, // 16-bit signed integers
     NC_INT       = 4, // 32-bit signed integers
     NC_FLOAT     = 5, // IEEE single precision floats
     NC_DOUBLE    = 6  // IEEE double precision floats
    };
    
    struct File {
      FileName name;
      int handle;
      size_t size;
      bool is64bit;
    };
    
    struct Dimensions {
      Count nRecords;
      std::vector<Name> name;
      std::vector<Count> size;
    };
    
    struct Attribute {
      Type type;
      std::vector<int64_t> integerValue;
      std::string stringValue;
      std::vector<double> floatValue;
    };
    
    typedef std::map<Name, Attribute> AttributesList;
    
    struct Variable {
      std::vector<int> dimensionsList;
      AttributesList attributesList;
      Type type;
      UInt bytes;
      UInt64 offset;
      UInt order;
    };
    
    typedef std::map<Name, Variable> VariablesList;
    
    NcMmap(FileName const &);
    NcMmap();
    ~NcMmap();
    
    Attribute* getGlobalAttribute(Name const &);
    VariablesList* getVariablesList();
    void openFile(FileName const &);

  protected:
  private:
    
    File file_;
    void *data_;
    Dimensions dimensions_;
    AttributesList globalAttributesList_;
    VariablesList variablesList_;
    
    void parseFormat_();
    void parseDimensions_(Offset &);
    AttributesList parseAttributes_(Offset &);
    void parseVariables_(Offset &);
    
    
    Byte *getByteP_(Offset const &);
    Int getInt_(Offset &);
    UInt getUInt_(Offset const &);
    UInt getUInt64_(Offset const &);
    Short getShort_(Offset const &);
    Float getFloat_(Offset const &);
    Double getDouble_(Offset const &);
    std::string getString_(Offset &);
  }; /* End of class NcMmap. */


#endif  /* Undefined OMD_NCMMAP__NCMMAP__H. */