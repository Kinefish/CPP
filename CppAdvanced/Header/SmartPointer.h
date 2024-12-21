#pragma once
#ifndef __SMART_POINTER_H__
#define __SMART_POINTER_H__

//类似于cpp自带的class auto_ptr，存在浅拷贝问题，不带引用计数
template <typename T>
class SmartPointer {
public:
    SmartPointer(T* ptr = nullptr)
        :_ptr(ptr)
    {}
    ~SmartPointer() { delete _ptr; }

    T& operator*() { return *_ptr; }
    T* operator->() { return _ptr; }
    
private:
    T* _ptr;
};

//带引用计数的智能指针，没有考虑多线程安全
template <typename T>
class RefCnt {
public:
    RefCnt(T* ptr = nullptr)
        :_reptr(ptr) {
        if (ptr) _cnt = 1;
    }
    ~RefCnt() {
        delete _reptr;
        _reptr = nullptr;
    }

    void add_cnt() { ++_cnt; }
    int sub_cnt() { return --_cnt; }

private:
    T* _reptr;
    int _cnt;
};

template <typename T>
class SmartPointer_1 {
public:
    SmartPointer_1(T* ptr = nullptr)
        :_ptr(ptr), _ptrCnt(new RefCnt<T>(ptr))
    {}
    
    SmartPointer_1(const SmartPointer_1<T>& src)
        :_ptr(src._ptr), _ptrCnt(src._ptrCnt) {
        if (_ptr)
            _ptrCnt->add_cnt();
    }
    
    ~SmartPointer_1() { if (!_ptrCnt->sub_cnt()) delete _ptr; _ptr = nullptr; }

    SmartPointer_1<T>& operator= (const SmartPointer_1<T>& src) {
        if (this != &src) {
            if (!_ptrCnt->sub_cnt()) delete _ptr;
            _ptr = src._ptr;
            _ptrCnt = src._ptrCnt;
            _ptrCnt->add_cnt();
        }
        return *this;
    }

    T& operator*() { return *_ptr; }
    T* operator->() { return _ptr; }
    
private:
    T* _ptr;
    RefCnt<T>* _ptrCnt;
};

#include <iostream>
namespace SmartPointerTest {                                                                                                                                                                                                                                                                                                                                                                                                                                   
    class Test {
    public:
        void show() { std::cout << "Test::show()" << std::endl; }
    };
    static void test01() {
        SmartPointer<int> p(new int);
    }
    static void test02() {
        SmartPointer_1<int> ptr(new int);
        SmartPointer_1<int> ptr2 = ptr;
        SmartPointer_1<int> ptr3(ptr2);

        *ptr = 20;

        std::cout << *ptr2 << " " << *ptr3 << std::endl;
    }
}

#endif // !__SMART_POINTER_H__

