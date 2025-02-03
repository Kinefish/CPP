# 对象的构造、析构以及临时对象和右值引用，引用折叠，move、forward语义

> 在必须返回对象的情境下的优化

## 优化

```
//优化前
String getString(const String str) {
	char* retStr = new char[sizeof(str._str) + 1];
	String tmpStr(retStr);
	return tmpStr;
}
int main() {
	String str1("abcd");
	String str2;
	str2 = getString(str1);		//又调用了拷贝赋值
	return 0;
}
/*
存在的问题
1.每调用一次retStr，都会在堆上new一块内存出来，会造成内存泄露
2.当一个函数返回临时对象时 会在 返回函数 的栈帧上 再 构造一个临时对象
	getString()调用结束后，再析构tmpStr
	回到调用方中，
	如果是
	Stirng str = getStirng(src);
	会再进行拷贝构造，最后才析构栈帧上的临时对象
	因此，cpp编译器会进行优化
	如果是临时对象拷贝构造一个新对象，会被优化成新对象的构造
	String str(src);
3.形参接受没有用引用，会导致压栈的时候又进行了str形参的构造
4.进行对象的接受时，优先使用构造新对象的方式进行接受，而不是赋值
*/
//优化
String getString(const String& str) 
	return String(str._str);
}
int main() {
	String str1("abcd");
	String str2 = getString(str1);
	return 0;
}
```

## 优化的三个原则

-	形参优先用引用传递
-	优先用临时对象返回
-	接收返回对象时，优先用构造方式接受



# 关于右值引用（形参中的右值引用）

-	当一个对象占用外部资源的时候，进行拷贝构造时，原先的方式是，`new`一个新空间，并且`forloop`一个个赋值
-	此时当一个函数是返回临时对象的时候，很容易造成，刚刚`forloop`一个个赋值之后，由于是临时对象，出了作用域随着而来的就是析构
-	其实最好的办法时，将你的外部资源给我就行了
-	问题的关键是，**临时对象进行拷贝构造和拷贝赋值**的时候，由于临时对象的特性，语句执行完就会析构，但是如果此时占用了外部资源，会导致构造和赋值的效率变低

-	`cpp`编译器中，将临时对象当作右值
`const String& str = String("abcd");`，左值引用需要加`const`
`String&& str = String("abcd");`，右值引用可以引用临时对象，`str`是右值引用，但本身是一个左值

## 优化

```
/*
	为String新增右值引用的拷贝构造和拷贝赋值
	目的是直接占用临时对象的外部资源，并将临时对象的ptr置空
*/
//拷贝构造右值引用
String(String&& str) {
	_str = str._str;
	str._str = nullptr;
}
//拷贝赋值右值引用
String& operator=(Stirng&& str) {
	if(this == &str) return *this;
	delete[] _str;

	_str = str._str;
	str._str = nullptr;
	return *this;
}
```

## 传递临时对象，返回临时对象（拷贝临时对象）

-	当临时对象作为形参进行传递的时候，编译器匹配的是右值引用作为形参的函数，会先进行临时对象的构造
-	当返回临时对象给新对象进行赋值的时候，编译器会进行优化，临时对象直接在新对象的内存上进行**普通构造**
-	旧版本的编译器可能会多一个返回对象的构造和析构，新编译器应该是优化掉了

## 传递临时对象

- 可以使用`templete<typename _Ty> func(_Ty&& src) { ... std::forward<_Ty>(src)}`，进行左值引用和右值引用的完美转发



# 智能指针

> 通过栈上的对象的自动调用析构函数的特点来进行对指针的释放
>
> 如果智能指针定义在堆上，那么就没有意义

## 浅拷贝问题

> 指针和普通对象不同，当指针发生浅拷贝的时候，无论是拷贝构造还是拷贝赋值，本意都是想让不同的指针指向同一个地方。
>
> 那么会发生析构野指针的问题，因为浅拷贝后，最内层指针的析构已经把外部资源释放掉了

