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
    //��Ա����+����������
    Complex operator+(const Complex& src) {
        return Complex(this->_real + src._real,
            this->_image + src._image);
    }
    //ǰ��++�������++���ص������Ƕ�һ���βΣ���������
    //ǰ��++���Է������ã���Ϊ����Ҳ�޸���
    Complex& operator++() {
        ++_real; ++_image;
        return *this;
    }
    //����++
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
    s1���ʵ����Ҫ����ȫ�������������ȫ���޷�����private����Ҫ��friend����
    s2���ʵ����Ҫ���������������ʵ�����Ƕ������operator+()�������������᳢�Խ�intǿתΪComplex�����ù��캯��
*/
//ȫ�ֵ����������
Complex operator+(const Complex& lSrc, const Complex& rSrc) { return Complex(lSrc._real + rSrc._real, lSrc._image + rSrc._image); }

//ȫ��<<���������
std::ostream& operator<<(std::ostream& cout, const Complex& src) {
    cout << "real:" << src._real << " image:" << src._image << std::endl;
    return cout;
}
//����>>ʱ�����ܼ�const
std::istream& operator>>(std::istream& cin, Complex& src) {
    std::cout << "real:";
    cin >> src._real;
    std::cout << "image:";
    cin >> src._image;
    return cin;
}
#endif // !__COMPLEX_H__
