#include "TestServiceProxy.h"

int main(int argc, char *argv[])
{
    TestServiceProxy proxy;
    proxy.setupAsynCall();
    proxy.sendTestCommand();
    return 0;
}
