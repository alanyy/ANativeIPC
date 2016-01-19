#include "ServiceBase.h"
#include "Log.h"
#include "ServiceBaseImpl.h"
#include "ServiceBaseDefines.h"

ServiceBase::ServiceBase(const char *name)
    : m_impl(NULL)
{
    BSLOGD("ServiceBase::ServiceBase %s", name);
    m_impl = new ServiceBaseImpl(name, this);
}

ServiceBase::~ServiceBase()
{
    BSLOGD("ServiceBase::~ServiceBase %s", name().string());
}

android::String8 ServiceBase::name()
{
    if(m_impl != NULL) {
        return m_impl->name();
    }
    return android::String8();
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

int ServiceBase::onRequest(unsigned int code, const android::Parcel &data, android::Parcel *reply, unsigned int flags)
{
    UNUSED(code);
    UNUSED(data);
    UNUSED(reply);
    UNUSED(flags);
    return BS_NO_CONNECTION;
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
