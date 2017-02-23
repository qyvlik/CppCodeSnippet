#include <iostream>

#include "createobjectfromname.hpp"

#include <thread>

class PodObject {
public:
    void print()const{
        std::cout << this << " " << a << std::endl;
    }
    int a = 0;
};

//class Task
//{
//public:
//    static std::thread task(const std::function<void(void)>& run, const std::function<void(void)>& finished ) {
//        return std::thread {
//            [=](){
//                run();
//                finished();
//            }
//        };
//    }
//};

int main()
{
        qyvlik::CreateObjectFromName podRefer;
        podRefer.registerPodType("PodObject", std::shared_ptr<qyvlik::ClassCreatorInterface>(new qyvlik::ClassCreator<PodObject>()));


        void* pod1 = podRefer.create("PodObject");
        if(pod1 != nullptr) {
            ((PodObject*)pod1)->a = 10;
            ((PodObject*)pod1)->print();
            delete ((PodObject*)pod1);
        }
        std::shared_ptr<PodObject> pod2 = podRefer.create<PodObject>("PodObject");
        pod2->print();
}
