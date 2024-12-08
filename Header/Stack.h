/*
    oopʵ��ջ
*/

#include <stdlib.h>
class Stack
{
public:
    Stack(int);
    Stack(const Stack&); //���
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
    int* _pStack; //��̬����
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
    ��ֵ������ǰ��������������ڣ���Ȼ���ڣ����п��ܴ��ڶ���ָ���ⲿ��Դ�������ִ�и�ֵ������ʱ����Ҫ
    **���ͷ�ԭ�ȵ���Դ**
    ����ע���Ը�ֵ
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