- `auto_ptr`，会将前面的`ptr`置空，只有最后的`ptr`有效，所以再访问前面的`ptr`会有问题
- `scope_ptr`，在源码中`delete`了拷贝构造和拷贝赋值
- `unique_ptr`，同样`delete`了拷贝构造和拷贝赋值，但是提供了**带右值引用**的拷贝构造和拷贝赋值，并且在使用上会有很明显的移动资源的意图

```cpp
unique_ptr<int> p(new int);
//对p使用std::move进行移动
unique_ptr<int> p2(std::move(p));
```

## 带引用计数的智能指针

> 实现多个指针持有同一个资源
>
> 拥有一个`count`进行计数，意味着必须保证线程安全
>
> 只有最后一个`ptr`析构掉的时候才会进行资源的释放
>
> 通过一个`RefCnt`类进行引用计数

- `shared_ptr`，强智能指针，会改变引用计数
- `weak_ptr`，弱智能指针，不会改变引用计数

>强智能指针中发生的交叉引用问题，会造成资源无法释放
>
>``` cpp
>class B;
>class A {
>public:
>shared_ptr<B> _ptrB;
>}
>class B {
>public:
>shared_ptr<A> ptrA;
>}
>
>int main() {
>	shared_ptr<A> pa(new A);
>	shared_ptr<B> pb(new B);
>    
>    pa->_ptrB = pb;
>    pb->_ptrA = pa;
>}
>```

- 定义对象时用`shared_ptr`，引用对象时用`weak_ptr`。但由于`weak_ptr`只是观察资源，没有提供`operator*()`和`operator->()`方法，所以无法使用资源，需要**提升**

```cpp
class B;
class A {
public:
void goodFuncInA();
weak_ptr<B> _ptrB;
}
//if class B want to use A by _ptrA,
//use .lock() func
class B {
public:
void func() {
	shared_ptr<A> tmpPtr = _ptrA.lock() // try to update ptr level to shared_ptr
    if(tmpPtr) {
       tmpPtr->goodFuncInA(); 
    }
}
weak_ptr<A> _ptrA;
}

int main() {
	shared_ptr<A> pa(new A);
	shared_ptr<B> pb(new B);
    
    pa->_ptrB = pb;
    pb->_ptrA = pa;
}
//use weak_ptr can release resources success
```

## 多线程中共享对象的线程安全问题

> 因为不确定共享对象在哪个线程中被释放了，其他线程再访问对象，就不应该了

- 使用强弱指针

``` cpp
class A {
public:
	void func() {}    
};

void handler(weak_ptr<A> wp) {
	shared_ptr<A> sp = wp.lock()
        if(sp) {	//object dose not release
            sp->func();
        }else {	//object already release

        }
}

//thread main
int main() {
	shared_ptr<A> ptr(new A);
	thread t1(handler, weak_ptr<A> wp);
}
```

## 删除器

> 当使用智能指针管理资源的时候，资源的释放并不是只通过`delete`，如果是数组类，需要`delete[]`，如果是文件类，需要`fclose`等

- 使用`lambda`表达式，函数对象

```cpp
int main() {
    unique_ptr<int, function<void(int*)>> ptr(new int[100],
	[](int* p)->void {
        delete[] p;
    })
	
	unique_ptr<FILE, function<void(FILE*)>> ptr2(fopen("data.txt", "w"), [](FILE* p)->void{
        fclose(p);
    })
}
```



# 函数对象

> 提供`obj.operator()`重载的对象

## 函数类型/函数指针类型

> 结合`typedef`使用函数指针、函数类型
>
> `typedef`了一个类型
>
> 结合`using`使用
>
> ```cpp
> using FUNC1 = R(*)(...);
> using FUNC2 = R(...);
> ```
>
> 

- 函数类型，`R(...)`形式
- 函数指针类型，`R(*)(...)`形式

