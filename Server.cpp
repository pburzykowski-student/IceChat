#include <Ice/Ice.h>
#include <Chat.h>
using namespace std;
using namespace Chat;


class ServerImpl : public Server{
private:
    RoomList roomList;
    UserList userList;
    Ice::ObjectAdapterPtr adapter;
public:
    ServerImpl(Ice::ObjectAdapterPtr adapter);
    virtual RoomPrx CreateRoom(const string& name,
                               const Ice::Current&);
    virtual RoomList getRooms(const Ice::Current&);
    virtual RoomPrx FindRoom(const string& name,
                             const Ice::Current&);
    virtual void RegisterUser(const string& name,
                              const string& password,
                              const Ice::Current&);
    virtual UserPrx FindUser(const string& userName, const Ice::Current&);


};


class RoomImpl : public Room {
private:
    string name;
    UserList userList;
    ServerPrx serverPrx;
public:
    RoomImpl(string name, ServerPrx serverPrx);
    virtual string getName(const ::Ice::Current&);
    virtual UserList getUsers(const ::Ice::Current&);
    virtual void AddUser(const UserPrx& user,
                         const string& password,
                         const ::Ice::Current&);
    virtual void SendMessage(const UserPrx& user,
                             const string& message,
                             const string& password,
                             const Ice::Current&);
    virtual void Destroy(const Ice::Current&);
    virtual void LeaveRoom(const UserPrx& user,
                           const string& password,
                           const Ice::Current&);
};



class UserImpl : public User{
private:
    string password;
    string userName;
public:
    UserImpl(const string& userName, const string& password);

    virtual void SendMessage(const RoomPrx& where,
                             const UserPrx& who,
                             const string& message,
                             const Ice::Current&);
    virtual void SendPrivateMessage(const UserPrx& who,
                                    const string& message,
                                    const Ice::Current&);
    virtual string getName(const Ice::Current&);

    virtual void ChangePassword(const string& oldpassword,
                                const string& newpassword,
                                const Ice::Current&);
    virtual string getPassword(const Ice::Current&);
};



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


ServerImpl::ServerImpl(Ice::ObjectAdapterPtr adapter){
    this->adapter = adapter;
}

RoomPrx ServerImpl::CreateRoom(const string& name,
                               const Ice::Current&){

    ServerPrx serverPrx = ServerPrx::uncheckedCast(adapter->addWithUUID(this));
    RoomPtr room = new RoomImpl("main", serverPrx);
    RoomPrx roomPrx = RoomPrx::uncheckedCast(adapter->addWithUUID(room));
    roomList.push_back(roomPrx);

    return roomPrx;
}

RoomList ServerImpl::getRooms(const Ice::Current&){
    return roomList;
}

RoomPrx ServerImpl::FindRoom(const string& name,
                             const Ice::Current&){
    for(vector<RoomPrx>::iterator it = roomList.begin(); it != roomList.end(); ++it) {
        if((*it)->getName() == name){
            return *it;
        }
    }
    throw NoSuchRoomExists();
}

void ServerImpl::RegisterUser(const string& name,
                              const string& password,
                              const Ice::Current&) {

    UserPtr user = new UserImpl(name, password);
    UserPrx userPrx = UserPrx::uncheckedCast(adapter->addWithUUID(user));
    userList.push_back(userPrx);

}

UserPrx ServerImpl::FindUser(const string& userName, const Ice::Current&){

    for(vector<UserPrx>::iterator it = userList.begin(); it != userList.end(); ++it) {
        if((*it)->getName() == userName){
            return *it;
        }
    }
    throw NoSuchUserExists();
}




RoomImpl::RoomImpl(string name, ServerPrx serverPrx){
    this->name = name;
    this->serverPrx = serverPrx;
}

string RoomImpl::getName(const Ice::Current&){
    return this->name;
}

UserList RoomImpl::getUsers(const ::Ice::Current&){
    return this->userList;
}

void RoomImpl::AddUser(const UserPrx& user,
                       const string& password,
                       const ::Ice::Current&){

    userList.push_back(user);
}


void RoomImpl::SendMessage(const UserPrx& user,
                           const string& message,
                           const string& password,
                           const Ice::Current&){

    cout << "Sending room message" << endl;
    RoomPrx roomPrx = serverPrx->FindRoom(this->name);
    for(vector<UserPrx>::iterator it = userList.begin(); it != userList.end(); ++it) {
        (*it)->SendMessage(roomPrx, user, message);
    }
    cout << "done" << endl;
}

void RoomImpl::Destroy(const Ice::Current&){

}

void RoomImpl::LeaveRoom(const UserPrx& user,
                         const string& password,
                         const Ice::Current&){
    userList.erase(find(userList.begin(), userList.end(),user));
}












UserImpl::UserImpl(const string& userName, const string& password){
    this->userName = userName;
    this->password = password;
}

void UserImpl::SendMessage(const RoomPrx& where,
                           const UserPrx& who,
                           const string& message,
                           const Ice::Current&){

    cout << "Sombody is sending message here! " << message << endl;

    //where->SendMessage(who, message, password);
}

void UserImpl::SendPrivateMessage(const UserPrx& who,
                                  const string& message,
                                  const Ice::Current&){
    cout << who->getName() << ": " << message << endl;
}

string UserImpl::getName(const Ice::Current&){
    return userName;
}


void UserImpl::ChangePassword(const string& oldpassword,
                              const string& newpassword,
                              const Ice::Current&){

}

string UserImpl::getPassword(const Ice::Current&){

}