#include "RoomImpl.h"

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