```cpp
int sum(int a, int b) {
    return a + b;
}

typedef int(*Func1)(int, int);	//typedef了一个函数指针类型
Func1 addFunc1 = add;
std::cout << addFunc1(1, 2) << std::endl;

typedef int(Func2)(int, int);	//typedef了一个函数类型
Func2* addFunc2 = add;
std:: cout << addFunc2(1, 2) << std::endl;	//typeid().name(addFunc2) -> 函数指针类型
--------------------------------------------
std:: cout << (*addFunc2)(1, 2) << std::endl; //typeif().name() -> 函数类型
```

> `cpp`编译器会进行隐式解引用
>
> `addFunc2(1, 2)`隐式解引用
>
> `(*addFunc)(1, 2)`显式解引用

- 函数对象

```cpp
void sayHello() {
	std::cout << "hello!" << std::endl;
}
void say(string str) {
	std::cout << str << std::endl;
}
int sum(int a, int b) {
	return a + b;
}
/*
	call class function
*/
class A {
  public:
    void Afunc(/*this pointer*/, string str) { std::cout << str << std::endl;}
};

int main() {
	function<void()> func1 = sayHello;
	function<void(string)> func2(say);
	function<int(int, int)> func3 = sum;
	function<int(int, int)> func4 = [](int a,int b)->int {return a + b;};

	/*class function pointer*/
    function<void(A*, string)> func5 = &A::Afunc;
    func5(&A(),"call A::Afunc");
}
```

## 使用场景

> `switch`下，可以优化成用`function map`

```cpp
/*
actoinMap中的function对象需要函数类型相同，本例下都是void()
*/
void func1();
void func2();
void func3();
void func4();

map<int, function<void()>> actionMap;
actionMap.insert({1, func1});
actionMap.insert({2, func2});
actionMap.insert({3, func3});
actionMap.insert({4, func4});

int choice = 0;
while(1) {
    
    std::cin >> choice;
    auto it = actionMap.find(choice);
    if(it == actionMap.end()) {
        //unvalid action
    }else {
        //excute action
        it->second();
    }
}
```

# 模板

> 特殊的实例化，意味着模板的实例化不是由编译器实现，由开发者提供

- 模板实参推演，当编译器进行实参推演的同时，会进行默认的实例化编译，在某些情况下，使用编译器的实例化不符合预期

```cpp
/*
compare tempalte
*/
template<typename T>
bool compare(T a, T b) {
    return a > b;
}
int main() {
    bool ret1 = compare(10, 20); //编译器进行实参推演，int，并且实例化出形参为int的函数编译
    
    /*
    编译器推演出为const char*，实例化出的compare就成了address比较
    不符合实际逻辑
    */
    bool ret2 = compare("aaa", "bbb"); 
    return 0;
}
```

因此需要新增一个**特例化模板**，此时`compare`已经是模板，特例化相当于实现这个函数，需要用到模板实例化语法，`compare<const char*>`

```cpp
template<>
bool compare<const char*>(const char* a, const char* b) {
    return strcmp(a, b) > 0;
}
```

## 完全特例化/部分特例化

> 特例化的类型范围的不同

- 匹配顺序，完全->部分->模板 

```cpp
/*
类模板
*/
template<typename T>
class Vector {
	public:
		Vector() { std::cout << "Vector template" << std::endl; }
};

//针对char*类型的完全特例化，针对这一个类型，它的类型是 完全 的了
template<>
class Vector<char*> {
    public:
    	Vector() { /*TODO*/ }
};

//针对指针类型的部分特例化，只知道 类型的 部分 是指针，所以还需要typename
template<typename Ty>
class Vector<Ty*> {
    public:
    	Vector() { /*TODO*/ }   
};

//针对函数指针（有返回值，两个形参）类型的部分特例化
template<typename R,typename A1, typename A2>
class Vector<R(*)(A1, A2)> {
    //TODO
}

int main() {
    Vector<int> vec1; //匹配 模板
    Vector<char*> vec2; //匹配 完全特例化
    Vector<int*> vec3; //匹配 部分特例化
    Vector<int(*)(int, int)> vec4; //匹配 部分特例化
	return 0;
}
```

## 实参推演

> 实参推演和模板特例化的灵活使用

