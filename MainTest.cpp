#define _CRT_SECURE_NO_WARNINGS

//∂‘œÛ≥ÿ≤‚ ‘≥Ã–Ú
#include<iostream>
#include<string>
#include<thread>
using namespace std;
#include"MyObjectPool4.h"
#include"MyString.h"

class Int
{
private:
	int value;
public:
	Int(int x) :value(x) { cout << "Int(int)" << endl; }
	Int(int x, int y) :value(x + y) { cout << "Int(int, int)" << endl; }
	Int(int& a, const int& b, int&& c) :value(a + b + c) { cout << "Int(int&, const int&, int&&)" << endl; }
	~Int() {}
	void Print() const
	{
		cout << "value: " << value << endl;
	}
};

void funa(ObjectPool<Int>& pool)
{
	{
		auto pa = pool.acquire(1);
		if (pa) pa->Print();
	}
	{
		auto pb = pool.acquire(2);
		if (pb) pb->Print();
	}
}

void funb(ObjectPool<Int>& pool)
{
	{
		auto pa = pool.acquire(3);
		if (pa) pa->Print();
	}
	{
		auto pb = pool.acquire(4);
		if (pb) pb->Print();
	}
}

void func(ObjectPool<Int>& pool)
{
	{
		auto pa = pool.acquire(5);
		if (pa) pa->Print();
	}
	{
		auto pb = pool.acquire(6);
		if (pb) pb->Print();
	}
}

void fund(ObjectPool<Int>& pool)
{
	{
		auto pa = pool.acquire(7);
		if (pa) pa->Print();
	}
	{
		auto pb = pool.acquire(8);
		if (pb) pb->Print();
	}
}

int main()
{
	ObjectPool<Int> mypool;
	std::thread tha(funa, std::ref(mypool));
	std::thread thb(funb, std::ref(mypool));
	std::thread thc(func, std::ref(mypool));
	std::thread thd(fund, std::ref(mypool));

	tha.join();
	thb.join();
	thc.join();
	thd.join();
	return 0;
}

#if 0
int main()
{
	ObjectPool<Int> mypool;
	{
		auto pa = mypool.acquire(1);
		if(pa) pa->Print();
		auto pb = mypool.acquire(2);
		if (pb) pb->Print();
		auto pc = mypool.acquire(3);
		if (pc) pc->Print();
		cout << "Total object num: " << mypool.getObjectTotal() << endl;
		cout << "Idle object num: " << mypool.getIdleSize() << endl;
		cout << "Activate object num: " << mypool.getActivateObjSize() << endl;
		auto pd = mypool.acquire(4);
		if (pd) pd->Print();
		auto pe = mypool.acquire(5);
		if (pe) pe->Print();
		auto pf = mypool.acquire(6);
		if (pf) pf->Print();
		auto pg = mypool.acquire(7);
		if (pg) pg->Print();
	}
	cout << "Total object num: " << mypool.getObjectTotal() << endl;
	cout << "Idle object num: " << mypool.getIdleSize() << endl;
	cout << "Activate object num: " << mypool.getActivateObjSize() << endl;
	return 0;
}

int main()
{
	ObjectPool<MyString> mypool;
	char buff[100] = {};
	for (int i = 0; i < 26; ++i)
	{
		sprintf(buff, "lijian %c", i + 'a');
		auto xp = mypool.acquire(buff);
		xp->PrintString();
	}
	cout << "idle object size: " << mypool.getIdleSize() << endl;
	return 0;
}

int main()
{
	ObjectPool<MyString> mypool;
	{
		auto xp = mypool.acquire("lijian");
		xp->PrintString();
		
	}
	return 0;
}

int main()
{
	ObjectPool<Int> mypool;
	{
		int a = 1;
		int b = 2;
		auto xp = mypool.acquire(a);
		xp->Print();
		auto yp = mypool.acquire(a, b);
		yp->Print();
		auto zp = mypool.acquire(a, b, 3);
		zp->Print();
	}
	cout << "idle object size: " << mypool.getIdleSize() << endl;
	return 0;
}

template<class ... Args>
void func(Args&&... args)
{
	Int a(std::forward<Args>(args)...);
}

int main()
{
	int a = 10;
	const int b = 20;
	func(a);
	func(a, b);
	func(a, b, 30);
	return 0;
}

int main()
{
	func();
	func(1, 1.2);
	func(1, 1.2, "lijian");
	return 0;
}

template<class T, class U>
void func(T a, U b)
{
	cout << "T type: " << typeid(a).name() << endl;
	cout << "U type: " << typeid(b).name() << endl;
}

int main()
{
	int a = 1;
	double b = 1.2;
	func(&a, &b);
	return 0;
}

int main()
{
	ObjectPool<Int> mypool;
	auto p =  mypool.acquire();
	return 0;
}

int main()
{
	ObjectPool<string> mypool;
	cout << "Pool Idle Object Size: " << mypool.getIdleSize() << endl;
	{
		auto xp = mypool.acquire();
		(*xp) += "li hello";
		cout << (*xp) << endl;
		xp->clear();
		auto yp = mypool.acquire();
		(*yp) += "jian hello";  
		cout << (*yp) << endl;
		yp->clear();
	}
	cout << "Pool Idle Object Size: " << mypool.getIdleSize() << endl;
	{
		auto p = mypool.acquire();
		cout << (*p) << endl;
	}
	return 0;
}
#endif