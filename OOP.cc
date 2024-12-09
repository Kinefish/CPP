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

/*
    ��Щ��������ʵ�ֳ��麯����
    
    �麯������:
    �����ڴ�->vptr->vftable->�麯����ַ

    ���캯����ɣ�����Ŵ��ڣ����Թ��캯������ʵ�ֳ��麯��������{}��ȫ�Ǿ�̬�󶨣����ᷢ����̬��
    ���캯�������µ���virtual function()��Ҳ�Ǿ�̬��
    
    static function()Ҳ����ʵ�ֳ�virtual����Ϊstatic����������

    ����������
    ʲôʱ�򽫻������������ʵ�ֳ� �麯�� virtual ~Base(){}
    Base* ptr = new Derive();��ʱ��Ҳ���ǻ���ָ��ָ�� �� �ϵ�������
    ����delete ptr��ʱ������Base�����麯��->Base::vftable->Derive::vftable->�Զ�virtual ~Derive()-> *ptr == class Derive
    -> delete ptr�����ҵ�~Derive() -> �����ڴ�й¶

    Base b;
    Derive* ptr = (Derive*)&b;
    ptr->show();
    ��̬�󶨣�����b�ڴ���ֻ��Base������˵��õ���Base::show()

*/

/*
    ��̬

    ��̬��̬
    ����ʱ�Ķ�̬���������أ���ͬһ���������ڣ���ģ�壬�������Ǿ�̬��

    ��̬��̬
    ͨ������ָ��(����)ָ�������࣬���ҵ�����Ӧ��virtual function���ͻᷢ����̬��
    ���ҵ��������vfptr�������ҵ�vftable�����������า�ǵ�virtual function
*/

/*
    Base������Ƴ�virtual function() = 0;���ǳ����࣬һ�㽫Base����Ƴɳ�����
    �����಻��ʵ�����������Զ���ָ�루���ã������ڶ�̬��̬
*/

/*
    Base:
        virtual show(int i = 10)
    Derive:
        virtual show(int i = 20)
    ������ָ�루���ã�ָ��������ʱ������׶α�����ֻ��ѹ��Base���show()�β��б����ֻ�ܷ��ʵ�Base���Ĭ��ֵ�����õ���Ȼ���������show
*/

/*
    ���ֱ���ʱ�ķ���Ȩ�� �� ����ʱ�ķ���Ȩ��
    Base* ptr = new Derive();
    ptr->show();
    ����׶Σ�
        ��������ptr��Base*���ͣ�public: Base::show()����ͨ�����룬���н׶��Ƕ�̬�󶨣����õ���ʵ��Derive::show()����ʱ
        private Derive::show()Ҳ���Ե��ã���Ϊ���õ�ַ����
        private: Base::show()������ͨ�����룬������������
    
*/
/*
    ʲôʱ��ִ��vfptr <- &vftable ?
    ���캯��ִ��ǰ������stack frame��ִ��
*/
#include <iostream>
#include <string>
class Animal {
public:
    Animal(const std::string& name)
        :_name(name)
    {}
    virtual void bark() const = 0; //��ʱAnimal�ǳ�����
protected:
    std::string _name;
};

class Cat : public Animal {
public:
    Cat(const std::string& name)
        :Animal(name)
    {}
    void bark() const { std::cout << _name << " miao miao miao!" << std::endl; }
};
class Dog : public Animal {
public:
    Dog(const std::string& name)
        :Animal(name)
    {}
    void bark() const { std::cout << _name << " wang wang wang!" << std::endl; }
};

/*
    ���ö�̬
    ����ָ�루���ã�ָ�������࣬����virtual function��ʱ�򣬽��ж�̬�󶨣��ҵ�animal��vfptr-> vftable -> ����������ĸ��Ƿ���
*/
static void bark(const Animal& animal) { animal.bark(); }

static void test() {
    Cat cat("è��");
    Dog dog("����");
    bark(cat);  // è�� miao miao miao!
    bark(dog);  // ���� wang wang wang!
    std::cout << "---------pointer----------" << std::endl;
    
    /*
        ͨ������cat��dog��vfptr������vftable�ǶԷ��ģ���˶�̬�󶨺󣬵��õ�vftable�е�virtual bark()��ʵ�ǶԷ���
    */
    Animal* ptr1 = &cat;
    Animal* ptr2 = &dog;

    int* pp1 = (int*)ptr1;
    int* pp2 = (int*)ptr2;
    int tmp = pp1[0];
    pp1[0] = pp2[0];
    pp2[0] = tmp;

    ptr1->bark();
    ptr2->bark();

}

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
