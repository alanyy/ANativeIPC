#include "ServiceProxyBaseImpl.h"
#include "Log.h"
#include "ServiceBaseDefines.h"
#include "ServiceProxyBase.h"
#include "ServiceBinderProxy.h"
#include <unistd.h>
#include "binder/IServiceManager.h"

const int RETRY_CONNECT_CNT = 5;
const int CONNECT_SLEEP_SECOND = 1000000;

ServiceProxyBaseImpl::ServiceProxyBaseImpl(const char *name, ServiceProxyBase *interface)
    : m_if(interface)
    , m_bp(NULL)
    , m_serviceLock()
    , m_name(name)
{
    BSLOGD("ServiceProxyBaseImpl::ServiceProxyBaseImpl %s", name);
}

ServiceProxyBaseImpl::~ServiceProxyBaseImpl()
{
    BSLOGD("ServiceProxyBaseImpl::~ServiceProxyBaseImpl %s", m_name.string());
    android::Mutex::Autolock _l(m_serviceLock);
    if (m_bp != NULL) {
        BSLOGD("ServiceProxyBaseImpl::~ServiceProxyBaseImpl unlink to death");
        m_bp->unlinkToDeath(this);
        m_bp = NULL;
    }
    m_if = NULL;
}

void ServiceProxyBaseImpl::binderDied(const android::wp<android::IBinder> &who)
{
    UNUSED(who);
    BSLOGD("ServiceProxyBaseImpl::binderDied");
    if(m_if) {
        m_if->onDisconnected();
    }
    android::Mutex::Autolock _l(m_serviceLock);
    m_bp = NULL;
}

android::String8  &ServiceProxyBaseImpl::name()
{
    return m_name;
}

bool ServiceProxyBaseImpl::tryConnect()
{
    BSLOGD("ServiceProxyBaseImpl::tryConnect");
    android::Mutex::Autolock _l(m_serviceLock);
    if (m_bp.get() == NULL) {
        android::sp<android::IServiceManager> sm = android::defaultServiceManager();
        android::sp<android::IBinder> binder;
        int retry = RETRY_CONNECT_CNT;
        do {
            binder = sm->getService(android::String16(m_name.string()));
            BSLOGD("ServiceProxyBaseImpl::tryConnect get service %x",binder.get());
            if (binder != NULL) {
                break;
            }
            usleep(CONNECT_SLEEP_SECOND); // 1 s
        } while((--retry) > 0);
        if(binder != NULL) {
            m_bp = new ServiceBinderProxy(binder);
            if(m_bp != NULL) {
                BSLOGD("ServiceProxyBaseImpl::tryConnect link to death");
                int ret = m_bp->linkToDeath(this);
                BSLOGD("ServiceProxyBaseImpl::tryConnect link to death return %d", ret);
            }
            if(m_if) {
                m_if->onConnected();
                BSLOGD("ServiceProxyBaseImpl::tryConnect connected");
            }
            return true;
        }
        else {
            BSLOGW("ServiceProxyBase::tryConnect failed");
            return false;
        }
    }

    return true;
}

int ServiceProxyBaseImpl::sendRequest(unsigned int code, const android::Parcel &data, android::Parcel *reply, unsigned int flags)
{
    BSLOGD("ServiceProxyBaseImpl::sendRequest code is %d", code);
    android::Mutex::Autolock _l(m_serviceLock);
    if(m_bp.get()) {
        return m_bp->Transact(code, data, reply, flags);
    }
    BSLOGW("ServiceProxyBase::sendRequest maybe not connected");
    m_bp = NULL;
    return BS_NO_CONNECTION;
}
