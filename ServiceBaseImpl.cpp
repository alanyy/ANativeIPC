#include "ServiceBaseImpl.h"
#include "binder/ProcessState.h"
#include "binder/IServiceManager.h"
#include "binder/IPCThreadState.h"
#include "ServiceBase.h"
#include "ServiceBaseDefines.h"
#include "Log.h"

ServiceBaseImpl::ServiceBaseImpl(const char *name, ServiceBase *interface)
    : BBinder()
    , m_name(name)
    , m_if(interface)
{
}

ServiceBaseImpl::~ServiceBaseImpl()
{
    m_if = NULL;
}

std::string &ServiceBaseImpl::name()
{
    return m_name;
}

void ServiceBaseImpl::initialize()
{
    m_proc = android::ProcessState::self();
    m_svcMng = android::defaultServiceManager();
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
    if(m_if) {
        int ret = m_if->onRequest(code, data, reply, flags);
        BSLOGD("ServiceBaseImpl::onTransact service onRequest return %d", ret);
        if(ret == BS_NO_ERROR) {
            return android::NO_ERROR;
        }
    }
    return BBinder::onTransact(code, data, reply, flags);
}

int ServiceBaseImpl::addService()
{
    BSLOGD("ServiceBaseImpl::addService %s", m_name.c_str());
    return m_svcMng->addService(android::String16(m_name.c_str()), this);
}

int ServiceBaseImpl::removeService()
{
    //need to add remove service action in binder and service manager
    return 0;
    //return m_svcMng->removeService(m_name.c_str());
}
