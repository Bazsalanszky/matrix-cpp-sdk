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



#include "Room.h"

const std::string &Matrix::Room::getRoomId() const {
    return RoomID;
}

const std::string &Matrix::Room::getDisplayName() const {
    Json::Value response =client->getWebAPI()->get("/_matrix/client/r0/rooms/"+RoomID+"/state/m.room.name/?access_token="+client->getToken());
    if(response["name"].isString()){
        return response["name"].asString();
    }
    return "";
}
