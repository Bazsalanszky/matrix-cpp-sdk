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



#ifndef MATRIX_CPP_SDK_SYNCER_H
#define MATRIX_CPP_SDK_SYNCER_H
#include <thread>
#include <vector>

#include "WebAPI.h"
#include "matrix/EventListener.h"

namespace Matrix{
    class Client;
    class Syncer {
        WebAPI* webapi;
        Matrix::Client* client;
        std::vector<EventListener*> listeners;
        std::thread* sync_thread = nullptr;
        std::string last_sync_time;
        void fetchRooms(const Json::Value &join_json);

        void syncRooms(const Json::Value& sync_response);
        void syncInvites(const Json::Value& sync_response);
        void fetchInvites(const Json::Value& sync_response);
        void syncThread();
        void callListeners(Event event);
    public:
        Syncer(Client *client);

        void start();
        void addEventListener(EventListener* eventListener);
        void sync();
    };
}

#endif //MATRIX_CPP_SDK_SYNCER_H
