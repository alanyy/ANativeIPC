#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H

#include "utils/StrongPointer.h"
#include "utils/RefBase.h"
#include "ServiceBaseGlobal.h"

class Message;

class SERVICEBASE_API MessageHandler : public android::RefBase
{
public:
    MessageHandler();
    virtual ~MessageHandler();
    virtual void onReceiveMessage(const android::sp<Message> &msg) = 0;
private:
    DISABLE_COPY(MessageHandler)
};

#endif // MESSAGEHANDLER_H
