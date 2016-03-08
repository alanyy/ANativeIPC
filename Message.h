#ifndef MESSAGE_H
#define MESSAGE_H

#include "utils/RefBase.h"
#include "utils/String8.h"
#include "ServiceBaseGlobal.h"

class SERVICEBASE_API Message : public android::RefBase
{
public:
    enum Type{
        TypeNull,
        TypeInt,
        TypeUInt,
        TypeDouble,
        TypeFloat,
        TypePointer,
        TypeString
    };

    Message(int what = 0);

    int what();
    void setWhat(int what);

    void clear();

    int setInt(int value);
    int setUInt(unsigned int value);
    int setDouble(double value);
    int setFloat(float value);
    int setPointer(void *value);
    int setString(const android::String8 &value);

    bool getIntValue(int index, int *value);
    bool getUIntValue(int index, unsigned int *value);
    bool getDoubleValue(int index, double *value);
    bool getFloatValue(int index, float *value);
    bool getPointerValue(int index, void **value);
    bool getStringValue(int index, android::String8 *value);

    int valueCount();
    bool getItemType(int index, Type *type);
    virtual ~Message();
private:
    DISABLE_COPY(Message)
    struct Item {
        union {
            int intValue;
            unsigned int uIntValue;
            double doubleValue;
            float floatValue;
            void *pointerValue;
            android::String8 *stringValue;
        } u;
        Type type;
    };

    enum {
        MaxNumItems = 32
    };

    Item *allocateItem();
    void freeItem(Item *item);
    Item *findItem(int index);
    bool setItem(int index, Item &item);

    int m_what;
    Item m_items[MaxNumItems];
    int m_itemNum;
};

#endif // MESSAGE_H
