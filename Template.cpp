/*
    函数模板
*/
template <typename T>
bool compare(T a, T b) {
    return a < b;
}

/*
    模板特例化
*/
#include <string.h>
template<>
bool compare<const char*>(const char* str_a, const char* str_b) {
    return strcmp(str_a, str_b) > 0;
}

/*
    普通函数
*/
bool compare(const char* str_a, const char* str_b) {
    return strcmp(str_a, str_b);
}

/*
    普通函数跟模板函数不是重载关系，函数名并不相同
    编译器默认优先调用普通函数 compare()，这样不必要进行实参类型推演，
    除非用
    compare<const char*>调用

    模板不能分文件（.c）调用，因为模板需要实例化之后才能进行编译，分文件编译导致link 
    建议写在.h中，#include是预编译阶段，cp paste

*/
/*
    告诉编译器指定实例化，可以在编译阶段编译
*/
template bool compare<float>(float, float);

