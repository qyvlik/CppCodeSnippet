#include <iostream>
#include <string>

using namespace std;

#define FSM
#define STATE(x) s_##x :
#define NEXTSTATE(x) goto s_##x

int main()
{
    FSM {
        int x = 0;
        STATE(a) {
            std::cout << "x:" << x << std::endl;
            NEXTSTATE(b);
        }

        STATE(b) {
            if(x == 0) {
                x ++;
                NEXTSTATE(a);
            } else {
                NEXTSTATE(end);
            }
        }

        STATE(end) {
            std::cout << "end" << std::endl;
        }
    }
    return 0;
}

