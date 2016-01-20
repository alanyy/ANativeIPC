#include "ServiceBaseImpl.h"
#include "binder/ProcessState.h"
#include "binder/IServiceManager.h"
#include "binder/IPCThreadState.h"
#include "binder/BpBinder.h"
#include "ServiceBase.h"
#include "Log.h"

ServiceBaseImpl::ServiceBaseImpl(const char *name, ServiceBase *interface)
    : BBinder()
    , m_name(name)
    , m_if(interface)
    , m_clientBinderProxy(NULL)
{
    m_clientBinderProxy = new ClientBinderProxy(this);
}

ServiceBaseImpl::~ServiceBaseImpl()
{
    m_if = NULL;
    clearBinders();
}

std::string &ServiceBaseImpl::name()
{
    return m_name;
}

void ServiceBaseImpl::initialize()
{
    int ret = addService();
    BSLOGD("ServiceBaseImpl::initialize add service return %d", ret);
    android::ProcessState::self()->startThreadPool();
}

void ServiceBaseImpl::finalize()
{
}

void ServiceBaseImpl::exec()
{
    android::IPCThreadState::self()->joinThreadPool();
}

android::status_t ServiceBaseImpl::onTransact(uint32_t code, const android::Parcel &data, android::Parcel *reply, uint32_t flags)
{
    switch(code) {
    case ADD_ASYNC_BINDER:
    {
        BSASSERT((flags & ASYN_CALL) == 0, "ServiceBaseImpl::onTransact add binder");
        android::sp<android::IBinder> binder = data.readStrongBinder();
        BSLOGD("ServiceBaseImpl::onTransact add async binder is %x", binder.get());
        android::BpBinder *binderproxy = binder->remoteBinder();
        if(binderproxy != NULL) {
            addBinder(binderproxy);
            int32_t id = binderproxy->handle();
            BSLOGD("ServiceBaseImpl::onTransact id is %d", id);
            reply->writeInt32(id);
        }
        else {
            reply->setError(BS_NO_CONNECTION);
        }
        return android::NO_ERROR;
    }
    case REMOVE_ASYNC_BINDER:
    {
        BSASSERT((flags & ASYN_CALL) == 0, "ServiceBaseImpl::onTransact remove binder");
        int handle = data.readInt32();
        if(removeBinder(handle)) {
            reply->writeInt32(BS_NO_ERROR);
        }
        else {
            reply->writeInt32(BS_ALREADY_EXISTS);
        }
        return android::NO_ERROR;
    }
    default:
    {
        if(m_if) {
            int ret = BS_NO_ERROR;
            if(flags & ASYN_CALL) {
                SenderId id = data.readInt32();
                ret = m_if->onAsyncRequest(id, code, data);
                BSLOGD("ServiceBaseImpl::onTransact service onAsyncRequest return %d", ret);
            }
            else {
                ret = m_if->onSyncRequest(code, data, reply);
                BSLOGD("ServiceBaseImpl::onTransact service onRequest return %d", ret);
            }
            if(ret == BS_NO_ERROR) {
                return android::NO_ERROR;
            }
        }
        return BBinder::onTransact(code, data, reply, flags);
    }
    }
}

int ServiceBaseImpl::addService()
{
    BSLOGD("ServiceBaseImpl::addService %s", m_name.c_str());
    android::sp<android::IServiceManager> sm = android::defaultServiceManager();
    if(sm != NULL) {
        return sm->addService(android::String16(m_name.c_str()), this);
    }
    return BS_NO_CONNECTION;
}

int ServiceBaseImpl::removeService()
{
    //need to add remove service action in binder and service manager
    return BS_INVALID_OPERATION;
    //return sm->removeService(m_name.c_str());
}

int ServiceBaseImpl::sendAsyncResponse(SenderId &id, unsigned int code, const android::Parcel &data)
{
    android::sp<android::BpBinder> binder = lookupBinder(id);
    if(binder != NULL) {
        return binder->transact(code, data, NULL, ASYN_CALL);
    }
    return BS_NO_CONNECTION;
}

android::sp<android::BpBinder> ServiceBaseImpl::lookupBinder(int handle)
{
    android::Mutex::Autolock _l(m_clientLock);
    android::Vector<android::sp<android::BpBinder> >::iterator it;
    for(it = m_binders.begin();it != m_binders.end(); ++it) {
        if(((*it) != NULL) && ((*it)->handle() == handle)) {
            return (*it);
        }
    }
    return android::sp<android::BpBinder>();
}

bool ServiceBaseImpl::removeBinder(int handle)
{
    android::Mutex::Autolock _l(m_clientLock);
    android::Vector<android::sp<android::BpBinder> >::iterator it;
    for(it = m_binders.begin();it != m_binders.end(); ++it) {
        if(((*it) != NULL) && ((*it)->handle() == handle)) {
            (*it)->unlinkToDeath(m_clientBinderProxy);
            if(m_if) {
                m_if->onSenderDisconnect(handle);
            }
            m_binders.erase(it);
            return true;
        }
    }
    return false;
}

void ServiceBaseImpl::clearBinders()
{
    android::Mutex::Autolock _l(m_clientLock);
    while(!m_binders.isEmpty()) {
        const android::sp<android::BpBinder> binder = m_binders.top();
        if(binder != NULL) {
            binder->unlinkToDeath(m_clientBinderProxy);
        }
        m_binders.pop();
    }
}

bool ServiceBaseImpl::addBinder(android::sp<android::BpBinder> binder)
{
    android::Mutex::Autolock _l(m_clientLock);
    if(binder != NULL) {
        int handle = binder->handle();
        android::Vector<android::sp<android::BpBinder> >::iterator it;
        for(it = m_binders.begin();it != m_binders.end(); ++it) {
            if(((*it) != NULL) && ((*it)->handle() == handle)) {
                return false;
            }
        }
        BSLOGD("ServiceBaseImpl::addBinder %x, handle %d",binder.get(), handle);
        m_binders.push(binder);
        binder->linkToDeath(m_clientBinderProxy);
        BSLOGD("ServiceBaseImpl::addBinder link to death");
        return true;
    }
    return false;
}

void ServiceBaseImpl::onBinderDied(const android::wp<android::IBinder> &who)
{
    android::Mutex::Autolock _l(m_clientLock);
    android::Vector<android::sp<android::BpBinder> >::iterator it;
    for(it = m_binders.begin();it != m_binders.end(); ++it) {
        if(((*it) != NULL) && ((*it) == who)) {
            SenderId id = (*it)->handle();
            if(m_if) {
                m_if->onSenderDisconnect(id);
            }
            m_binders.erase(it);
        }
    }
}


ServiceBaseImpl::ClientBinderProxy::ClientBinderProxy(ServiceBaseImpl *service)
    : m_service(service)
{
}

ServiceBaseImpl::ClientBinderProxy::~ClientBinderProxy()
{
    m_service = NULL;
}

void ServiceBaseImpl::ClientBinderProxy::binderDied(const android::wp<android::IBinder> &who)
{
    if(m_service) {
        m_service->onBinderDied(who);
    }
}
