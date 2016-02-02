#include "Message.h"
#include "Log.h"
#include "ServiceBaseGlobal.h"

Message::Message(int what)
    : m_what(what)
    , m_itemNum(0)
{
    BSLOGD("Message::Message %d", what);
    memset(m_items, 0, sizeof(m_items));
}

int Message::what()
{
    return m_what;
}

void Message::setWhat(int what)
{
    m_what = what;
}

void Message::clear()
{
    BSASSERT(m_itemNum >= 0, "Message::clear");
    for(int i = 0; i < m_itemNum; ++i) {
        Item *item = &(m_items[i]);
        freeItem(item);
    }
    m_itemNum = 0;
}

int Message::setInt(int value)
{
    Item *item = allocateItem();
    if(item != NULL) {
        item->u.intValue = value;
        item->type = TypeInt;
        return (m_itemNum - 1);
    }
    return -1;
}

int Message::setUInt(unsigned int value)
{
    Item *item = allocateItem();
    if(item != NULL) {
        item->u.uIntValue = value;
        item->type = TypeUInt;
        return (m_itemNum - 1);
    }
    return -1;
}

int Message::setDouble(double value)
{
    Item *item = allocateItem();
    if(item != NULL) {
        item->u.doubleValue = value;
        item->type = TypeDouble;
        return (m_itemNum - 1);
    }
    return -1;
}

int Message::setFloat(float value)
{
    Item *item = allocateItem();
    if(item != NULL) {
        item->u.floatValue = value;
        item->type = TypeFloat;
        return (m_itemNum - 1);
    }
    return -1;
}

int Message::setPointer(void *value)
{
    Item *item = allocateItem();
    if(item != NULL) {
        item->u.pointerValue = value;
        item->type = TypePointer;
        return (m_itemNum - 1);
    }
    return -1;
}

int Message::setString(const std::string &value)
{
    Item *item = allocateItem();
    if(item != NULL) {
        item->u.stringValue = new std::string(value);
        item->type = TypeString;
        return (m_itemNum - 1);
    }
    return -1;
}

bool Message::getIntValue(int index, int *value)
{
    Item *item = findItem(index);
    if(item != NULL) {
        if(item->type == TypeInt) {
            if(value != NULL) {
                *value = item->u.intValue;
            }
            return true;
        }
    }
    return false;
}

bool Message::getUIntValue(int index, unsigned int *value)
{
    Item *item = findItem(index);
    if(item != NULL) {
        if(item->type == TypeUInt) {
            if(value != NULL) {
                *value = item->u.uIntValue;
            }
            return true;
        }
    }
    return false;
}

bool Message::getDoubleValue(int index, double *value)
{
    Item *item = findItem(index);
    if(item != NULL) {
        if(item->type == TypeDouble) {
            if(value != NULL) {
                *value = item->u.doubleValue;
            }
            return true;
        }
    }
    return false;
}

bool Message::getFloatValue(int index, float *value)
{
    Item *item = findItem(index);
    if(item != NULL) {
        if(item->type == TypeFloat) {
            if(value != NULL) {
                *value = item->u.floatValue;
            }
            return true;
        }
    }
    return false;
}

bool Message::getPointerValue(int index, void **value)
{
    Item *item = findItem(index);
    if(item != NULL) {
        if(item->type == TypePointer) {
            if(value != NULL) {
                *value = item->u.pointerValue;
            }
            return true;
        }
    }
    return false;
}

bool Message::getStringValue(int index, std::string *value)
{
    Item *item = findItem(index);
    if(item != NULL) {
        if(item->type == TypeString) {
            if((value != NULL) && (item->u.stringValue != NULL)) {
                *value = *(item->u.stringValue);
            }
            return true;
        }
    }
    return false;
}

int Message::valueCount()
{
    return m_itemNum;
}

bool Message::getItemType(int index, Message::Type *type)
{
    BSASSERT(m_itemNum >= 0, "Message::getItemType");
    if((index >= 0) && (index < m_itemNum)) {
        if(type != NULL) {
            *type = m_items[index].type;
        }
        return true;
    }
    return false;
}


Message::~Message()
{
    BSLOGD("Message::~Message %d", m_what);
    clear();
}

Message::Item *Message::allocateItem()
{
    BSASSERT(m_itemNum >= 0, "Message::allocaItem");
    if(m_itemNum < MaxNumItems) {
        Item * item = &(m_items[m_itemNum]);
        ++m_itemNum;
        return item;
    }
    return NULL;
}

void Message::freeItem(Message::Item *item)
{
    if(item != NULL) {
        if(item->type == TypeString) {
            delete item->u.stringValue;
        }
        memset(item, 0, sizeof(Message::Item));
    }
}

Message::Item *Message::findItem(int index)
{
    BSASSERT(m_itemNum >= 0, "Message::findItem");
    if((index >= 0) && (index < m_itemNum)) {
        return &(m_items[index]);
    }
    return NULL;
}

bool Message::setItem(int index, Message::Item &item)
{
    BSASSERT(m_itemNum >= 0, "Message::setItem");
    if((index >= 0) && (index < m_itemNum)) {
        if(m_items[index].type != TypeNull) {
            freeItem(&(m_items[index]));
        }
        m_items[index] = item;
        return true;
    }
    return false;
}
