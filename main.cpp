#include <iostream>
#include "src/Client.h"
#include "src/Logger.h"

int main(){
    Matrix::Logger logger("test.log");
    Matrix::Homeserver server(getenv("HOMESERVER_NAME"),getenv("HOMESERVER_URL"));
    Matrix::Client c(&server, getenv("ACCESS_TOKEN"));
    logger.warning(c.getUserId());
    c.setDisplayName("Magdi Josnő");
    c.syncRooms();
    for (auto it = c.begin();it != c.end();++it){
        /*std::vector<Matrix::Event> timeline = it->second.getTimeline();
        for (int i = 0; i < timeline.size(); ++i) {
            if(timeline[i].getType() == "m.room.message"){
                logger.debug(timeline[i].getSender().getDisplayName()+":\t" + timeline[i].getContent()["body"].asString());
            }
        }*/
        Json::Value m_body;
        m_body["msgtype"] = "m.text";
        m_body["body"] = "Have a nice day, everyone!";
        c.send(it->first,"m.room.message",m_body);
    }
    return 0;
}
