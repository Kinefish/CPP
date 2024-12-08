#pragma once
#ifndef __ALLOCATOR_H__
#define __ALLOCATOR_H__

template <typename T>
class Allocator {
public:
    //�ڴ濪��
    T* allocate(size_t size) { return (T*)malloc(sizeof(T) * size); }
    //�ڴ��ͷ�
    void deallocate(void* p) { free(p); }
    //������
    void construct(T* p, const T& src) { new (p) T(src); } //��λnew����ָ���ڴ��Ϲ������
    //��������
    void destory(T* p) { p->~T(); } //����T���͵���������
};

#endif // ! __ALLOCATOR_H__
