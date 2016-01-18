#include "ServiceProxyBase.h"
#include "cutils/atomic.h"
#include "Log.h"
#include "ServiceProxyBaseImpl.h"
#include "ServiceBaseDefines.h"

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
    BSLOGD("ServiceProxyBase::~ServiceProxyBase %s", m_impl->name().c_str());
    android_atomic_and(~SERVICE_CONNECTED, &m_status);
}

std::string ServiceProxyBase::name()
{
    if(m_impl != NULL) {
        return m_impl->name();
    }
    return std::string();
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

bool ServiceProxyBase::tryConnect()
{
    if(m_impl != NULL) {
        return m_impl->tryConnect();
    }
    return false;
}

int ServiceProxyBase::sendRequest(unsigned int code, const android::Parcel &data, android::Parcel *reply, unsigned int flags)
{
    if(m_impl != NULL) {
        return m_impl->sendRequest(code, data, reply, flags);
    }
    return BS_NO_CONNECTION;
}
