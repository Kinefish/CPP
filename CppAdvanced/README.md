1.����Ĺ��졢�����Լ���ʱ�������ֵ���ã������۵���move��forward����
-	�Ż�
	```
	//�Ż�ǰ
	String getString(const String str) {
		char* retStr = new char[sizeof(str._str) + 1];
		String tmpStr(retStr);
		return tmpStr;
	}
	int main() {
		String str1("abcd");
		String str2;
		str2 = getString(str1);		//�ֵ����˿�����ֵ
		return 0;
	}
	/*
	���ڵ�����
	1.ÿ����һ��retStr�������ڶ���newһ���ڴ������������ڴ�й¶
	2.��һ������������ʱ����ʱ ���� ���غ��� ��ջ֡�� �� ����һ����ʱ����
		getString()���ý�����������tmpStr
		�ص����÷��У�
		�����
		Stirng str = getStirng(src);
		���ٽ��п������죬��������ջ֡�ϵ���ʱ����
		��ˣ�cpp������������Ż�
		�������ʱ���󿽱�����һ���¶��󣬻ᱻ�Ż����¶���Ĺ���
		String str(src);
	3.�βν���û�������ã��ᵼ��ѹջ��ʱ���ֽ�����str�βεĹ���
	4.���ж���Ľ���ʱ������ʹ�ù����¶���ķ�ʽ���н��ܣ������Ǹ�ֵ
	*/
	//�Ż�
	String getString(const String& str) 
		return String(str._str);
	}
	int main() {
		String str1("abcd");
		String str2 = getString(str1);
		return 0;
	}
	```

-	�Ż�������ԭ��
	-	�β����������ô���
	-	��������ʱ���󷵻�
	-	���շ��ض���ʱ�������ù��췽ʽ����

-	������ֵ���ã��β��е���ֵ���ã�
	-	��һ������ռ���ⲿ��Դ��ʱ�򣬽��п�������ʱ��ԭ�ȵķ�ʽ�ǣ�newһ���¿ռ䣬����forloopһ������ֵ
	-	��ʱ��һ�������Ƿ�����ʱ�����ʱ�򣬺�������ɣ��ո�forloopһ������ֵ֮����������ʱ���󣬳������������Ŷ����ľ�������
	-	��ʵ��õİ취ʱ��������ⲿ��Դ���Ҿ�����
	-	����Ĺؼ��ǣ�**��ʱ������п�������Ϳ�����ֵ**��ʱ��������ʱ��������ԣ����ִ����ͻ����������������ʱռ�����ⲿ��Դ���ᵼ�¹���͸�ֵ��Ч�ʱ��
	
	-	cpp�������У�����ʱ��������ֵ
	`const String& str = String("abcd");`����ֵ������Ҫ��cosnt
	`String&& str = String("abcd");`����ֵ���ÿ���������ʱ����str����ֵ���ã���������һ����ֵ
-	�Ż�
```
/*
	ΪString������ֵ���õĿ�������Ϳ�����ֵ
	Ŀ����ֱ��ռ����ʱ������ⲿ��Դ��������ʱ�����ptr�ÿ�
*/
//����������ֵ����
String(String&& str) {
	_str = str._str;
	str._str = nullptr;
}
//������ֵ��ֵ����
String& operator=(Stirng&& str) {
	if(this == &str) return *this;
	delete[] _str;

	_str = str._str;
	str._str = nullptr;
	return *this;
}
```

-	������ʱ���󣬷�����ʱ���󣨿�����ʱ����
	-	����ʱ������Ϊ�βν��д��ݵ�ʱ�򣬱�����ƥ�������ֵ������Ϊ�βεĺ��������Ƚ�����ʱ����Ĺ���
	-	��������ʱ������¶�����и�ֵ��ʱ�򣬱�����������Ż�����ʱ����ֱ�����¶�����ڴ��Ͻ���**��ͨ����**
	-	�ɰ汾�ı��������ܻ��һ�����ض���Ĺ�����������±�����Ӧ�����Ż�����

-	������ʱ����
	- ����ʹ��`templete<typename _Ty> func(_Ty&& src) { ... std::forward<_Ty>(src)}`��������ֵ���ú���ֵ���õ�����ת��


		