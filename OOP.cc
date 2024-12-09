/*
    继承后的派生类占用内存需要加上基类

    基类的private成员，在public、protected、private三种继承方式中，都不可见，并且外部无法访问
    public继承下，基类成员在派生类中的访问限定不变
    protected继承下，基类的public权限变成protected
    private继承下，基类的所有权限变private，仅限类内访问

    链式继承中，继承后成员的访问限定看 直接基类 中的访问限定

    默认继承方式看派生类的定义
    Class A : B 默认private继承
    Struct A ：B 默认public继承

    不考虑类型强转
    继承结构的转换，对象转换，指针（引用）转换只支持从下到上的转换
    默认上（基类）、下（派生类）
    从内存上看，派生类会占用基类之外的内存，转换后，小内存自然不能转换成大内存
    Base base();
    Derive derive();

    base = derive; OK
    Base* ptr = &derive; OK

*/

/*
    Base()
    Derive()
    ~Derive()
    ~Base()

    初始化列表先执行，再执行构造函数函数体{}
    在派生类中初始化基类的成员，需要在派生类的初始化列表中调用基类的构造函数
    所以先执行Base()->Derive()
    派生类作用域到期后
    先执行~Derive()->~Base()
*/

/*
    静态绑定、动态绑定：绑定也就是函数调用
    编译阶段形成汇编指令 call Base::show() 静态绑定
    call [reg] 动态绑定，当调用virtual function的时候，进行动态绑定

    
    虚函数virtual
    a.一个类定义了virtual func()的时候，编译器会对这个类产生一个vftable
    vftable
    |-----------|
    |&RTTI      |->"Base"(假设Base类定义了virtual function)，&RTTI（run-time type information）存的是常量字符串地址
    |-----------|
    |offset     |
    |-----------|
    |&Base::vtFunc_1()|->Base类中virtual function 1 的地址
    |&Base::vtFunc_2()|...
    ...
    程序运行时，会将vftable表加载到.rodata段（read-only data）

    b.对象中会多存一个地址 &vfptr，指向vftable，可以通过vfptr的偏移来找到virtual function

    c.多个对象的 &vfptr都指向同一张vftable

    d.如果派生类的某一个成员函数的返回类型、函数名、参数列表与基类的某个virtual function相同
    (注：看样子也不需要与某个virtual function相同，只要Base中有virtual，那么派生类就会有vftable)
    那么编译器会给派生类生成一张vftable表，同时将该function->virtual function
    并且会在自己的vftable表覆盖基类的&Base::vtFunc()信息
    所以ptr->show()就会调用vftable表中&Derive::vtFunc()函数

    只要Base有virtual function，那么他就会有vftable表
    并且派生类也会有vftable表，而且派生类有可能共用基类的vfptr
*/
#include <iostream>
class Base {
public:
	Base(int data = 10)
        :_base_a(data) {
        std::cout << "Base()" << std::endl;
    }
    ~Base() {
        std::cout << "~Base()" << std::endl;
    }
    virtual void show() const { std::cout << "Base::show()" << std::endl; }
    virtual void show(int) const { std::cout << "Base::show(int)" << std::endl; }
protected:
    int _base_a;
};

class Derive : public Base {
public:
    Derive(int data)
        :Base(data),
        _derive_a(data) {
        std::cout << "Derive()" << std::endl;
    }
    ~Derive() {
        std::cout << "~Derive()" << std::endl;
    }
    void show() const { std::cout << "Derive::show()" << std::endl; }
private:
    int _derive_a;
};

static void func() {
    Derive derive(20);
    Base base;
    std::cout << "++++++++++++++++++" << std::endl;
    Base* ptr = &derive;

    ptr->show();
    std::cout << "sizeof(Base)" << sizeof(base) << std::endl;
    std::cout << "sizeof(derive)" << sizeof(derive) << std::endl;
    std::cout << "ptr type: " << typeid(ptr).name() << std::endl;
    std::cout << "*ptr type: " << typeid(*ptr).name() << std::endl;

    std::cout << "++++++++++++++++++" << std::endl;
}
