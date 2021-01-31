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



#ifndef MATRIX_CPP_SDK_EVENT_H
#define MATRIX_CPP_SDK_EVENT_H
#include <string>
#include <json/json.h>

#include "RoomMember.h"

namespace Matrix {
    class Event {
        std::string id;
        std::string type;
        Json::Value content;
        RoomMember target;
        RoomMember sender;
    public:
        Event(const std::string &id, const std::string &type, const RoomMember &sender,
              const Json::Value &content);
        const std::string &getId() const;
        const std::string &getType() const;
        const Json::Value &getContent() const;

        const RoomMember &getSender() const;
    };
}

#endif //MATRIX_CPP_SDK_EVENT_H
