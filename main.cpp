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
#include <chrono>
#include "Header/LinkQueue.h"

int main() {

    LinkQue<int> linkQue;
    // 获取开始时间
    auto start = std::chrono::high_resolution_clock::now();

    // 执行需要计时的代码
    for (int i = 0; i < 100000; ++i) {
        linkQue.push(i);
        linkQue.pop();
    }

    // 获取结束时间
    auto end = std::chrono::high_resolution_clock::now();

    // 计算运行时间（单位：毫秒）
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    std::cout << "Time taken: " << duration << " ms" << std::endl;
    return 0;
}