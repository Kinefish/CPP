#include <iostream>
//void f1();
//void f2();
//int conference() {
//    f2();
//    return 0;
//}
//void f1() {
//    //�������ñ��� ��������
//    int array[5] = {};
//    int (&p)[5] = array;
//    std::cout << sizeof(array) << std::endl;
//    std::cout << sizeof(p) << std::endl;
//}
//
//void f2() {
//    /*
//        ��ֵ�����ڴ桢�����֡�ֵ�����޸ĵģ�
//        ��ֵ���෴
//    */
//
//    int a = 10; // a����ֵ
//    int& conf_a = a; //��ֵ����
//
//    
//    int&& conf_constant_1 = 20; //��ֵ����
//    const int& conf_constant_2 = 20; //��ֵ����
//    /*��ֵ���û��:
//        00744C0C  mov         dword ptr [ebp-30h],14h
//        00744C13  lea         eax,[ebp-30h]
//        00744C16  mov         dword ptr [conf_constant_1],eax
//        ��ջ֡����һ���ڴ棬д��20���ٽ�����ڴ�ĵ�ַ���ص�eax�У���������ַcp�����ñ���conf_constant_1ָ���λ��
//    */
//}