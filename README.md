1.函数默认值传参
-	給默认值时，从右向左給。因为压栈顺序是右->左；
	- 调用效率上，生成的指令更少，默认值的情况下直接操作立即数就行；
	- 声明、定义都可以給出默认值，但每个形参默认值只能給一次；

2.inline
-	当函数调用开销（实参的cp、压栈、退栈等）占比过大的时候，可以使用inline避免；
	- 是否inline是由编译器决定；
	- 也不会生成函数符号（.text段中、也可以查看符号表）；
	- 递归不会被编译器处理成inline；
	- release版本中才出现inline;

3.函数重载
-	CPP支持函数重载、C不支持。CPP产生函数符号：函数名+参数列表、C：函数名；
	- CPP与C相互调用，在CPP中 extern "C" {} 即可；
	- CPP编译器内置 __cplusplus 宏，可以实现代码的跨C移植；
	- 需要考虑作用域，在下例中，main()中没有函数重载，在同一个作用域中才能称得上重载；
	```
	- bool compare(int a,int b) {
	-	return a > b;
	- }
	- bool compare(const char* a, const char* b) {
	-	return strcmp(a, b);
	- }
	- 
	- int main() {
	-	bool compare(int a,int b);
	-	std::cout << compare("aaa","bbb") << std::endl; //报错 const char* 无法强转 int;
	-	return 0;
	- }
	```

4.const
-	CPP对const修饰的常量进行编译的方式，直接**替换**；C中将const修饰的量当作变量进行编译
	```
	- int b = 10;
	- const int a = b;
	- ... //后续的代码CPP中替换成b，不是10，退化成常变量
	```

	```
	- const int a = 10; //无论C还是CPP，a都被改为30
	- int* p = (int*) & a;
	- *p = 30
	- printf(a, *P, *(&a));
	```
	- C:30 30 30 a不能再作为左值修改，但是可以通过指针修改，const只是确保**语法**层面上不能修改
	- CPP:20 30 30 const修饰a后，后续出现a的代码替换成
-	CPP中的const必须初始化——常量；C中的const可以不初始化——常变量；

5.const与一二级指针
-	CPP语言规范，const修饰的是离它最近的类型；
-	const强转：const int* 强转 int* 不行；int* 强转 const int * 可以
	- const 右边没有*的时候，不参与类型。
	- int const * p, p类型：int const *；
	- int* const p, p类型： int*；

	- const int* p = &a; int 是被const修饰的类型，所以表达式*p不能再作为左值修改，p可以
	- p = &c；//可以指向任意地址 *p = 30; //不可以再修改指向的地址的值了
	
	- int* const p = &a; int* 是被const修饰的类型，所以p不能再作为左值修改，*p可以
	- p = &c；//不可以再指向其他地址 *p = 30; //可以修改a的值

6.cpp引用
-	引用和指针在汇编上一样

7.空间配置器allocator
	- **需要把内存开辟和对象构造分开处理**，new关键字会同时进行
	-	当使用模板的时候，如果模板类型是 class 的话，会出现大问题
	-	e.g
		-	定义一个vector（没有allocator），类型是Test class，并且vector中的构造size = 10。会导致new关键字调用了10次Test()
		-	但本意应该是vector<Test> vec;的时候，内存开辟，但对象不应该构造
		-	并且在delete[]的时候，析构了10次，相当于把开辟的内存都当作Test对象进行处理
		-	但本意应该是析构容器**有效**的元素，再释放容器指向的堆内存
		-	push_back()中的问题
			-	没有allocator的时候，由于在定义的时候就已经在vec指向的堆内存中，构造了Test对象，再push_back(t1)的时候，就变成了
			-	将容器中的对象(定义vec时构造的)cp赋值成t1
			-	但本意应该是，定义时只开辟内存，push_back(t1)时，在容器的内存空间中再构造t1对象
		-	pop_back()中的问题
			-	需要考虑对象指向了外部空间的问题，不能简单的_last--，这样不会调用析构
			-	本意应该是，只析构对象，但不释放内存，所以不能用delete
	-	allocator只做4件事；内存的开辟/释放、对象的构造/析构

8.迭代器
-	采用内部类实现的方式
	- 重载\++运算符的时候，尽量用前置\++，因为前置\++返回引用，后置++返回对象，效率上更优
	- 在成员函数提供begin()\end()方法
	- 编译器的foreach也是调用上述两种方法
-	迭代器的失效
	- 为什么会失效
	- erase会使删除点->末尾元素迭代器全部失效
		- 正确使用是，因为erase调用后，会返回新的iterator，只需要将it重新赋值就行
		- 删除逻辑会使vector元素往前移，新的it指向的虽然还是同一个位置，但是*it出来的元素是新的
		- e.g：删除vector中的所有偶数
		```
	-	while(it != vec.end()) {
		-	if(*it % 2 != 0) ++it;
			- else 
			-	it = vec.erase(it); //指向新的iterator
			- 
		- }
		- 
	- ```
	- increase，不扩容情况下，会使增加点->末尾元素迭代器全部失效
		- 要实现vector的所有偶数前加一个数
		- for(auto it = vec.begin(); it != vec.end(); ++it) {
		-   if(*it % 2 == 0) {
			-  it = vec.increase(it, *it - 1) //新增 偶数-1；
			-  ++it; //一定要再加一次it，不加的话进入for判断，it会重新指向原来的偶数，因为当前it指向的是新增的值，原来的偶数被后移了，死循环 
			- }
		- }
	- increase，导致扩容情况下，会使容器原来的所有迭代器都失效，因为扩容会指向新的内存空间