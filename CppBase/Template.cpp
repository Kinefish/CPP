/*
    ����ģ��
*/
template <typename T>
bool compare(T a, T b) {
    return a < b;
}

/*
    ģ��������
*/
#include <string.h>
template<>
bool compare<const char*>(const char* str_a, const char* str_b) {
    return strcmp(str_a, str_b) > 0;
}

/*
    ��ͨ����
*/
bool compare(const char* str_a, const char* str_b) {
    return strcmp(str_a, str_b);
}

/*
    ��ͨ������ģ�庯���������ع�ϵ��������������ͬ
    ������Ĭ�����ȵ�����ͨ���� compare()����������Ҫ����ʵ���������ݣ�
    ������
    compare<const char*>����

    ģ�岻�ܷ��ļ���.c�����ã���Ϊģ����Ҫʵ����֮����ܽ��б��룬���ļ����뵼��link 
    ����д��.h�У�#include��Ԥ����׶Σ�cp paste

*/
/*
    ���߱�����ָ��ʵ�����������ڱ���׶α���
*/
template bool compare<float>(float, float);

