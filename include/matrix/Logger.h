/* 
   Copyright (C) 2021 Toldi Balázs Ádám
  
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  

   File created at: 2021. 01. 28.
*/

#ifndef MATRIX_CPP_SDK_LOGGER_H
#define MATRIX_CPP_SDK_LOGGER_H
#include <array>
#include <string>
#include <fstream>
#include <json/json.h>

namespace Matrix {
    class Logger {

        std::ofstream ofstream;
    public:
        enum level {
            DEBUG = 0,
            INFO = 1,
            WARNING = 2,
            ERROR = 3
        };
        Logger(std::string fileName = "matrix.log");
        void log(level lvl,std::string string);
        void info(std::string string);
        void warning(std::string string);
        void error(std::string string);
        void debug(std::string string);
        static std::string JsonToString(Json::Value json);
    };
}

#endif //MATRIX_CPP_SDK_LOGGER_H
