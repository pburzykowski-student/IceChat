#include "UserImpl.h"


UserImpl::UserImpl(const string& userName, const string& password){
    this->userName = userName;
    this->password = password;
}

void UserImpl::SendMessage(const string & room,
                           const UserPrx& user,
                           const string& message,
                           const Ice::Current&){


    cout << "(Room: " << room << ") " << user->getName() << ": " << message << endl;
}

void UserImpl::SendPrivateMessage(const UserPrx& who,
                                  const string& message,
                                  const Ice::Current&){
    cout <<"(Whisper) " << who->getName() << ": " << message << endl;
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