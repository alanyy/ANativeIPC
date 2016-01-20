#include "ServiceProxyBaseImpl.h"
#include "Log.h"
#include "ServiceBaseDefines.h"
#include "ServiceProxyBase.h"
#include "ServiceBinderProxy.h"
#include "AnonymousBinder.h"
#include <unistd.h>
#include "binder/IServiceManager.h"
#include "binder/ProcessState.h"

const int RETRY_CONNECT_CNT = 5;
const int CONNECT_SLEEP_SECOND = 1000000;

ServiceProxyBaseImpl::ServiceProxyBaseImpl(const char *name, ServiceProxyBase *interface)
    : m_if(interface)
    , m_bp(NULL)
    , m_asyncBinder(NULL)
    , m_serviceLock()
    , m_asyncLock()
    , m_name(name)
{
    BSLOGD("ServiceProxyBaseImpl::ServiceProxyBaseImpl %s", name);
}

ServiceProxyBaseImpl::~ServiceProxyBaseImpl()
{
    BSLOGD("ServiceProxyBaseImpl::~ServiceProxyBaseImpl %s", m_name.c_str());
    m_serviceLock.lock();
    if (m_bp != NULL) {
        BSLOGD("ServiceProxyBaseImpl::~ServiceProxyBaseImpl unlink to death");
        m_bp->unlinkToDeath(this);
        m_bp = NULL;
    }
    m_serviceLock.unlock();
    m_asyncLock.lock();
    if(m_asyncBinder != NULL) {
        m_asyncBinder = NULL;
    }
    m_asyncLock.unlock();
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

std::string &ServiceProxyBaseImpl::name()
{
    return m_name;
}

bool ServiceProxyBaseImpl::tryConnect()
{
    BSLOGD("ServiceProxyBaseImpl::tryConnect");
    android::Mutex::Autolock _l(m_serviceLock);
    if (m_bp == NULL) {
        android::sp<android::IServiceManager> sm = android::defaultServiceManager();
        android::sp<android::IBinder> binder;
        int retry = RETRY_CONNECT_CNT;
        do {
            binder = sm->getService(android::String16(m_name.c_str()));
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

bool ServiceProxyBaseImpl::setupAsyncRequest()
{
    android::Mutex::Autolock _l(m_asyncLock);
    if(m_asyncBinder == NULL) {
        m_asyncBinder = new AnonymousBinder(m_if);
        if(m_asyncBinder != NULL) {
            android::ProcessState::self()->startThreadPool();
            android::Parcel data;
            android::Parcel reply;
            data.writeStrongBinder(m_asyncBinder);
            sendSyncRequest(ADD_ASYNC_BINDER, data, &reply);
            if(reply.errorCheck() == BS_NO_ERROR){
                SenderId remoteId = reply.readInt32();
                BSLOGD("ServiceProxyBaseImpl::setupAsyncRequest remoteId is %d",remoteId);
                m_asyncBinder->setRemoteId(remoteId);
                return true;
            }
            else {
                BSLOGW("ServiceProxyBaseImpl::setupAsyncRequest reply is failed");
            }
        }
        return false;
    }
    return true;
}

bool ServiceProxyBaseImpl::teardownAsyncRequest()
{
    android::Mutex::Autolock _l(m_asyncLock);
    if(m_asyncBinder != NULL) {
        android::Parcel data;
        android::Parcel reply;
        data.writeInt32(m_asyncBinder->remoteId());
        sendSyncRequest(REMOVE_ASYNC_BINDER, data, &reply);
        if(reply.readInt32() == BS_NO_ERROR){
            return true;
        }
        BSLOGW("ServiceProxyBaseImpl::teardownAsyncRequest reply is failed");
        return false;
    }
    return true;
}

int ServiceProxyBaseImpl::sendSyncRequest(unsigned int code, const android::Parcel &data, android::Parcel *reply)
{
    BSLOGD("ServiceProxyBaseImpl::sendSyncRequest code is %d", code);
    android::Mutex::Autolock _l(m_serviceLock);
    if(m_bp.get()) {
        return m_bp->Transact(code, data, reply);
    }
    BSLOGW("ServiceProxyBase::sendSyncRequest maybe not connected");
    m_bp = NULL;
    return BS_NO_CONNECTION;
}

bool ServiceProxyBaseImpl::prepareAsyncData(android::Parcel &data)
{
    android::Mutex::Autolock _l(m_asyncLock);
    if(m_asyncBinder != NULL) {
        data.writeInt32(m_asyncBinder->remoteId());
        return true;
    }
    return false;
}

int ServiceProxyBaseImpl::sendAsyncRequest(unsigned int code, const android::Parcel &data)
{
    BSLOGD("ServiceProxyBaseImpl::sendAsyncRequest code is %d", code);
    android::Mutex::Autolock _l(m_serviceLock);
    if(m_bp.get()) {
        return m_bp->Transact(code, data, NULL, ASYN_CALL);
    }
    BSLOGW("ServiceProxyBase::sendAsyncRequest maybe not connected");
    m_bp = NULL;
    return BS_NO_CONNECTION;
}
