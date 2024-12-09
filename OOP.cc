/*
    �̳к��������ռ���ڴ���Ҫ���ϻ���

    �����private��Ա����public��protected��private���ּ̳з�ʽ�У������ɼ��������ⲿ�޷�����
    public�̳��£������Ա���������еķ����޶�����
    protected�̳��£������publicȨ�ޱ��protected
    private�̳��£����������Ȩ�ޱ�private���������ڷ���

    ��ʽ�̳��У��̳к��Ա�ķ����޶��� ֱ�ӻ��� �еķ����޶�

    Ĭ�ϼ̳з�ʽ��������Ķ���
    Class A : B Ĭ��private�̳�
    Struct A ��B Ĭ��public�̳�

    ����������ǿת
    �̳нṹ��ת��������ת����ָ�루���ã�ת��ֻ֧�ִ��µ��ϵ�ת��
    Ĭ���ϣ����ࣩ���£������ࣩ
    ���ڴ��Ͽ����������ռ�û���֮����ڴ棬ת����С�ڴ���Ȼ����ת���ɴ��ڴ�
    Base base();
    Derive derive();

    base = derive; OK
    Base* ptr = &derive; OK

*/

/*
    Base()
    Derive()
    ~Derive()
    ~Base()

    ��ʼ���б���ִ�У���ִ�й��캯��������{}
    ���������г�ʼ������ĳ�Ա����Ҫ��������ĳ�ʼ���б��е��û���Ĺ��캯��
    ������ִ��Base()->Derive()
    �������������ں�
    ��ִ��~Derive()->~Base()
*/

/*
    ��̬�󶨡���̬�󶨣���Ҳ���Ǻ�������
    ����׶��γɻ��ָ�� call Base::show() ��̬��
    call [reg] ��̬�󶨣�������virtual function��ʱ�򣬽��ж�̬��

    
    �麯��virtual
    a.һ���ඨ����virtual func()��ʱ�򣬱����������������һ��vftable
    vftable
    |-----------|
    |&RTTI      |->"Base"(����Base�ඨ����virtual function)��&RTTI��run-time type information������ǳ����ַ�����ַ
    |-----------|
    |offset     |
    |-----------|
    |&Base::vtFunc_1()|->Base����virtual function 1 �ĵ�ַ
    |&Base::vtFunc_2()|...
    ...
    ��������ʱ���Ὣvftable����ص�.rodata�Σ�read-only data��

    b.�����л���һ����ַ &vfptr��ָ��vftable������ͨ��vfptr��ƫ�����ҵ�virtual function

    c.�������� &vfptr��ָ��ͬһ��vftable

    d.����������ĳһ����Ա�����ķ������͡��������������б�������ĳ��virtual function��ͬ
    (ע��������Ҳ����Ҫ��ĳ��virtual function��ͬ��ֻҪBase����virtual����ô������ͻ���vftable)
    ��ô�������������������һ��vftable��ͬʱ����function->virtual function
    ���һ����Լ���vftable���ǻ����&Base::vtFunc()��Ϣ
    ����ptr->show()�ͻ����vftable����&Derive::vtFunc()����

    ֻҪBase��virtual function����ô���ͻ���vftable��
    ����������Ҳ����vftable�������������п��ܹ��û����vfptr
*/
#include <iostream>
class Base {
public:
	Base(int data = 10)
        :_base_a(data) {
        std::cout << "Base()" << std::endl;
    }
    ~Base() {
        std::cout << "~Base()" << std::endl;
    }
    virtual void show() const { std::cout << "Base::show()" << std::endl; }
    virtual void show(int) const { std::cout << "Base::show(int)" << std::endl; }
protected:
    int _base_a;
};

class Derive : public Base {
public:
    Derive(int data)
        :Base(data),
        _derive_a(data) {
        std::cout << "Derive()" << std::endl;
    }
    ~Derive() {
        std::cout << "~Derive()" << std::endl;
    }
    void show() const { std::cout << "Derive::show()" << std::endl; }
private:
    int _derive_a;
};

static void func() {
    Derive derive(20);
    Base base;
    std::cout << "++++++++++++++++++" << std::endl;
    Base* ptr = &derive;

    ptr->show();
    std::cout << "sizeof(Base)" << sizeof(base) << std::endl;
    std::cout << "sizeof(derive)" << sizeof(derive) << std::endl;
    std::cout << "ptr type: " << typeid(ptr).name() << std::endl;
    std::cout << "*ptr type: " << typeid(*ptr).name() << std::endl;

    std::cout << "++++++++++++++++++" << std::endl;
}
