#pragma once
#ifndef __VECTOR_H__
#define __VECTOR_H__
#include "Allocator.h"

template <typename T, typename Alloc = Allocator<T>> //使用默认的配置器，需要加<T>，不加的话只是模板名
class Vector {
public:
    Vector(int size = 5)
        /*:_first(new T[size]),*/
        :_first(_allocator.allocate(size)),
        _last(_first),
        _end(_first + size)
    {}
    ~Vector() {
        //delete[] _first;
        for (T* p = _first; p != _last; p++) {
            _allocator.destory(p);
        }
        _allocator.deallocate(_first);
        _first = _end = _last = nullptr;
    }
    Vector(const Vector<T>& src) {
        int size = src._end - src._first;
        //_first = new T[size];
        _first = _allocator.allocate(size);
        int len = src._last - src._first;
        for (int i = 0; i < len; i++) {
            //_first[i] = src._first[i];
            _allocator.construct(_first + i, src._first[i]);
        }
        _last = _first + len;
        _end = _first + size;
    }
    Vector<T>& operator=(const Vector<T>& src) {
        if (this == &src) return *this;
        //delete[] _first;
        for (T* p = _first; p != _last; p++) {
            _allocator.destory(p);
        }
        _allocator.deallocator(_first);


        int size = src._end - src._first;
        /*_first = new T[size];
        int len = src._last - src._first;
        for (int i = 0; i < len; i++) {
            _first[i] = src._first[i];
        }*/
        _first = _allocator.allocate(size);
        int len = src._last - src._first;
        for (int i = 0; i < len; i++) {
            _allocator.construct(_first + i, src._first[i]);
        }
        _last = _first + len;
        _end = _first + size;
        return *this;
    }
    void push_back(const T& val) {//push val into vector _last
        if (full())
            resize();
        //*_last++ = val;
        _allocator.construct(_last++, val);
    }
    void pop_back() { // pop val from vector _last;
        if (empty())
            throw"Vector is empty";
        //_last--;
        _allocator.destory(--_last);
    }
    T back() const { // *_last value;
        return *(_last - 1);
    }

    bool empty() const { return _last == _first; }
    bool full() const { return _last== _end; }
    int size() const { return _last - _first; }

    /*
        Vector 迭代器实现
    */
    class iterator {
    public:
        iterator(T* p = nullptr) : _p(p) {}
        void operator++() { ++_p; }
        bool operator!=(const iterator& src) const { return _p != src._p; }
        //解引用提供修改，也可以不提供修改，加const
        T& operator*() { return *_p; }
    private:
        T* _p;
    };
    iterator begin() { return iterator(_first); }
    iterator end() { return iterator(_last); }

private:
    Alloc _allocator;
    T* _first;  //start of array idx;
    T* _last;   //valid val's nxt idx;
    T* _end;    //array length nxt idx;

    void resize() {
        int size = _end - _first;
        //T* newPtr = new T[size * 2];
        T* newPtr = _allocator.allocate(size * 2);
        for (int i = 0; i < size; i++) {
            //newPtr[i] = _first[i];
            _allocator.construct(newPtr + i, _first[i]);
        }
        //delete[] _first;
        for (T* p = _first; p != _last; p++) {
            _allocator.destory(p);
        }
        _allocator.deallocate(_first);
        _first = newPtr;
        _last = _first + size;
        _end = _first + (size * 2);
    }
};


#endif // !__VECTOR_H__
