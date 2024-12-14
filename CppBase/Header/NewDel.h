#pragma once
#ifndef __NEWDEL_H__
#define __NEWDEL_H__

/*
重载new、delete关键字的全局函数
汇编中其实是调用了operator new()、operator delete()
*/

/*
new delete
new[] delete[]的混用，只建议成对使用

假设Test对象占4bytes
Test *p = new Test[5];发生了什么？
在堆上开辟4+5*4 = 24bytes内存
其中返回的地址是 p == 0x104；
其实真正malloc的起始地址是 0x100;
用4bytes去记录[5]中的5

delete[]p;发生了什么？
由于使用的是delete[]，默认new的时候是以new[]形式，所以默认malloc的起始地址存有对象个数的数据，
这样就知道应该 析构 多少个对象，从p地址开始析构
最后释放malloc的地址，也就是起始地址0x100 == p - 4;

如果在class中混用，会导致只析构一个对象，并且从p开始释放内存，导致崩溃

*/

/*
new 和 malloc
malloc只负责内存的开辟
new负责内存开辟、初始化赋值（对象构造）

malloc 返回 void*
new 强转后再返回

malloc失败返回nullptr
new失败返回bad_alloc()异常
*/

/*
一些可以防止内存泄露的方法
new delete做一张映射表
*/

#include <iostream>
//会先调用operator new()，再调用构造函数
void* operator new(size_t size) {
    void* ptr = malloc(size);
    if (!ptr) throw std::bad_alloc();
    return ptr;
}

//会先调用对象的析构函数，再调用operator delete()
void operator delete(void* ptr) {
    free(ptr);
}

void* operator new[](size_t size) {
    void* ptr = malloc(size);
    if (!ptr) throw std::bad_alloc();
    return ptr;
}

void operator delete[](void* ptr) {
    free(ptr);
}

#endif