/*
    class�ڱ����ʱ�򣬳�Ա��������ǰ���Զ����� *this
*/
/*
    ���������
    ��Ա������ʼ��˳��������˳���йأ������������б�����˳���޹�
    ��������ڶ��ϣ�newһ��������������������ǲ����Զ�����+�ͷ��ڴ�ģ���Ҫ�ֶ�delete
    ջ�ϵĶ����뿪��������Զ�������������
    ȫ�����ǳ�������ŵ���
*/
/*
    ��̬��Ա���������༶��ģ����Ƕ��󼶱�
    static�ؼ������Σ��������������ⶨ�岢��ʼ��
    ��ͨ��Ա��������һ�� class *thisָ�룬����̬��Ա����û�и�ָ��

    ����Ա���� ��������Ե��ã�����ֻ����д�ķ���������const
    type func() const {} -> const Class *this

    ��Ա����, �������ܵ���
    type func() {}
*/
#include <iostream>
class Test {
public:
    Test() {
        std::cout << "TEST() FUNC CALLING" << std::endl;
    }
    ~Test() {
        std::cout << "~TEST() FUNC CALLING" << std::endl;
    }
    void func() { std::cout << "Test::func()" << std::endl; }
    static void static_func() { std::cout << "Test::static_fucn()" << std::endl; }
    int _data;
    static int _staticData;
};

int Test::_staticData = 0;
int f1() {

    Test t1;
    int Test::* ptr = &Test::_data;
    t1.*ptr = 30;

    int* staticPtr = &Test::_staticData;
    *staticPtr = 30;

    void (Test:: * pfunc)() = &Test::func;
    (t1.*pfunc)();

    void(*staticFunc)() = &Test::static_func;
    (*staticFunc)();

    std::cout << t1._data << std::endl << Test::_staticData << std::endl;
    return 0;
}
//void oop_f1();
//void oop_f2();
//#include <iostream>
//int oop() {
//    oop_f2();
//    return 0;
//}
//
//void oop_f1() {
//    Stack s(5);
//
//    for (int i = 0; i < 15; i++) {
//        s.push(rand() % 100);
//    }
//    while (!s.empty()) {
//        std::cout << s.top() << std::endl;
//        s.pop();
//    }
//}
//
//void oop_f2() {
//    /*
//        �����ǳ����
//        ������Ĭ�ϵĿ����������ڴ�����ݿ���
//        **�������Ա�к���ָ��**������ָ���ⲿ��Դ��ʱ��ǳ����������������ָ��ͬһ���ⲿ��Դ
//        ����������ʱ���ڲ�����ִ��û���⣬�������ִ��ʱ�������ⲿ��Դ�Ѿ����ڲ������ͷţ������������ָ�� �Ѿ����ͷŵ���Դ���ͳ�������
//        
//        ����class�ڵ�cpʹ��memcpy��realloc���ڴ濽����������class��ĳ����Ա������һ�����飬������������ŵ����ⲿ��Դ��ʱ����ô�����ڴ濽���ͻ�core down
//        ��for
//
//
//    */
//    Stack s1(10);
//    for (int i = 0; i < 15; i++) {
//        s1.push(rand() % 100);
//    }
//    //���俽�������Ч��û���������������£�����Ĭ�Ͽ������죬�������������������������
//    Stack s2(s1);   //�ȼ��� Stack s2 = s1;
//    //s2 = s1 core down����Ϊ = û������
//    s2 = s1;
//}