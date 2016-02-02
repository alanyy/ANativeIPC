#include "TestService.h"
#include "TestThread.h"

int main(int argc, char *argv[])
{
    TestService test;
    test.initialize();
    TestThread t;
    t.start();
    t.test();
//    test.exec();
    test.finalize();
    return 0;
}
