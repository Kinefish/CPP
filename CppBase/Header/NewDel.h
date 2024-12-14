#pragma once
#ifndef __NEWDEL_H__
#define __NEWDEL_H__

/*
����new��delete�ؼ��ֵ�ȫ�ֺ���
�������ʵ�ǵ�����operator new()��operator delete()
*/

/*
new delete
new[] delete[]�Ļ��ã�ֻ����ɶ�ʹ��

����Test����ռ4bytes
Test *p = new Test[5];������ʲô��
�ڶ��Ͽ���4+5*4 = 24bytes�ڴ�
���з��صĵ�ַ�� p == 0x104��
��ʵ����malloc����ʼ��ַ�� 0x100;
��4bytesȥ��¼[5]�е�5

delete[]p;������ʲô��
����ʹ�õ���delete[]��Ĭ��new��ʱ������new[]��ʽ������Ĭ��malloc����ʼ��ַ���ж�����������ݣ�
������֪��Ӧ�� ���� ���ٸ����󣬴�p��ַ��ʼ����
����ͷ�malloc�ĵ�ַ��Ҳ������ʼ��ַ0x100 == p - 4;

�����class�л��ã��ᵼ��ֻ����һ�����󣬲��Ҵ�p��ʼ�ͷ��ڴ棬���±���

*/

/*
new �� malloc
mallocֻ�����ڴ�Ŀ���
new�����ڴ濪�١���ʼ����ֵ�������죩

malloc ���� void*
new ǿת���ٷ���

mallocʧ�ܷ���nullptr
newʧ�ܷ���bad_alloc()�쳣
*/

/*
һЩ���Է�ֹ�ڴ�й¶�ķ���
new delete��һ��ӳ���
*/

#include <iostream>
//���ȵ���operator new()���ٵ��ù��캯��
void* operator new(size_t size) {
    void* ptr = malloc(size);
    if (!ptr) throw std::bad_alloc();
    return ptr;
}

//���ȵ��ö���������������ٵ���operator delete()
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