```cpp
template<typename R, typename T, typename A1, typename T2>
void func(R(T::*cb)(A1, A2)) {
    /*
    	进行实参推演，在单独的template<typename T>中更灵活也更清晰
    */
    std::cout << typeid(R).name() << std::endl;
    std::cout << typeid(T).name() << std::endl;
    std::cout << typeid(A1).name() << std::endl;
    std::cout << typeid(A2).name() << std::endl;
}
class Test {
    public:
    	int sum(int a, int b) { return a + b; }
};

func(&T::sum);
```

上例中，传入`func`的形参是一个函数指针，如果想调用该`cb`，则需要新增形参

```cpp
template<typename R, typename T, typename A1, typename A2>
void func(R(T::* cb)(A1, A2), T* obj, A1 a1, A2 a2) {
	std::cout << (obj->*cb)(a1, a2) << std::endl;
}
```

- `std::bind`相较于函数指针更灵活

# 函数指针，函数对象，std::bind

> 实现“调用函数”的方式

## 函数指针

```cpp
int sum(int a,int b) { return a + b; }

typedef int(*FUNCptr)(int, int);
void func(FUNCptr fp, int a, int b) {
    fp = sum;
    int ret = fp(a,b);
}
```

## 函数对象

> 实现了`operator()`重载的对象

```cpp
class Sum {
    public:
    	Sum(int val = 5)
            :val_(val) {}
    	int operator()(int a) { return a + val_; }
	private:
    	int val_;
};
int main() {
    Sum add(10);	//存储被加数状态
    int ret = add(20); //计算20 + 10
    Sum add2;
    int ret2 = add(10); //计算10 + 5
    return 0;
}
```

函数对象可以通过定义成员变量来**存储状态**，也可以通过提供成员方法来**改变状态**

## std::bind

> 返回的是一个函数对象

```cpp
class Test {
    public:
    	int sum(int a, int b) { return  a + b; }
}
int main() {
    auto boundfunc = std::bind(&Test::sum, new Test(), std::placehodlers::_1, std::placeholders::_2);	//参数占位符，调用的时候才传入参数
    int ret = boundfunc(10, 29);	//调用的时候传入参数
    return 0;
}
```

绑定器也能实现状态存储，只需要在定义的时候传入绑定的参数即可

```cpp
std::bind(&Test::sum, new Test(), 10, 20) //绑定参数
```

其中，使用`new Test()`传入`this`指针的方法会导致内存泄漏，直到程序结束后，`os`才会清理堆上的内存，因此在程序执行过程中都处于内存泄漏的状态，大概...

可以使用智能指针代替`shared_ptr/unique_ptr`

```cpp
std::unique_ptr<Test> up(new Test());
auto boundfunc = std::bind(&Test::sum, up, ...);
std::shared_ptr<Test> sp(new Test());
auto boundfunc2 = std::bind(&Test::sum, sp, ...);
```

## 延迟执行

> 三者都可以实现

- 函数指针

参考**实参推演**，利用**模板**可以实现成员函数的回调

- 函数对象

```cpp
int main() {
    auto cb = [](int a, int b)->int { return a + b; };
    void process(int a, int b, function<int(int, int)> callback) {
        int ret = callback(a, b); //回调
    }
}
```

函数对象实现成员函数的回调，使用模板

```cpp
class Test {
	public:
		int sum(int a, int b) { return a + b; }
		int multi(int a, int b) { return a * b; }
};
template<typename R, typename T, typename A1, typename A2>
class CallBack {
public:
	CallBack(const T* obj, const R(T::*func)(A1, A2))
		:obj_(obj),
		callback_(func) {}
	void operator()(A1 a1, A2 a2) {
		obj_->*callback_(a1, a2);
	}
private:
	T* obj_;
	R(T::*callback_)(A1, A2);
};

int main() {
	Test obj;
	CallBack<Test, int, int, int> sumCallBack(&obj, &Test::sum);
	CallBack<Test, int, int, int> multiCallBack(&obj, &Test::multi);
}
```

- `std::bind`，本身就是通过绑定返回一个函数对象，可以随时调用达到回调的目的

