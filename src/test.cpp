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

int main(int argc, char *argv[])
{
  
  try
  {
    OpenMeteoData::NcMmap ncMmap ("../sampledata/netcdf64.nc");
    OpenMeteoData::NcMmap::Attribute title = ncMmap.getGlobalAttribute("corner_lats");
    for (size_t i=0; i<title.floatValue.size(); i++)
    {
      std::cout << " f: " << title.floatValue[i] << std::endl;
    }
    
  } 
  catch (std::string const &error)
  {
    std::cerr << error << std::endl;
  }
  
  return 0;
}