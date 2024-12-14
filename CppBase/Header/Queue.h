#pragma once
#ifndef __QUEUE_H__
#define __QUEUE_H__



/*
    支持扩容的循环队列
    _rear进
    _front出
*/
class Queue {
public:
    Queue(int size);
    Queue(const Queue&);
    Queue& operator=(const Queue&);
    ~Queue();
    bool empty() { return _front == _rear; }
    bool full() { return (_rear + 1) % _size == _front; }
    void enQueue(int val) {
        if (full())
            resize();
        _pArray[_rear] = val;
        _rear = (_rear + 1) % _size;
    }
    void deQueue() {
        if (empty())
            return;
        _front = (_front + 1) % _size;
    }
    int front() {
        return _pArray[_front];
    }

private:
    int* _pArray;
    int _front;
    int _rear;
    int _size;
    void resize() {
        int* newPtr = new int[2 * _size];
        int idx = 0;
        for (int i = _front; i != _rear; i = (i + 1) % _size) {
            newPtr[idx++] = _pArray[i];
        }
        delete[] _pArray;
        _pArray = newPtr;
        _size *= 2;
        _front = 0;
        _rear = idx;
    }
};

inline Queue::~Queue() {
    delete[] _pArray;
    _pArray = nullptr;
}

inline Queue::Queue(int size = 10) :_front(0), _rear(0), _size(size) {
    _pArray = new int[size] {0};
}

inline Queue::Queue(const Queue& src) :
    _front(src._front), _rear(src._rear), _size(src._size) {
    _pArray = new int[src._size];
    for (int i = 0; i < _size; i++) {
        _pArray[i] = src._pArray[i];
    }
}

inline Queue& Queue::operator=(const Queue& src) {
    if (this == &src) return *this;
    delete[] _pArray;

    _front = src._front;
    _rear = src._rear;
    _size = src._size;

    _pArray = new int[src._size];
    for (int i = 0; i < _size; i++) {
        _pArray[i] = src._pArray[i];
    }
}

#endif // ! __QUEUE_H__