### 懒加载

> 属于延迟执行的一种，可以称作一种思想。只有数据需要的时候才加载。

```cpp
#include <iostream>
using namespace std;

class Lazy {
private:
    int *data;

public:
    Lazy() : data(nullptr) {}

    // 懒加载方法：只有在需要时才初始化数据
    int getData() {
        if (data == nullptr) {
            data = new int(42);  // 只有在第一次访问时才创建数据
            cout << "Data initialized!" << endl;
        }
        return *data;
    }

    ~Lazy() {
        if (data) {
            delete data;
        }
    }
};

int main() {
    Lazy obj;

    cout << "Before accessing data" << endl;
    // 数据尚未初始化，只有调用 getData 时才会初始化
    cout << "Data: " << obj.getData() << endl;

    return 0;
}
```



# function实现原理

> 通过模板实现

```cpp
template<typename Ty>
class myfunction {};

//模板偏特化一个myfunction，一个返回值R，不确定形参个数A...
template<typename R, typename... A>	//typename... 可变参模板，不确定形参个数
class myfunction<R(A...)> {
public:
    using FUNC = R(*)(A...);
    myfunction(FUNC pfunc)
        :pfunc_(pfunc) {}
    R operator(A... arg) {
		return pfunc_(arg...);		//传入arg的时候需要可变参语法arg...
    }
private:
    FUNC pfunc_;
};

void hello(string str) { std::cout << str << std::endl; }

int main() {
    myfunction<void(string)> func1(hello);
  	func1("hello");
    return 0;
}
```

# function&bind实现的简单线程池

```cpp
class Thread {
public:
    using FUNC = std::function<void(int)>;
    Thread(FUNC func, int no)
    	:func_(func),
    	no_(no) {}
    thread start() {
        std::thread t(func_, no_);
        return t;
    }
private:
    FUNC func_;
    int no_;
};

class ThreadPool {
public:
    ThreadPool() {}
    ~ThreadPool() {
        for(int i = 0; i < pool_.size(); ++i) {
            delete pool_[i];
        }
    }
    void startPool(int size) {
        for(int i = 0; i < size; ++i) {
            pool_.push_back(
            	new Thread(std::bind(
                	&ThreadPool::runInThread, this, std::placeholders::_1), i
                ))
            )
        }
        
        for(int i = 0; i < size; ++i ){
            handler.push_back(pool_[i]->start());
        }
        
        for(std::thread& t : handler_) {
            t.join();
        }
        
    }
private:
    std::vector<Thread*> pool_;
    std::vector<Thread> handler_;
    /*
    	提供一个线程运行的函数，实现成
    	成员方法可以方便的使用如果有ThreadPool的其他成员属性如锁、信号量等，实现更复杂的线程池
    */
    void runInThread(int id) {
        std::cout << "call runInThread:" << id << std::endl; 
    }
};
```

# lambda表达式

> 更多应用在泛型算法、优先级队列以及智能指针删除器中

- 优先级队列

```cpp
class Data {
    //使用lambda表达式可以避免写更多冗余的比较
    Data(int val1 = 10, int val2 = 10)
        :ma_(val1),
    	mb_(val2) {}
private:
    int ma_;
    int mb_;
};

int main() {
    using FUNC = function<bool(Data&, Data&)>;
    priority_queue<Data, vector<Data>, /*模板中第三个是比较器的函数对象*/FUNC)> maxHeap(
    /*有提供传入函数对象的构造函数*/
    	[](Data& d1, Data& d2)->bool{
            return d1.ma_ > d2.mb_; //爱咋比咋比
        }
    )
    return 0;
}
```

- 泛型算法

```cpp
int main() {
    std::sort(vec.begin(), vec.end(), [](int a, int b)->bool { return a > b; });
    std::find_if(vec.begin, vec.end(), [](int a)->bool { return a > 65; });
    std::for_each(vec.begin(), vec.end(), [](int a)->void{
        if(a % 2 == 0)
            std::cout << a;
    });
}
```

- 智能指针删除器

见**智能指针**
