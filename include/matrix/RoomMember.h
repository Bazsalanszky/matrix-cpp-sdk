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



#ifndef MATRIX_CPP_SDK_ROOMMEMBER_H
#define MATRIX_CPP_SDK_ROOMMEMBER_H
#include <string>

namespace Matrix {
    class RoomMember {
        std::string userID;
        std::string displayName;
    public:
        RoomMember();
        RoomMember(const std::string &userId);
        RoomMember(const std::string &userId, const std::string &displayName);
        const std::string &getDisplayName() const;
        void setDisplayName(const std::string &displayName);
        const std::string &getUserId() const;
        RoomMember(const RoomMember& rm);
    };
}

#endif //MATRIX_CPP_SDK_ROOMMEMBER_H
