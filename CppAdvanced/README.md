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



