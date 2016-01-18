#include "AnonymousBinder.h"
#include "Log.h"
#include "binder/Parcel.h"

AnonymousBinder::AnonymousBinder()
    : android::BBinder()
{
}

AnonymousBinder::~AnonymousBinder()
{
}

android::status_t AnonymousBinder::onTransact(uint32_t code, const android::Parcel &data, android::Parcel *reply, uint32_t flags)
{
    BSLOGD("AnonymousBinder::onTransact code is %d, reply is %d, flags is %x", code, reply, flags);
    const char *str = data.readCString();
    BSLOGD("AnonymousBinder::onTransact str is %s", str);
    return android::NO_ERROR;
}
