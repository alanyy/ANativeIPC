#include "ServiceBase.h"
#include "Log.h"
#include "ServiceBaseImpl.h"

ServiceBase::ServiceBase(const char *name)
    : m_impl(NULL)
{
    BSLOGD("ServiceBase::ServiceBase %s", name);
    m_impl = new ServiceBaseImpl(name, this);
}

ServiceBase::~ServiceBase()
{
    BSLOGD("ServiceBase::~ServiceBase %s", name().c_str());
}

std::string ServiceBase::name()
{
    if(m_impl != NULL) {
        return m_impl->name();
    }
    return std::string();
}

void ServiceBase::initialize()
{
    if(m_impl != NULL) {
        m_impl->initialize();
    }
}

void ServiceBase::finalize()
{
    if(m_impl != NULL) {
        m_impl->finalize();
    }
}

void ServiceBase::exec()
{
    if(m_impl != NULL) {
        m_impl->exec();
    }
}

int ServiceBase::onSyncRequest(unsigned int code, const android::Parcel &data, android::Parcel *reply)
{
    UNUSED(code);
    UNUSED(data);
    UNUSED(reply);
    return BS_UNKNOWN_TRANSACTION;
}

int ServiceBase::onAsyncRequest(SenderId &id, unsigned int code, const android::Parcel &data)
{
    UNUSED(id);
    UNUSED(code);
    UNUSED(data);
    return BS_UNKNOWN_TRANSACTION;
}

void ServiceBase::onSenderDisconnect(SenderId &id)
{
    UNUSED(id);
}

int ServiceBase::addService()
{
    if(m_impl != NULL) {
        return m_impl->addService();
    }
    return BS_NO_CONNECTION;
}

int ServiceBase::removeService()
{
    if(m_impl != NULL) {
        return m_impl->removeService();
    }
    return BS_NO_CONNECTION;
}

int ServiceBase::sendAsyncResponse(SenderId &id, unsigned int code, const android::Parcel &data)
{
    if(m_impl != NULL) {
        return m_impl->sendAsyncResponse(id, code, data);
    }
    return BS_NO_CONNECTION;
}
