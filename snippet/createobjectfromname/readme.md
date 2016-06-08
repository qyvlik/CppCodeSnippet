# 从字符串构建对象

最终使用效果如下：

```
#include <iostream>

#include "createobjectfromname.hpp"


class PodObject {
public:
    void print()const{
        std::cout << this << " " << a << std::endl;
    }
    int a = 0;
};


int main()
{
    qyvlik::CreateObjectFromName podRefer;
    podRefer.registerPodType("PodObject", std::shared_ptr<qyvlik::ClassCreatorInterface>(new qyvlik::ClassCreator<PodObject>()));


    void* pod1 = podRefer.create("PodObject");
    if(pod1 != nullptr) {
        ((PodObject*)pod1)->print();
        delete ((PodObject*)pod1);
    }
    std::shared_ptr<PodObject> pod2 = podRefer.create<PodObject>("PodObject");
    pod2->print();
}
```
