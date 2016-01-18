#ifndef ANONYMOUSBINDER_H
#define ANONYMOUSBINDER_H

#include "binder/Binder.h"
class AnonymousBinder : public android::BBinder
{
public:
    AnonymousBinder();
    ~AnonymousBinder();
    virtual android::status_t onTransact(uint32_t code,
                                         const android::Parcel& data,
                                         android::Parcel* reply,
                                         uint32_t flags = 0);
};

#endif // ANONYMOUSBINDER_H
