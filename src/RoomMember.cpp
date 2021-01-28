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



#include "RoomMember.h"

Matrix::RoomMember::RoomMember(const std::string &userId) : userID(userId) {}

Matrix::RoomMember::RoomMember(const std::string &userId, const std::string &displayName) : userID(userId),
                                                                                            displayName(displayName) {}

const std::string &Matrix::RoomMember::getDisplayName() const {
    return displayName;
}

void Matrix::RoomMember::setDisplayName(const std::string &displayName) {
    RoomMember::displayName = displayName;
}

const std::string &Matrix::RoomMember::getUserId() const {
    return userID;
}
