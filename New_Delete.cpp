#include <malloc.h>
void f1();
void f1() {
    /*
        在堆上开辟内存
        C.malloc
        CPP.new
    */
    int* c_ptr_1 = (int*)malloc(sizeof(int));
    if (c_ptr_1 == nullptr) {
        return;
    }
    *c_ptr_1 = 20;
    free(c_ptr_1);

    int* c_ptr_2 = (int*)malloc(sizeof(int) * 20);
    //...
    free(c_ptr_2);

    //CPP
    int* p1 = new int(20);
    delete p1;

    int* p2 = new int[20]();
    delete[] p2;
}