#include <Ice/Ice.h>
#include <Chat.h>
#include <time.h>
#include "Impl/UserImpl.h"

using namespace std;
using namespace Chat;

class ClientHandler{

private:
    Ice::CommunicatorPtr ic;
    ServerPrx serverPrx;
    UserPrx userPrx;
    string userName;
    string password;
    int port;

    void connectToServer(){
        this->port = randPort();
        ic = Ice::initialize();
        Ice::ObjectPrx base = ic->stringToProxy(
                "Server:default -p 10000");
        serverPrx = ServerPrx::checkedCast(base);
        if (!serverPrx)
            throw "Invalid proxy";
    }

    int randPort(){
        srand (time(NULL));
        return rand() % 1000 + 10000;
    }

    string sgetPort(){
        ostringstream ss;
        ss << port;
        return ss.str();
    }

public:
    ClientHandler(){
        connectToServer();
    }

    Ice::CommunicatorPtr getCommunicatorPtr(){
        return ic;
    }

    string getUserName(){
        return userName;
    }

    bool createUser(string userName, string password){
        try{
            serverPrx->FindUser(userName);
        } catch(NoSuchUserExists e){
            this->userName = userName;
            this->password = password;


            cout << "Port: " << endl;
            cout << sgetPort() << endl;

            Ice::ObjectAdapterPtr adapter
                    = ic->createObjectAdapterWithEndpoints(userName, "default -p " + sgetPort());
            adapter->activate();



            UserPtr userPtr = new UserImpl(userName, password);
            userPrx = UserPrx::uncheckedCast(adapter->addWithUUID(userPtr));
            serverPrx->RegisterUser(userPrx);
            return true;
        }
        return false;

    }

};

class commandSplitter{
private:
    string key;
    string arg;

public:

    string getKey(){
        return key;
    }

    string getArg(){
        return arg;
    }

    void execute(string command){
        int position = 0;
        char current = command[position];

        while(current != ' ' && position < command.length()){
            current = command[position++];
            key += current;
        }

        for(int i = position; i < command.length(); i++){
            current = command[position++];
            arg += current;
        }
    }
};


int
main(int argc, char* argv[])
{
    int status = 0;
    Ice::CommunicatorPtr ic;
    try {
        ClientHandler client;
        ic = client.getCommunicatorPtr();
        string userName, password;

        cout << endl;
        cout << endl;
        cout << "============ Chat Ice 2018 ============" << endl;
        cout << endl;
        cout << "Type \"/exit\" to close program" << endl;
        cout << "Type \"/help\" to get command list" << endl;
        cout << "But first lets register your identity!" << endl;
        cout << endl;
        cout << "========================================" << endl;
        cout << endl;
        cout << endl;

        bool isRegistred;
        do {
            cout << "=== Register user === " << endl;
            cout << "User name: " << endl;
            cin >> userName;
            cout << "Password" << endl;
            cin >> password;

            isRegistred = client.createUser(userName, password);
            if(!isRegistred){
                cout << "Sorry, that user name is not available!" << endl;
            }
        } while(!isRegistred);






        string t = "/say cos tam";
        commandSplitter cs;
        cs.execute(t);

        cout << "Key:" << endl;
        cout << cs.getKey() << endl;
        cout << "Arg: " << endl;
        cout << cs.getArg() << endl;


        string end;
        cin >> end;

        /*UserPtr user = new UserImpl("User1", "pass1");
        UserPrx userPrx = UserPrx::uncheckedCast(adapter->addWithUUID(user));
        server->RegisterUser(userPrx);

        userPrx = server->FindUser("User1");
        RoomPrx roomPrx = server->CreateRoom("room1");
        roomPrx->AddUser(userPrx, "pass1");

        cout << "Sending message" << endl;
        roomPrx->SendMessage(userPrx, "test", "pass1");*/

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
