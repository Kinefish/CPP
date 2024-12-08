/*
    oop实现栈
*/

#include <stdlib.h>
class Stack
{
public:
    Stack(int);
    Stack(const Stack&); //深拷贝
    void operator=(const Stack& src);
    ~Stack();

    void push(int val) {
        if (full())
            resize();
        _pStack[++_top] = val;
    }

    void pop() {
        if (empty())
            return;
        _top--;
    }

    int top() {
        return _pStack[_top];
    }

    bool empty() {
        return _top == -1;
    }
    bool full() {
        return _top == _size - 1;
    }

private:
    int* _pStack; //动态开辟
    int _top;
    int _size;
    void resize() {
        int* newStack = new int[2 * _size]();
        for (int i = 0; i < _size; i++) {
            newStack[i] = _pStack[i];
        }
        delete[] _pStack;
        _pStack = newStack;
        _size *= 2;
    }
};

Stack::Stack(int size = 10) {
    _pStack = new int[size];
    _top = -1;
    _size = size;
}

Stack::Stack(const Stack& src) {
    _pStack = new int[src._size];
    for (int i = 0; i < src._size; i++) {
        _pStack[i] = src._pStack[i];
    }
    _top = src._top;
    _size = src._size;
}

/*
    赋值拷贝，前提是两个对象存在，既然存在，就有可能存在对象指向外部资源的情况，执行赋值操作的时候，需要
    **先释放原先的资源**
    并且注意自赋值
    s1 = s1;
*/
void Stack::operator=(const Stack& src) {
    if (this == &src) return;
    delete[] _pStack;
    _pStack = new int[src._size];
    for (int i = 0; i < src._size; i++) {
        _pStack[i] = src._pStack[i];
    }
    _top = src._top;
    _size = src._size;
}

Stack::~Stack() {
    delete[] _pStack;
    _pStack = nullptr;
}