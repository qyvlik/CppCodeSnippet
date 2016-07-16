#include <iostream>

#include "src/trigger.h"

using namespace qyvlik;
using namespace qyvlik::trigger;

class OnDestroy : public OnHappened
{
public:
    OnDestroy(ConnectionManager* connectionManager):
        OnHappened(connectionManager)
    { }

    int type() const override
    {
        return 0;
    }
};

class Object
{
public:
    Object(ConnectionManager* manager):
        destroy(new OnDestroy(manager))
    {}

    ~Object()
    {
        destroy->triger();
        delete destroy;
    }

    void print() const {
        std::cout << "print ~" << std::endl;
    }

    OnDestroy* destroy;
};

class ObjectHandle
{
public:
    ObjectHandle(ConnectionManager* cm):
        mConnectionManager(cm),
        mObject(nullptr)
    {
        mHandle = new OnHandle(
                    [this](OnHappened *){
                        this->mConnectionManager->disconnect(mObject->destroy, mHandle);
                        this->mObject = nullptr;
                    });
    }

    ObjectHandle(ConnectionManager* cm, Object* obj):
        mConnectionManager(cm),
        mObject(obj)
    {
        mHandle = new OnHandle(
                    [this](OnHappened *){
                        this->mConnectionManager->disconnect(mObject->destroy, mHandle);
                        this->mObject = nullptr;
                    });
        cm->connect(mObject->destroy, mHandle);
    }

    ~ObjectHandle()
    {
        delete mHandle;
    }

    void set(Object* object) {
        if(mObject)
            this->mConnectionManager->disconnect(mObject->destroy, mHandle);
        mObject = object;
        mConnectionManager->connect(object->destroy, mHandle);
    }

    Object* get() const {
        return mObject;
    }

    Object* operator ->() const throw(std::string) {
        if(mObject == nullptr) throw std::string("Null Pointer!");
        return mObject;
    }

private:
    ConnectionManager* mConnectionManager;
    Object* mObject;
    OnHandle* mHandle;
};

int main()
{
    ConnectionManager cManager;
    ObjectHandle objecthanled(&cManager);

    {
        Object obj(&cManager);
        objecthanled.set(&obj);

        OnHandle* handle = new OnHandle (
                    [&](OnHappened *happened) {
                        if(happened->type() == 0) std::cout << "["<< happened << "] destroy"  << std::endl;

                        cManager.disconnect(obj.destroy, handle);
                        delete handle;
                    });

        cManager.connect(obj.destroy, handle);

        objecthanled->print();

        cManager.printConnections();
    }

    cManager.printConnections();

    try {
        objecthanled->print();
    } catch(std::string e) {
        std::cerr << e << std::endl;
    }


    return 0;
}

