#include <iostream>
//void f1();
//void f2();
//int conference() {
//    f2();
//    return 0;
//}
//void f1() {
//    //定义引用变量 引用数组
//    int array[5] = {};
//    int (&p)[5] = array;
//    std::cout << sizeof(array) << std::endl;
//    std::cout << sizeof(p) << std::endl;
//}
//
//void f2() {
//    /*
//        左值：有内存、有名字、值可以修改的；
//        右值：相反
//    */
//
//    int a = 10; // a是左值
//    int& conf_a = a; //左值引用
//
//    
//    int&& conf_constant_1 = 20; //右值引用
//    const int& conf_constant_2 = 20; //右值引用
//    /*右值引用汇编:
//        00744C0C  mov         dword ptr [ebp-30h],14h
//        00744C13  lea         eax,[ebp-30h]
//        00744C16  mov         dword ptr [conf_constant_1],eax
//        在栈帧开辟一段内存，写入20，再将这段内存的地址加载到eax中，最后将这个地址cp到引用变量conf_constant_1指向的位置
//    */
//}