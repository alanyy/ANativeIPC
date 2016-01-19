#ifndef SERVICEBASE_H
#define SERVICEBASE_H

#include "ServiceBaseGlobal.h"
#include "binder/Parcel.h"
#include <utils/String8.h>

class ServiceBaseImpl;

class SERVICEBASE_API ServiceBase
{
public:
    explicit ServiceBase(const char *name);
    virtual ~ServiceBase();

    virtual android::String8 name();

    virtual void initialize();
    virtual void finalize();
    virtual void exec();

    virtual int onRequest(unsigned int code, const android::Parcel &data,
                          android::Parcel *reply, unsigned int flags);

protected:
    virtual int addService();
    virtual int removeService();
private:
    DISABLE_COPY(ServiceBase)
    android::sp<ServiceBaseImpl> m_impl;
};

#endif // SERVICEBASE_H
