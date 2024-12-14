#pragma once
#ifndef __ALLOCATOR_H__
#define __ALLOCATOR_H__

/*
    ����construct����ֵ����
*/
template <typename T>
class Allocator {
public:
    //�ڴ濪��
    T* allocate(size_t size) { return (T*)malloc(sizeof(T) * size); }
    //�ڴ��ͷ�
    void deallocate(void* p) { free(p); }
    //������
    //void construct(T* p, const T& src) { new (p) T(src); } //��λnew����ָ���ڴ��Ϲ������
    //void construct(T* p, T&& src) { new(p) T(std::move(src)); }
    template <typename _Ty>
    void construct(T* p, _Ty&& src) { new (p) T(std::forward<_Ty>(src)); }

    //��������
    void destory(T* p) { p->~T(); } //����T���͵���������
};

#endif // ! __ALLOCATOR_H__
