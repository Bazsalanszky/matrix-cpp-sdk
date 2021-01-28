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

#include "Logger.h"

#include <iostream>

static const char* asciiCodes[5] = {"\x1B[36m","\x1B[37m","\x1B[33m","\x1B[31m","\033[0m"};

Matrix::Logger::Logger(std::string fileName) {
    ofstream = std::ofstream(fileName);
}

void Matrix::Logger::log(Matrix::Logger::level lvl, std::string string) {
    std::cout << asciiCodes[lvl] << string << asciiCodes[4] <<std::endl;
}

void Matrix::Logger::info(std::string string) {
    log(INFO,string);
}

void Matrix::Logger::warning(std::string string) {
    log(WARNING,string);
}

void Matrix::Logger::error(std::string string) {
    log(ERROR,string);
}

void Matrix::Logger::debug(std::string string) {
    log(DEBUG,string);
}

std::string Matrix::Logger::JsonToString(Json::Value json) {
    Json::FastWriter fst;
    return fst.write(json);
}
