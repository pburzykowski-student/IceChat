#include <Ice/Ice.h>
#include <Chat.h>
#include "Impl/UserImpl.h"

using namespace std;
using namespace Chat;


int
main(int argc, char* argv[])
{
    int status = 0;
    Ice::CommunicatorPtr ic;
    try {
        ic = Ice::initialize(argc, argv);
        Ice::ObjectPrx base = ic->stringToProxy(
                                "Server:default -p 10000");
        ServerPrx server = ServerPrx::checkedCast(base);
        if (!server)
            throw "Invalid proxy";


        Ice::ObjectAdapterPtr adapter = ic->createObjectAdapterWithEndpoints("UserUser1", "default -p 10001 ");
        adapter->activate();

        UserPtr user = new UserImpl("User1", "pass1");
        UserPrx userPrx = UserPrx::uncheckedCast(adapter->addWithUUID(user));
        server->RegisterUser(userPrx);

        userPrx = server->FindUser("User1");
        RoomPrx roomPrx = server->CreateRoom("room1");
        roomPrx->AddUser(userPrx, "pass1");

        roomPrx->SendMessage(userPrx, "test", "pass1");

	cout << "Working  <--!" << endl;
    } catch (const Ice::Exception& ex) {
        cerr << ex << endl;
        status = 1;
    } catch (const char* msg) {
        cerr << msg << endl;
        status = 1;
    }
    if (ic)
        ic->destroy();
    return status;
}
