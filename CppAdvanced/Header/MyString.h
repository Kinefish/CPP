#include <string.h>
#include <iostream>

#ifndef __MY_STRING_H__
#define __MY_STRING_H__
class MyString {
public:
    MyString(const char* str = nullptr) {
        std::cout << "MyString(const char*)" << std::endl;
        if (!str) {
            _str = new char[1];
            *_str = '\0';
            return;
        }
        _str = new char[strlen(str) + 1];
        strcpy(_str, str);
    }
    //��ֵ���õĿ�������
    MyString(const MyString& str) {
        std::cout << "MyString(const MyString& str)" << std::endl;
        _str = new char[strlen(str._str) + 1];
        strcpy(_str, str._str);
    }
    //��ֵ���õĿ�������
    MyString(MyString&& str) {
        std::cout << "MyString(MyString&& str)" << std::endl;
        _str = str._str;
        str._str = nullptr;
    }
    //��ֵ���õĿ�������
    MyString& operator=(const MyString& str) {
        /*
        �Ż�
        ͨ����������һ��tmpStr��ͬʱ���о���Դ���ͷź�����Դ���滻
        if(this != &str) {
            MyString tmpStr(str);
            char* instead_str = tmpStr._str;
            tmpStr._str = _str;
            _str = instead_str;
        }
        return *this;
        */
        std::cout << "operator=(const MyString& str)" << std::endl;
        if (this == &str) return *this;
        delete[] _str;
        _str = new char[strlen(str._str) + 1];  //���throw bad_alloc()���ᵼ�¶�ʧ_str�����Ի����Ż��İ취
        strcpy(_str, str._str);
        return *this;
    }
    //��ֵ���õĿ�������
    MyString& operator=(MyString&& str) {
        std::cout << "operator=(MyString&& str)" << std::endl;
        if (this == &str) return *this;
        delete[] _str;
        _str = str._str;
        str._str = nullptr;
        return *this;
    }
    ~MyString() {
        std::cout << "~MyString()" << std::endl;
        delete[] _str;
        _str = nullptr;
    }
    const char* c_str() const { return _str; }
private:
    char* _str;

    friend std::ostream& operator<<(std::ostream& cout, const MyString& str);
    friend MyString operator+(const MyString& lstr, const MyString& rstr);
};

std::ostream& operator<<(std::ostream& cout, const MyString& str) {
    cout << str._str;
    return cout;
}
MyString operator+(const MyString& lstr, const MyString& rstr) {
    MyString tmpStr;
    tmpStr._str = new char[strlen(lstr._str) + strlen(rstr._str) + 1];
    strcpy(tmpStr._str, lstr._str);
    strcat(tmpStr._str, rstr._str);
    return tmpStr;
}
inline static MyString getString(const MyString& str) {
    const char* retStr = str.c_str();
    MyString tmpStr(retStr);
    return tmpStr;
}

#include "../../CppBase/Header/Vector.h"

namespace MyStringTest {
    static void test01() {
        MyString str1("abcd");
        MyString str2 = getString(str1);
        
    }
    static void test02() {
        MyString str1("abcd");


        Vector<MyString> vec;
        std::cout << "-------------" << std::endl;
        vec.push_back(str1);
        vec.push_back(MyString("abccc"));
        std::cout << "-------------" << std::endl;

    }
}

#endif // ! __MY_STRING_H__
