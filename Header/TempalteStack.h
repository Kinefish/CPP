#pragma once
#ifndef __TEMPLATE_STACK_H__
#define __TEMPLATE_STACK_H__

template <typename T>
class Stack {
    Stack(int size)
        :_top(0),
        _size(size),
        _pArray(new T[size])
    {}
    Stack(const Stack<T>& src)
        :_top(src._top),
        _size(src._size),
        _pArray(new int[src._size]) {
        for (int i = 0; i < _top; i++) {
            _pArray[i] = src._pArray[i];
        }
    }
    Stack<T>& operator=(const Stack<T>& src) {
        if (this == &src) return *this;
        delete[] _pArray;

        _top = src._top;
        _size = src._size;
        
        _pArray = new int[src._size]
        for (int i = 0; i < _top; i++) {
            _pArray[i] = src._pArray[i];
        }
        return *this;
    }
    ~Stack() {
        delete[] _pArray;
        _pArray = nullptr;
    }

    void push(const T& val) {
        if (full())
            resize();
        _pArray[_top++] = val;
    }
    void pop() {
        if (empty())
            throw"stack is empty!";
        _top--£»
    }
    T top() const {
        return _pArray[_top - 1];
    }

    void empty() const { return _top == 0 };
    void full() const { return _top == _size };

private:
    T* _pArray;
    int _top;
    int _size;
    void resize() {
        T* newStack = new int[2 * _size];
        for (int i = 0; i < _top; i++) {
            newStack[i] = _pArray[i];
        }
        delete[]_pArray;
        _pArray = newStack;
        _size *= 2;
    }
};

#endif // !__TEMPLATE_STACK_H__
