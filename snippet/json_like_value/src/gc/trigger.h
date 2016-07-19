#ifndef QYVLIK_TRIGGER_TRIGGER_H
#define QYVLIK_TRIGGER_TRIGGER_H

#include "../declara_forword.hpp"

namespace qyvlik {
namespace trigger {

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

    void setHandle(const Handle& handle) {
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

    void trigger();

    virtual domain::Arguments arguments() const = 0;

protected:
    void trigger(ConnectionMap::iterator iter, ConnectionMap::iterator end);
    ConnectionManager* mConnectionManager;
};

class OnDestroy : public OnHappened
{
public:
    friend class ConnectionManager;
public:
    OnDestroy(ConnectionManager* connectionManager);

    int type() const override;

    void setArguments(const domain::Arguments& arguments);

    domain::Arguments arguments() const override;
private:
     domain::Arguments mArguments;
};

class ConnectionManager
{
    friend class OnHappened;
public:
    explicit ConnectionManager();

    void whenHappened(OnHappened* happended);

    void connect(OnHappened* happen, OnHappenedHandle* handle);

    void disconnect(OnHappened* happen, OnHappenedHandle* handle);

    void printConnections() const;

protected:
    void disconnect_helper();

protected:
    ConnectionMap mConnections;
};

class ValueHandle
{
public:
    ValueHandle(ConnectionManager* cm);

    ValueHandle(ConnectionManager* cm, domain::Value* obj);

    ~ValueHandle();

    void set(domain::Value* object);

    domain::Value* get() const;

    domain::Value* operator ->() const throw(std::string);

private:
    ConnectionManager* mConnectionManager;
    domain::Value* mValue;
    OnHandle* mHandle;
};

} // namespace trigger
} // namespace qyvlik

#endif // QYVLIK_TRIGGER_TRIGGER_H
