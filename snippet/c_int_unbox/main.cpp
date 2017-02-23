#include <QCoreApplication>
#include <iostream>

namespace qyvlik {

enum type { pair, string, vector, };

typedef struct Value* SCM;

struct Value {
    enum type type;
    union {
        struct {
            SCM car, cdr;
        } pair ;
        struct {
            int length;
            char* elts;
        } string;
        struct {
            int length;
            SCM* elts;
        } vector;
    } value;
};

#define POINTER_P(x) (((int)(x)&7) == 0)

#define INTERGET_P(x) (!POINTER_P(x))

#define GET_INTEGER(x) ((int)(x) >> 3)

#define MAKE_INTEGER(x) ((SCM)(((x) << 3) | 1))

}


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);



    return a.exec();
}

