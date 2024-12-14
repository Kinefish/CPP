1.对象的构造、析构以及临时对象和右值引用，引用折叠，move、forward语义
-	优化
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

-	优化的三个原则
	-	形参优先用引用传递
	-	优先用临时对象返回
	-	接收返回对象时，优先用构造方式接受

-	关于右值引用（形参中的右值引用）
	-	当一个对象占用外部资源的时候，进行拷贝构造时，原先的方式是，new一个新空间，并且forloop一个个赋值
	-	此时当一个函数是返回临时对象的时候，很容易造成，刚刚forloop一个个赋值之后，由于是临时对象，出了作用域随着而来的就是析构
	-	其实最好的办法时，将你的外部资源给我就行了
	-	问题的关键是，**临时对象进行拷贝构造和拷贝赋值**的时候，由于临时对象的特性，语句执行完就会析构，但是如果此时占用了外部资源，会导致构造和赋值的效率变低
	
	-	cpp编译器中，将临时对象当作右值
	`const String& str = String("abcd");`，左值引用需要加cosnt
	`String&& str = String("abcd");`，右值引用可以引用临时对象，str是右值引用，但本身是一个左值
-	优化
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

-	传递临时对象，返回临时对象（拷贝临时对象）
	-	当临时对象作为形参进行传递的时候，编译器匹配的是右值引用作为形参的函数，会先进行临时对象的构造
	-	当返回临时对象给新对象进行赋值的时候，编译器会进行优化，临时对象直接在新对象的内存上进行**普通构造**
	-	旧版本的编译器可能会多一个返回对象的构造和析构，新编译器应该是优化掉了

-	传递临时对象
	- 可以使用`templete<typename _Ty> func(_Ty&& src) { ... std::forward<_Ty>(src)}`，进行左值引用和右值引用的完美转发


		