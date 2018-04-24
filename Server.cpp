#include <Ice/Ice.h>
#include <Chat.h>

#include "Impl/ServerImpl.h"
#include "Impl/RoomImpl.h"
#include "Impl/UserImpl.h"

using namespace std;
using namespace Chat;

int main(int argc, char* argv[]){

    //Ice::ObjectPtr


    int status = 0;
    Ice::CommunicatorPtr ic;
    try {
        ic = Ice::initialize(argc, argv);
        Ice::ObjectAdapterPtr adapter
                = ic->createObjectAdapterWithEndpoints(
                        "SimpleChatAdapter", "default -p 10000");


        /*Ice::ObjectPtr object = new ChatI;
        adapter->add(object,
                     ic->stringToIdentity("SimpleChat"));*/


        ServerPtr server = new ServerImpl(adapter);

        adapter->add(server, ic->stringToIdentity("Server"));

        /*UserPtr user = new UserImpl("name", "pass");
        ServerPtr server = new ServerImpl(adapter);
        ServerPrx serverPrx = ServerPrx::uncheckedCast(adapter->addWithUUID(server));
        RoomPtr room = new RoomImpl("main", serverPrx);*/

        /*RoomList roomList1;
        roomList1.push_back(RoomPrx::uncheckedCast(adapter->addWithUUID(new RoomImpl("main"))));
        roomList1.push_back(RoomPrx::uncheckedCast(adapter->addWithUUID(new RoomImpl("lol"))));
        */

        /* for(vector<RoomPrx>::iterator it = roomList1.begin(); it != roomList1.end(); ++it) {
                std::cout << (*it)->getName() << endl;
        }*/


        adapter->activate();
        ic->waitForShutdown();
    } catch (const Ice::Exception& e) {
        cerr << e << endl;
        status = 1;
    } catch (const char* msg) {
        cerr << msg << endl;
        status = 1;
    }
    if (ic) {
        try {
            ic->destroy();
        } catch (const Ice::Exception& e) {
            cerr << e << endl;
            status = 1;
        }
    }
    return status;
}


