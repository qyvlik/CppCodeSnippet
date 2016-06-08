#ifndef CREATEOBJECTFROMNAME_HPP
#define CREATEOBJECTFROMNAME_HPP

#include <string>
#include <memory>
#include <map>

namespace qyvlik {

class ClassCreatorInterface
{
public:
    virtual ~ClassCreatorInterface()
    {}
    virtual void* create() const = 0;
};


template<typename Class>
class ClassCreator : public ClassCreatorInterface
{
    static_assert(std::is_default_constructible<Class>::value, "Only Support The Object Which Has Default Construction!");
public:
    ClassCreator()
    {}
    virtual void* create() const {
        return (void*) new Class();
    }
};

class CreateObjectFromName
{
public:
    void registerPodType(const std::string& podTypeName, std::shared_ptr<ClassCreatorInterface> podCreator )
    {
        classCreators->insert(std::pair< std::string, std::shared_ptr<ClassCreatorInterface> >(podTypeName, podCreator));
    }

    template<typename Pod>
    std::shared_ptr<Pod> create(const std::string& podType) const
    {
        static_assert(std::is_default_constructible<Pod>::value, "Only Support The Object Which Has Default Construction!");
        auto find = classCreators->find(podType);
        if(find != classCreators->end()) {
            return std::shared_ptr<Pod>((Pod*)(*find).second->create());
        } else {
            return std::shared_ptr<Pod>();
        }
    }

    void* create(const std::string& podType) const
    {
        auto find = classCreators->find(podType);
        if(find != classCreators->end()) {
            //
            return (*find).second->create();
        } else {
            return nullptr;
        }
    }

    CreateObjectFromName():
        classCreators(new std::map< std::string, std::shared_ptr<ClassCreatorInterface> >())
    { }

private:
    bool avoid_fail;
    std::shared_ptr < std::map< std::string, std::shared_ptr<ClassCreatorInterface> > > classCreators;
};

}

#endif // CREATEOBJECTFROMNAME_HPP

