# ����Ĺ��졢�����Լ���ʱ�������ֵ���ã������۵���move��forward����

> �ڱ��뷵�ض�����龳�µ��Ż�

## �Ż�

```
//�Ż�ǰ
String getString(const String str) {
	char* retStr = new char[sizeof(str._str) + 1];
	String tmpStr(retStr);
	return tmpStr;
}
int main() {
	String str1("abcd");
	String str2;
	str2 = getString(str1);		//�ֵ����˿�����ֵ
	return 0;
}
/*
���ڵ�����
1.ÿ����һ��retStr�������ڶ���newһ���ڴ������������ڴ�й¶
2.��һ������������ʱ����ʱ ���� ���غ��� ��ջ֡�� �� ����һ����ʱ����
	getString()���ý�����������tmpStr
	�ص����÷��У�
	�����
	Stirng str = getStirng(src);
	���ٽ��п������죬��������ջ֡�ϵ���ʱ����
	��ˣ�cpp������������Ż�
	�������ʱ���󿽱�����һ���¶��󣬻ᱻ�Ż����¶���Ĺ���
	String str(src);
3.�βν���û�������ã��ᵼ��ѹջ��ʱ���ֽ�����str�βεĹ���
4.���ж���Ľ���ʱ������ʹ�ù����¶���ķ�ʽ���н��ܣ������Ǹ�ֵ
*/
//�Ż�
String getString(const String& str) 
	return String(str._str);
}
int main() {
	String str1("abcd");
	String str2 = getString(str1);
	return 0;
}
```

## �Ż�������ԭ��

-	�β����������ô���
-	��������ʱ���󷵻�
-	���շ��ض���ʱ�������ù��췽ʽ����



# ������ֵ���ã��β��е���ֵ���ã�

-	��һ������ռ���ⲿ��Դ��ʱ�򣬽��п�������ʱ��ԭ�ȵķ�ʽ�ǣ�`new`һ���¿ռ䣬����`forloop`һ������ֵ
-	��ʱ��һ�������Ƿ�����ʱ�����ʱ�򣬺�������ɣ��ո�`forloop`һ������ֵ֮����������ʱ���󣬳������������Ŷ����ľ�������
-	��ʵ��õİ취ʱ��������ⲿ��Դ���Ҿ�����
-	����Ĺؼ��ǣ�**��ʱ������п�������Ϳ�����ֵ**��ʱ��������ʱ��������ԣ����ִ����ͻ����������������ʱռ�����ⲿ��Դ���ᵼ�¹���͸�ֵ��Ч�ʱ��

-	`cpp`�������У�����ʱ��������ֵ
`const String& str = String("abcd");`����ֵ������Ҫ��`const`
`String&& str = String("abcd");`����ֵ���ÿ���������ʱ����`str`����ֵ���ã���������һ����ֵ

## �Ż�

```
/*
	ΪString������ֵ���õĿ�������Ϳ�����ֵ
	Ŀ����ֱ��ռ����ʱ������ⲿ��Դ��������ʱ�����ptr�ÿ�
*/
//����������ֵ����
String(String&& str) {
	_str = str._str;
	str._str = nullptr;
}
//������ֵ��ֵ����
String& operator=(Stirng&& str) {
	if(this == &str) return *this;
	delete[] _str;

	_str = str._str;
	str._str = nullptr;
	return *this;
}
```

## ������ʱ���󣬷�����ʱ���󣨿�����ʱ����

-	����ʱ������Ϊ�βν��д��ݵ�ʱ�򣬱�����ƥ�������ֵ������Ϊ�βεĺ��������Ƚ�����ʱ����Ĺ���
-	��������ʱ������¶�����и�ֵ��ʱ�򣬱�����������Ż�����ʱ����ֱ�����¶�����ڴ��Ͻ���**��ͨ����**
-	�ɰ汾�ı��������ܻ��һ�����ض���Ĺ�����������±�����Ӧ�����Ż�����

## ������ʱ����

- ����ʹ��`templete<typename _Ty> func(_Ty&& src) { ... std::forward<_Ty>(src)}`��������ֵ���ú���ֵ���õ�����ת��



# ����ָ��

> ͨ��ջ�ϵĶ�����Զ����������������ص������ж�ָ����ͷ�
>
> �������ָ�붨���ڶ��ϣ���ô��û������

## ǳ��������

> ָ�����ͨ����ͬ����ָ�뷢��ǳ������ʱ�������ǿ������컹�ǿ�����ֵ�����ⶼ�����ò�ͬ��ָ��ָ��ͬһ���ط���
>
> ��ô�ᷢ������Ұָ������⣬��Ϊǳ���������ڲ�ָ��������Ѿ����ⲿ��Դ�ͷŵ���

- `auto_ptr`���Ὣǰ���`ptr`�ÿգ�ֻ������`ptr`��Ч�������ٷ���ǰ���`ptr`��������
- `scope_ptr`����Դ����`delete`�˿�������Ϳ�����ֵ
- `unique_ptr`��ͬ��`delete`�˿�������Ϳ�����ֵ�������ṩ��**����ֵ����**�Ŀ�������Ϳ�����ֵ��������ʹ���ϻ��к����Ե��ƶ���Դ����ͼ

```cpp
unique_ptr<int> p(new int);
//��pʹ��std::move�����ƶ�
unique_ptr<int> p2(std::move(p));
```

## �����ü���������ָ��

> ʵ�ֶ��ָ�����ͬһ����Դ
>
> ӵ��һ��`count`���м�������ζ�ű��뱣֤�̰߳�ȫ
>
> ֻ�����һ��`ptr`��������ʱ��Ż������Դ���ͷ�
>
> ͨ��һ��`RefCnt`��������ü���

- `shared_ptr`��ǿ����ָ�룬��ı����ü���
- `weak_ptr`��������ָ�룬����ı����ü���

>ǿ����ָ���з����Ľ����������⣬�������Դ�޷��ͷ�
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

- �������ʱ��`shared_ptr`�����ö���ʱ��`weak_ptr`��������`weak_ptr`ֻ�ǹ۲���Դ��û���ṩ`operator*()`��`operator->()`�����������޷�ʹ����Դ����Ҫ**����**

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

## ���߳��й��������̰߳�ȫ����

> ��Ϊ��ȷ������������ĸ��߳��б��ͷ��ˣ������߳��ٷ��ʶ��󣬾Ͳ�Ӧ����

- ʹ��ǿ��ָ��

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

## ɾ����

> ��ʹ������ָ�������Դ��ʱ����Դ���ͷŲ�����ֻͨ��`delete`������������࣬��Ҫ`delete[]`��������ļ��࣬��Ҫ`fclose`��

- ʹ��`lambda`���ʽ����������

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



