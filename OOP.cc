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

/*
    哪些函数不能实现成虚函数？
    
    虚函数依赖:
    对象内存->vptr->vftable->虚函数地址

    构造函数完成，对象才存在，所以构造函数不能实现成虚函数，并且{}中全是静态绑定，不会发生动态绑定
    构造函数中哪怕调用virtual function()，也是静态绑定
    
    static function()也不能实现成virtual，因为static不依赖对象

    虚析构函数
    什么时候将基类的析构函数实现成 虚函数 virtual ~Base(){}
    Base* ptr = new Derive();的时候，也就是基类指针指向 堆 上的派生类
    这样delete ptr的时候，由于Base类有虚函数->Base::vftable->Derive::vftable->自动virtual ~Derive()-> *ptr == class Derive
    -> delete ptr会先找到~Derive() -> 避免内存泄露

    Base b;
    Derive* ptr = (Derive*)&b;
    ptr->show();
    动态绑定，并且b内存上只有Base对象，因此调用的是Base::show()

*/

/*
    多态

    静态多态
    编译时的多态：函数重载（在同一个作用域内）、模板，发生的是静态绑定

    动态多态
    通过基类指针(引用)指向派生类，并且调用相应的virtual function，就会发生动态绑定
    ，找到派生类的vfptr，接着找到vftable，调用派生类覆盖的virtual function
*/

/*
    Base类中设计成virtual function() = 0;就是抽象类，一般将Base类设计成抽象类
    抽象类不能实例化，但可以定义指针（引用），用于动态多态
*/

/*
    Base:
        virtual show(int i = 10)
    Derive:
        virtual show(int i = 20)
    当基类指针（引用）指向派生类时，编译阶段编译器只会压入Base类的show()形参列表，因此只能访问到Base类的默认值，调用的依然是派生类的show
*/

/*
    区分编译时的访问权限 和 运行时的访问权限
    Base* ptr = new Derive();
    ptr->show();
    编译阶段：
        编译器看ptr是Base*类型，public: Base::show()可以通过编译，运行阶段是动态绑定，调用的其实是Derive::show()，此时
        private Derive::show()也可以调用，因为是用地址访问
        private: Base::show()不可以通过编译，更别提运行了
    
*/
/*
    什么时候执行vfptr <- &vftable ?
    构造函数执行前，开辟stack frame后执行
*/
#include <iostream>
#include <string>
class Animal {
public:
    Animal(const std::string& name)
        :_name(name)
    {}
    virtual void bark() const = 0; //此时Animal是抽象类
protected:
    std::string _name;
};

class Cat : public Animal {
public:
    Cat(const std::string& name)
        :Animal(name)
    {}
    void bark() const { std::cout << _name << " miao miao miao!" << std::endl; }
};
class Dog : public Animal {
public:
    Dog(const std::string& name)
        :Animal(name)
    {}
    void bark() const { std::cout << _name << " wang wang wang!" << std::endl; }
};

/*
    利用多态
    基类指针（引用）指向派生类，调用virtual function的时候，进行动态绑定，找到animal的vfptr-> vftable -> 调用派生类的覆盖方法
*/
static void bark(const Animal& animal) { animal.bark(); }

static void test() {
    Cat cat("猫咪");
    Dog dog("二哈");
    bark(cat);  // 猫咪 miao miao miao!
    bark(dog);  // 二哈 wang wang wang!
    std::cout << "---------pointer----------" << std::endl;
    
    /*
        通过交换cat和dog的vfptr，导致vftable是对方的，因此动态绑定后，调用的vftable中的virtual bark()其实是对方的
    */
    Animal* ptr1 = &cat;
    Animal* ptr2 = &dog;

    int* pp1 = (int*)ptr1;
    int* pp2 = (int*)ptr2;
    int tmp = pp1[0];
    pp1[0] = pp2[0];
    pp2[0] = tmp;

    ptr1->bark();
    ptr2->bark();

}

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
