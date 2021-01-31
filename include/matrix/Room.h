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


#ifndef MATRIX_CPP_SDK_ROOM_H
#define MATRIX_CPP_SDK_ROOM_H
#include <string>
#include <vector>
#include <map>

#include "WebAPI.h"
#include "Event.h"

namespace Matrix {

    class Client;
    class Room {
        std::string RoomID;
        Client* client;
        WebAPI* webapi;
        std::vector<Event> timeline;
        std::map<std::string,RoomMember> members;
        void fetchMembers();
    public:
        Room();
        Room(const std::string &roomId, Client *client);
        const std::string &getRoomId() const;
        const std::string &getDisplayName() const;
        void addEvent(Event e);
        RoomMember getMember(std::string user_id) const;
        void sync();
        const std::vector<Event> getTimeline() const;
    };
}

#endif //MATRIX_CPP_SDK_ROOM_H