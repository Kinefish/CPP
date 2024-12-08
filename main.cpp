#include <iostream>
class Test {
public:
    Test() {
        std::cout << "TEST() FUNC CALLING" << std::endl;
    }
    ~Test() {
        std::cout << "~TEST() FUNC CALLING" << std::endl;
    }
};
//int Test::_staticData = 0;
//int f1() {
//
//    Test t1;
//    int Test::* ptr = &Test::_data;
//    t1.*ptr = 30;
//
//    int* staticPtr = &Test::_staticData;
//    *staticPtr = 30;
//
//    void (Test:: * pfunc)() = &Test::func;
//    (t1.*pfunc)();
//
//    void(*staticFunc)() = &Test::static_func;
//    (*staticFunc)();
//
//    std::cout << t1._data << std::endl << Test::_staticData << std::endl;
//    return 0;
//}

#include <vector>
int main() {
    std::vector<int> vec;
    for (int i = 0; i < 7; i++)
        vec.push_back(rand() % 100);

    for (auto it = vec.begin(); it != vec.end(); ++it)
        std::cout << *it << " ";
    
    return 0;
}