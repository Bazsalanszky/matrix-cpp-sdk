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
#include "Client.h"

const std::string &Matrix::Room::getRoomId() const {
    return RoomID;
}

const std::string &Matrix::Room::getDisplayName() const {
    Json::Value response =webapi->get("/_matrix/client/r0/rooms/"+RoomID+"/state/m.room.name/?access_token="+client->getToken());
    if(response["name"].isString()){
        return response["name"].asString();
    }
    return "";
}

Matrix::Room::Room(const std::string &roomId, Matrix::Client *client) : RoomID(roomId), client(client),webapi(client->getWebAPI()) {}

void Matrix::Room::addEvent(Event e) {
    timeline.push_back(e);
}

Matrix::RoomMember Matrix::Room::getMember(std::string id) const {
    return members.at(id);
}

Matrix::Room::Room() {

}

void Matrix::Room::sync() {
    fetchMembers();
    Json::Value sync_response = webapi->get("/_matrix/client/r0/sync?access_token="+client->getToken());
    Json::Value joined = sync_response["rooms"]["join"];
    for (int i = 0; i < joined.size(); ++i) {
        Json::Value room = joined[joined.getMemberNames()[i]];
        Json::Value timeline_json = room["timeline"]["events"];
        for (int j = 0; j < timeline_json.size(); ++j) {
            if(joined.getMemberNames()[i] == RoomID) {
                addEvent(Event(timeline_json[j]["event_id"].asString(), timeline_json[j]["type"].asString(),
                                      getMember(timeline_json[j]["sender"].asString()),
                                      timeline_json[j]["content"]));
            }
        }
    }
}

void Matrix::Room::fetchMembers() {
    Json::Value response = webapi->get("/_matrix/client/r0/rooms/"+RoomID+"/members?access_token="+client->getToken());
    response = response["chunk"];
    for (int i = 0; i < response.size(); ++i) {
        members[response[i]["sender"].asString()] = RoomMember(response[i]["sender"].asString(),response[i]["content"]["displayname"].asString());
    }
}

const std::vector<Matrix::Event> Matrix::Room::getTimeline() const {
    return timeline;
}
