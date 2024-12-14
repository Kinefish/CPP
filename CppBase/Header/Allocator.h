#pragma once
#ifndef __ALLOCATOR_H__
#define __ALLOCATOR_H__

/*
    新增construct的右值引用
*/
template <typename T>
class Allocator {
public:
    //内存开辟
    T* allocate(size_t size) { return (T*)malloc(sizeof(T) * size); }
    //内存释放
    void deallocate(void* p) { free(p); }
    //对象构造
    //void construct(T* p, const T& src) { new (p) T(src); } //定位new，在指定内存上构造对象
    //void construct(T* p, T&& src) { new(p) T(std::move(src)); }
    template <typename _Ty>
    void construct(T* p, _Ty&& src) { new (p) T(std::forward<_Ty>(src)); }

    //对象析构
    void destory(T* p) { p->~T(); } //调用T类型的析构函数
};

#endif // ! __ALLOCATOR_H__
