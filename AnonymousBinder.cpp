#include "AnonymousBinder.h"
#include "Log.h"
#include "binder/Parcel.h"
#include "ServiceProxyBase.h"

AnonymousBinder::AnonymousBinder(ServiceProxyBase *interface)
    : android::BBinder()
    , m_remoteId(-1)
    , m_if(interface)
{
}

AnonymousBinder::~AnonymousBinder()
{
    m_if = NULL;
}

void AnonymousBinder::setRemoteId(SenderId &id)
{
    BSLOGD("AnonymousBinder::setRemoteId id is %d", id);
    m_remoteId = id;
}

SenderId &AnonymousBinder::remoteId()
{
    return m_remoteId;
}

android::status_t AnonymousBinder::onTransact(uint32_t code, const android::Parcel &data, android::Parcel *reply, uint32_t flags)
{
    BSASSERT(flags & ASYN_CALL, "AnonymousBinder::onTransact");
    if(m_if) {
        int ret = m_if->onAsyncResponse(code, data);
        if(ret == BS_NO_ERROR) {
            return android::NO_ERROR;
        }
    }
    return BBinder::onTransact(code, data, reply, flags);
}
