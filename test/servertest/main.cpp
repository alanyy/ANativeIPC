#include "TestService.h"

int main(int argc, char *argv[])
{
    TestService test;
    test.initialize();
    test.exec();
    test.finalize();
    return 0;
}
