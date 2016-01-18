#ifndef SERVICEBASEDEFINES_H
#define SERVICEBASEDEFINES_H

#include "utils/Errors.h"
#include "binder/IBinder.h"

enum Error_Num {
    BS_NO_ERROR = android::NO_ERROR,
    BS_NO_MEMORY = android::NO_MEMORY,
    BS_TIME_OUT = android::TIMED_OUT,
    BS_UNKNOWN_TRANSACTION = android::UNKNOWN_TRANSACTION,
    BS_NO_CONNECTION = -1,
    BS_UNKNOWN = android::UNKNOWN_ERROR
};

enum Call_Transaction {
    ADD_ASYNC_BINDER = android::IBinder::FIRST_CALL_TRANSACTION,
    SERVICE_FIRST_TRANSACTION
};

enum Call_Flags {
    ASYN_CALL = android::IBinder::FLAG_ONEWAY
};

#endif // SERVICEBASEDEFINES_H
