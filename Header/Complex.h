#pragma once
#ifndef __COMPLEX_H__
#define __COMPLEX_H__
#include <iostream>

class Complex {
public:
    Complex(int real = 0, int image = 0)
        :_real(real),
        _image(image)
    {}
    //成员函数+操作符重载
    Complex operator+(const Complex& src) {
        return Complex(this->_real + src._real,
            this->_image + src._image);
    }
    //前置++，与后置++重载的区别是多一个形参，用于区分
    //前置++可以返回引用，因为本身也修改了
    Complex& operator++() {
        ++_real; ++_image;
        return *this;
    }
    //后置++
    Complex operator++(int) {
        return Complex(_real++, _image++);
    }
    void operator+=(const Complex& src) { _real += src._real, _image += src._image; }
private:
    int _real;
    int _image;

friend
Complex operator+(const Complex&, const Complex&);
friend
std::ostream& operator<<(std::ostream&, const Complex&);
friend
std::istream& operator>>(std::istream&, Complex&);
};

/*
    S1: Complex com = 30 + Complex();
    S2: Complex com = Complex() + 30;
    s1语句实现需要重载全局运算符，并且全局无法访问private，需要加friend修饰
    s2语句实现需要重载类内运算符，实际上是对象调用operator+()函数，编译器会尝试将int强转为Complex，调用构造函数
*/
//全局的运算符重载
Complex operator+(const Complex& lSrc, const Complex& rSrc) { return Complex(lSrc._real + rSrc._real, lSrc._image + rSrc._image); }

//全局<<运算符重载
std::ostream& operator<<(std::ostream& cout, const Complex& src) {
    cout << "real:" << src._real << " image:" << src._image << std::endl;
    return cout;
}
//重载>>时，不能加const
std::istream& operator>>(std::istream& cin, Complex& src) {
    std::cout << "real:";
    cin >> src._real;
    std::cout << "image:";
    cin >> src._image;
    return cin;
}
#endif // !__COMPLEX_H__
