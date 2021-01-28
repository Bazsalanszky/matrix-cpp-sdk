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



#include "Event.h"

const std::string &Matrix::Event::getId() const {
    return id;
}

const std::string &Matrix::Event::getType() const {
    return type;
}

const Json::Value &Matrix::Event::getContent() const {
    return content;
}

Matrix::Event::Event(const std::string &id, const std::string &type, const RoomMember &sender,
                     const Json::Value &content) : id(id), type(type),content(content),sender(sender),target(target) {}

const Matrix::RoomMember &Matrix::Event::getSender() const {
    return sender;
}
