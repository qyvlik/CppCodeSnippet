#ifndef QYVLIK_TRIGGER_TRIGGER_H
#define QYVLIK_TRIGGER_TRIGGER_H

#include <map>
#include <functional>

namespace qyvlik {
namespace trigger {

class OnHappenedHandle;
class OnHappened;
class ConnectionManager;

typedef std::multimap<OnHappened*, std::pair<bool, OnHappenedHandle*> > ConnectionMap;

class OnHappenedHandle
{
public:
    explicit OnHappenedHandle()
    {}

    virtual ~OnHappenedHandle()
    {}

    virtual void handle(OnHappened* happened) = 0;
};


class OnHandle : public OnHappenedHandle
{
public:
    typedef std::function<void(OnHappened *happened)> Handle;
    OnHandle(const Handle& handle):
        mHandle(handle)
    {}

    void handle(OnHappened *happened) override
    {
        mHandle(happened);
    }

    void resetHandle(const Handle& handle) {
        mHandle = handle;
    }

private:
    Handle mHandle;
};


class OnHappened
{
    friend class ConnectionManager;
public:
    explicit OnHappened(ConnectionManager* connectionManager);

    virtual ~OnHappened();

    virtual int type() const = 0;

    void triger();

    // virtual void** arguments() const = 0;
    // virtual int    arguemntsCount() const = 0;
    // virtual int*   arguementsType() const = 0;

protected:
    void trigger(ConnectionMap::iterator iter, ConnectionMap::iterator end);

    ConnectionManager* mConnectionManager;
};


class ConnectionManager
{
    friend class OnHappened;
public:
    explicit ConnectionManager()
    {}

    void whenHappened(OnHappened* happended);

    void connect(OnHappened* happen, OnHappenedHandle* handle);

    void disconnect(OnHappened* happen, OnHappenedHandle* handle);

    void printConnections() const;

protected:
    void disconnect_helper();

protected:
    ConnectionMap mConnections;
};


} // namespace trigger
} // namespace qyvlik

#endif // QYVLIK_TRIGGER_TRIGGER_H
