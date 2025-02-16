/*
    class在编译的时候，成员方法会在前面自动加上 *this
*/
/*
    构造和析构
    成员变量初始化顺序跟定义的顺序有关，跟参数构造列表出现顺序无关
    如果对象在堆上（new一个出来），作用域结束是不会自动析构+释放内存的，需要手动delete
    栈上的对象离开作用域会自动调用析构函数
    全局则是程序结束才调用
*/
/*
    静态成员变量，是类级别的，不是对象级别
    static关键字修饰，类内声明，类外定义并初始化
    普通成员方法是有一个 class *this指针，而静态成员方法没有该指针

    常成员方法 常对象可以调用，建议只读不写的方法都加上const
    type func() const {} -> const Class *this

    成员方法, 常对象不能调用
    type func() {}
*/
#include <iostream>
class Test {
public:
    Test() {
        std::cout << "TEST() FUNC CALLING" << std::endl;
    }
    ~Test() {
        std::cout << "~TEST() FUNC CALLING" << std::endl;
    }
    void func() { std::cout << "Test::func()" << std::endl; }
    static void static_func() { std::cout << "Test::static_fucn()" << std::endl; }
    int _data;
    static int _staticData;
};

int Test::_staticData = 0;
int f1() {

    Test t1;
    int Test::* ptr = &Test::_data;
    t1.*ptr = 30;

    int* staticPtr = &Test::_staticData;
    *staticPtr = 30;

    void (Test:: * pfunc)() = &Test::func;
    (t1.*pfunc)();

    void(*staticFunc)() = &Test::static_func;
    (*staticFunc)();

    std::cout << t1._data << std::endl << Test::_staticData << std::endl;
    return 0;
}
//void oop_f1();
//void oop_f2();
//#include <iostream>
//int oop() {
//    oop_f2();
//    return 0;
//}
//
//void oop_f1() {
//    Stack s(5);
//
//    for (int i = 0; i < 15; i++) {
//        s.push(rand() % 100);
//    }
//    while (!s.empty()) {
//        std::cout << s.top() << std::endl;
//        s.pop();
//    }
//}
//
//void oop_f2() {
//    /*
//        深拷贝、浅拷贝
//        编译器默认的拷贝构造是内存的数据拷贝
//        **当对象成员中含有指针**，并且指向外部资源的时候，浅拷贝会让两个对象指向同一处外部资源
//        当析构发生时，内层析构执行没问题，外层析构执行时，由于外部资源已经被内层析构释放，导致外层析构指向 已经被释放的资源。就出现问题
//        
//        避免class内的cp使用memcpy、realloc等内存拷贝函数，当class内某个成员变量是一个数组，并且这个数组存放的是外部资源的时候，那么调用内存拷贝就会core down
//        用for
//
//
//    */
//    Stack s1(10);
//    for (int i = 0; i < 15; i++) {
//        s1.push(rand() % 100);
//    }
//    //两句拷贝构造等效，没有重载深拷贝的情况下，调用默认拷贝构造，如果重载了深拷贝，调用深拷贝构造
//    Stack s2(s1);   //等价于 Stack s2 = s1;
//    //s2 = s1 core down，因为 = 没有重载
//    s2 = s1;
//}