#pragma once
#include <string.h>
#include <iostream>
#ifndef __STRING_H__
#define __STRING_H__

class String {
public:
    String(const char* str);
    String(const String&);
    ~String();
    String& operator=(const String&);
    bool operator>(const String&) const;
    int length() const { return strlen(_data); }
    //�������޸�
    const char& operator[](int idx) const { return _data[idx]; }
    //�����޸�
    char& operator[](int idx) { return _data[idx]; }
    //c���char*�ַ���
    const char* c_str() const { return _data; }

    /*
        String���ڲ��������������ڲ���ʵ��
        ++() * !=
        ����Ҫʵ������
        ��Ϊ_pָ�����һ�����ڵ��ڴ�
    */
    class iterator {
    public:
        iterator(char* p = nullptr) : _p(p) {}
        iterator& operator++() { ++_p; return *this; }
        bool operator!=(const iterator& src) const { return _p != src._p; }
        char& operator*() { return *_p; }
    private:
        char* _p;
    };
    iterator begin() const { return iterator(_data); }
    iterator end() const { return iterator(_data + length()); }

private:
    char* _data;
friend
String operator+(const String&, const String&);
friend
std::ostream& operator<<(std::ostream& cout, const String&);
};

std::ostream& operator<<(std::ostream& cout, const String& src) {
    cout << src._data;
    return cout;
}

String operator+(const String& lStr, const String& rStr) {
    /*
    * method-1
    û�м�ʱdelete�����ᵼ���ڴ�й©�����Ҳ�����+�����壬���ص���һ����ʱ���󣬲��ǹ���һ���µĶ�����Ȼc11֮��
    ���������ܻ��return ��ʱ�������Ż������ǲ�����+�����壬����������ʹ��

    char* newStr = new char[strlen(lStr._data) + strlen(rStr._data) + 1];
    strcpy(newStr, lStr._data);
    strcat(newStr, rStr._data);
    return String(newStr);

    */
    /*method-2
    �ֶ�new��delete������Ч��ʮ�ֵ���,����new��delete

    char* newStr = new char[strlen(lStr._data) + strlen(rStr._data) + 1];
    strcpy(newStr, lStr._data);
    strcat(newStr, rStr._data);
    String ret(newStr);
    delete[] newStr;
    return ret;
    */

    /*
    method-3,1��new��delete*/

    String ret("");
    ret._data = new char[strlen(lStr._data) + strlen(rStr._data) + 1];
    strcpy(ret._data, lStr._data);
    strcat(ret._data, rStr._data);
    return ret;
}

inline String::String(const char* str = nullptr) {
    if (str == nullptr) {
        _data = new char[1];
        *_data = '\0';
        return;
    }
    _data = new char[strlen(str) + 1];
    strcpy(_data, str);
}

inline String::String(const String& src) {
    _data = new char[strlen(src._data) + 1];
    strcpy(_data, src._data);
}

inline String& String::operator=(const String& src) {
    if (this == &src) return *this;
    delete[] _data;

    _data = new char[strlen(src._data) + 1];
    strcpy(_data, src._data);
    return *this;
}

inline bool String::operator>(const String& src) const
{
    return strcmp(_data, src._data) > 0;
}

inline String::~String() {
    delete[] _data;
    _data = nullptr;
}
#endif // !__STRING_H__
