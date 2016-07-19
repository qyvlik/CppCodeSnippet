#include "trigger.h"

#include <iostream>

#include "../domain/value.h"

namespace qyvlik {
namespace trigger {

OnHappened::OnHappened(ConnectionManager *connectionManager):
    mConnectionManager(connectionManager)
{ }

OnHappened::~OnHappened()
{ }

void OnHappened::trigger()
{
    mConnectionManager->whenHappened(this);
    mConnectionManager->disconnect_helper();
}

void OnHappened::trigger(ConnectionMap::iterator iter, ConnectionMap::iterator end)
{
    while(iter != end) {

        // 这里是 first 字段是为了防止在执行处理器的时候，断开发生器和处理器链接，导致的迭代器失效。
        if(iter->second.first) {
            iter->second.second->handle(iter->first);
        }

        iter++;
    }
}


ConnectionManager::ConnectionManager()
{}

void ConnectionManager::whenHappened(OnHappened *happended)
{
    auto find = mConnections.equal_range(happended);
    happended->trigger(find.first, find.second);
}

void ConnectionManager::connect(OnHappened *happen, OnHappenedHandle *handle)
{
    mConnections.insert(std::pair<OnHappened*, std::pair<bool, OnHappenedHandle*> >(happen, std::pair<bool, OnHappenedHandle*>(true, handle)));
}

void ConnectionManager::disconnect(OnHappened *happen, OnHappenedHandle *handle)
{
    auto range = mConnections.equal_range(happen);
    while(range.first != range.second) {
        if(range.first->second.second == handle) {
            range.first->second.first = false;
        }
        range.first++;
    }
}

void ConnectionManager::printConnections() const
{
    auto iter = mConnections.begin();
    auto end  = mConnections.end();
    while(iter != end) {
        std::cout << "Happen : [" << iter->first << "] -> "
                  << "Handle : [" << iter->second.second << "] "
                  << std::endl;
        iter++;
    }
}

void ConnectionManager::disconnect_helper()
{
    auto iter = mConnections.begin();
    auto end  = mConnections.end();
    while(iter != end) {

        if(!iter->second.first) {
            iter = mConnections.erase(iter);
            continue;
        }

        iter++;
    }
}

//-------------------------------------------------------------------------------------

OnDestroy::OnDestroy(ConnectionManager *connectionManager):
    OnHappened(connectionManager)
{}

int OnDestroy::type() const
{
    return 0;
}

void OnDestroy::setArguments(const domain::Arguments &arguments) {
    mArguments = arguments;
}

domain::Arguments OnDestroy::arguments() const
{
    return mArguments;
}

//-------------------------------------------------------------------------------------

ValueHandle::ValueHandle(ConnectionManager *cm):
    mConnectionManager(cm),
    mValue(nullptr)
{
    mHandle = new OnHandle(
                [this](OnHappened *){
                    this->mConnectionManager->disconnect(mValue->onDestroy(), mHandle);
                    this->mValue = nullptr;
            });
}

ValueHandle::ValueHandle(ConnectionManager *cm, domain::Value *obj):
    mConnectionManager(cm),
    mValue(obj)
{
    mHandle = new OnHandle(
                [this](OnHappened *){
                    this->mConnectionManager->disconnect(mValue->onDestroy(), mHandle);
                    this->mValue = nullptr;
            });
    cm->connect(mValue->onDestroy(), mHandle);
}

ValueHandle::~ValueHandle()
{
    delete mHandle;
}

void ValueHandle::set(domain::Value *object) {
    if(mValue)
        this->mConnectionManager->disconnect(mValue->onDestroy(), mHandle);
    mValue = object;
    mConnectionManager->connect(object->onDestroy(), mHandle);
}

domain::Value *ValueHandle::get() const {
    return mValue;
}

domain::Value *ValueHandle::operator ->() const throw(std::string) {
    if(mValue == nullptr) throw std::string("Null Pointer!");
    return mValue;
}


} // namespace tigger
} // namespace qyvlik
