#include <Ice/Ice.h>
#include <Chat.h>
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
                                "SimpleChat:default -p 10000");
        /*ChatPrx chat = ChatPrx::checkedCast(base);
        if (!chat)
            throw "Invalid proxy";*/

	cout << "Working!" << endl;
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
