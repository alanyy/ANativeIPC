#include "ServiceProxyBase.h"
#include "cutils/atomic.h"
#include "Log.h"
#include "ServiceProxyBaseImpl.h"

const int SERVICE_CONNECTED = 0x1;

ServiceProxyBase::ServiceProxyBase(const char *name)
    : m_impl(NULL)
    , m_status(0)
{
    BSLOGD("ServiceProxyBase::ServiceProxyBase %s", name);
    m_impl = new ServiceProxyBaseImpl(name, this);
    tryConnect();
}

ServiceProxyBase::~ServiceProxyBase()
{
    BSLOGD("ServiceProxyBase::~ServiceProxyBase %s", m_impl->name().string());
    android_atomic_and(~SERVICE_CONNECTED, &m_status);
}

android::String8 ServiceProxyBase::name()
{
    if(m_impl != NULL) {
        return m_impl->name();
    }
    return android::String8();
}

bool ServiceProxyBase::isConnected()
{
    return (android_atomic_acquire_load(&m_status) & SERVICE_CONNECTED);
}

void ServiceProxyBase::onConnected()
{
    android_atomic_or(SERVICE_CONNECTED, &m_status);
}

void ServiceProxyBase::onDisconnected()
{
    android_atomic_and(~SERVICE_CONNECTED, &m_status);
}

int ServiceProxyBase::onAsyncResponse(unsigned int code, const android::Parcel &reply)
{
    UNUSED(code);
    UNUSED(reply);
    return BS_UNKNOWN_TRANSACTION;
}

bool ServiceProxyBase::tryConnect()
{
    if(m_impl != NULL) {
        return m_impl->tryConnect();
    }
    return false;
}

bool ServiceProxyBase::setupAsyncRequest()
{
    if(m_impl != NULL) {
        return m_impl->setupAsyncRequest();
    }
    return false;
}

bool ServiceProxyBase::teardownAsyncRequest()
{
    if(m_impl != NULL) {
        return m_impl->teardownAsyncRequest();
    }
    return false;
}

int ServiceProxyBase::sendSyncRequest(unsigned int code, const android::Parcel &data, android::Parcel *reply)
{
    if(m_impl != NULL) {
        return m_impl->sendSyncRequest(code, data, reply);
    }
    return BS_NO_CONNECTION;
}

bool ServiceProxyBase::prepareAsyncData(android::Parcel &data)
{
    if(m_impl != NULL) {
        return m_impl->prepareAsyncData(data);
    }
    return false;
}

int ServiceProxyBase::sendAsyncRequest(unsigned int code, const android::Parcel &data)
{
    if(m_impl != NULL) {
        return m_impl->sendAsyncRequest(code, data);
    }
    return BS_NO_CONNECTION;
}
