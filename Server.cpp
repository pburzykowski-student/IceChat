#include <Ice/Ice.h>
#include <Chat.h>
using namespace std;
using namespace Chat;

class RoomI : public Room {
private:
 	string name;
public:	
	RoomI(string name);
	virtual string getName(const ::Ice::Current&);
	virtual UserList getUsers(const ::Ice::Current&);	
	virtual void AddUser(const UserPrx& who, const string& password, const ::Ice::Current&);
	virtual void SendMessage(const UserPrx& who, const string& message, const string& password, const Ice::Current&);
	virtual void Destroy(const Ice::Current&);
	virtual void LeaveRoom(const UserPrx& who, const string& password, const Ice::Current&);
};


RoomI::RoomI(string name){
	this->name = name;
}

string RoomI::getName(const Ice::Current&){
	return this->name;
}

UserList RoomI::getUsers(const ::Ice::Current&){
	UserList userList;
	return userList;
}


void RoomI::AddUser(const UserPrx&, const string&, const ::Ice::Current&){

}


void RoomI::SendMessage(const UserPrx& who, const string& message, const string& password, const Ice::Current&){

}

void RoomI::Destroy(const Ice::Current&){

}

void RoomI::LeaveRoom(const UserPrx& who, const string& password, const Ice::Current&){

}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
class RoomFactoryI : public RoomFactory{
public:
	RoomFactoryI();
	virtual RoomPrx createRoom(const string& name, const Ice::Current&);
	virtual RoomList getRooms(const Ice::Current&);
};

	RoomFactoryI::RoomFactoryI(){

	}
	
	RoomPrx RoomFactoryI::createRoom(const string& name, const Ice::Current&){

	}

	RoomList RoomFactoryI::getRooms(const Ice::Current&){

	}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

class UserI : public User{
public:
	UserI();
	virtual void SendMessage(const RoomPrx& where, const UserPrx& who, const string& message, const Ice::Current&);
	virtual void SendPrivateMessage(const UserPrx& who, const string& message, const Ice::Current&);
	virtual string getName(const Ice::Current&);
};

	UserI::UserI(){

	}

	void UserI::SendMessage(const RoomPrx& where, const UserPrx& who, const string& message, const Ice::Current&){

	}
	
	void UserI::SendPrivateMessage(const UserPrx& who, const string& message, const Ice::Current&){

	}

	string UserI::getName(const Ice::Current&){

	}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

class ServerI : public Server{

public:
	ServerI();
	virtual RoomPrx CreateRoom(const string& name, const Ice::Current&);
	virtual RoomList getRooms(const Ice::Current&);
	virtual RoomPrx FindRoom(const string& name, const Ice::Current&);
	virtual void RegisterUser(const string& name, const string& password, const Ice::Current&); //throw UserAlreadyExists;
	virtual void ChangePassword(const UserPrx& user, const string& oldpassword, const string& newpassword, const Ice::Current&);
	virtual void getPassword(const string& user, const Ice::Current&);
	virtual void RegisterRoomFactory(const RoomFactoryPrx& factory, const Ice::Current&);
	virtual void UnregisterRoomFactory(const RoomFactoryPrx& factory, const Ice::Current&);
	
};

	ServerI::ServerI(){

	}

	RoomPrx ServerI::CreateRoom(const string& name, const Ice::Current&){

	}
	
	RoomList ServerI::getRooms(const Ice::Current&){

	}

	RoomPrx ServerI::FindRoom(const string& name, const Ice::Current&){

	}

	void ServerI::RegisterUser(const string& name, const string& password, const Ice::Current&) {

	}

	void ServerI::ChangePassword(const UserPrx& user, const string& oldpassword, const string& newpassword, const Ice::Current&){

	}

	void ServerI::getPassword(const string& user, const Ice::Current&){

	}

	void ServerI::RegisterRoomFactory(const RoomFactoryPrx& factory, const Ice::Current&){

	}
	
	void ServerI::UnregisterRoomFactory(const RoomFactoryPrx& factory, const Ice::Current&){

	}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////


int main(int argc, char* argv[]){
		
		//Ice::ObjectPtr
		RoomPtr room = new RoomI("main");
		RoomFactoryPtr roomFactory = new RoomFactoryI();
		UserPtr user = new UserI();
		ServerPtr server = new ServerI();

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

