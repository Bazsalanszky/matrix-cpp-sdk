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

   File created at: 2021. 02. 02.
*/

#include "Syncer.h"
#include "Room.h"
#include "Client.h"

void Matrix::Syncer::sync() {

    Json::Value sync_response = (last_sync_time.empty()) ? webapi->get(
            "/_matrix/client/r0/sync?access_token=" + client->getToken())
                                                         : webapi->get(
                    "/_matrix/client/r0/sync?access_token=" + client->getToken() + "&since=" + last_sync_time);
    syncInvites(sync_response["rooms"]["invite"]);
    syncRooms(sync_response["rooms"]["join"]);
    last_sync_time = sync_response["next_batch"].asString();

}

void Matrix::Syncer::addEventListener(Matrix::EventListener *eventListener) {
    listeners.push_back(eventListener);
}

void Matrix::Syncer::start() {
    if (sync_thread == nullptr) {
        std::thread t(&Syncer::syncThread, this);
        sync_thread = &t;
        t.join();
    }
}

void Matrix::Syncer::syncThread() {
    using namespace std::chrono_literals;
    while (1) {
        sync();
        std::this_thread::sleep_for(1s);
    }
}

void Matrix::Syncer::fetchInvites(const Json::Value &sync_response) {
    for (int i = 0; i < sync_response.size(); ++i) {
        client->addInvitedRoom(Room(sync_response.getMemberNames()[i], client));
    }
}

void Matrix::Syncer::syncRooms(const Json::Value &sync_response) {
    fetchRooms(sync_response);
    for (auto it = client->rooms_joined.begin(); it != client->rooms_joined.end(); it++) {
        for (int i = 0; i < sync_response.size(); ++i) {
            if (sync_response.getMemberNames()[i] == it->first) {
                Json::Value timeline_json = sync_response[sync_response.getMemberNames()[i]]["timeline"]["events"];
                for (int j = 0; j < timeline_json.size(); ++j) {
                    Event e(timeline_json[j]["event_id"].asString(), timeline_json[j]["type"].asString(),
                            it->second.getMember(timeline_json[j]["sender"].asString()),
                            timeline_json[j]["content"], it->first);
                    callListeners(e);
                    it->second.addEvent(e);
                }
                break;
            }
        }
    }
}

void Matrix::Syncer::fetchRooms(const Json::Value &join_json) {
    for (int i = 0; i < join_json.size(); ++i) {
        if (client->rooms_joined.find(join_json.getMemberNames()[i]) == client->rooms_joined.end()) {
            client->addRoom(Room(join_json.getMemberNames()[i], client));
        }
    }
}

void Matrix::Syncer::syncInvites(const Json::Value &sync_response) {
    fetchInvites(sync_response);
    for (auto it = client->rooms_invited.begin(); it != client->rooms_invited.end(); it++) {
        for (int i = 0; i < sync_response.size(); ++i) {
            if (sync_response.getMemberNames()[i] == it->first) {
                Json::Value timeline_json = sync_response[sync_response.getMemberNames()[i]]["invite_state"]["events"];
                for (int j = 0; j < timeline_json.size(); ++j) {
                    Event e(timeline_json[j]["event_id"].asString(), timeline_json[j]["type"].asString(),
                            it->second.getMember(timeline_json[j]["sender"].asString()),
                            timeline_json[j]["content"], it->first);
                    callListeners(e);
                    it->second.addEvent(e);
                }
                break;
            }
        }
    }
}

Matrix::Syncer::Syncer(Matrix::Client *client) : client(client), webapi(client->getWebAPI()) {}

void Matrix::Syncer::callListeners(Matrix::Event event) {
    for (int i = 0; i < listeners.size(); ++i) {
        listeners[i]->onEvent(event);
    }
}
