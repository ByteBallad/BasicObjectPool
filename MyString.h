#ifndef MYSTRING_H
#define MYSTRING_H

#include<string.h>
#include<stdlib.h>
#include<iostream>
using namespace std;

class MyString
{
private:
	struct StrNode
	{
		int ref;
		int len;
		int capa;
		char data[0];
	};
private:
	StrNode* pstr;
	static StrNode* GetStrNode(int total)
	{
		StrNode* s = (StrNode*)malloc(sizeof(StrNode) + sizeof(char) * total);
		if (nullptr == s) exit(EXIT_FAILURE);
		memset(s, 0, sizeof(StrNode) + sizeof(char) * total);
		return s;
	}
	static void Freenode(StrNode* p)
	{
		free(p);
	}
public:
	MyString(const char* p = nullptr) :pstr(nullptr)
	{
		if (nullptr != p)
		{
			int len = strlen(p);
			int total = len * 2;
			pstr = GetStrNode(total);
			pstr->ref = 1;
			pstr->len = len;
			pstr->capa = total - 1;
			strcpy(pstr->data, p);
		}
	}
	MyString(const MyString& s) :pstr(s.pstr)
	{
		if (pstr != nullptr)
		{
			pstr->ref += 1;
		}
	}
	MyString(MyString&& s) :pstr(s.pstr)
	{
		s.pstr = nullptr;
	}
	MyString& operator=(const MyString& s)
	{
		if (this == &s || this->pstr == s.pstr) return *this;
		MyString(s).swap(*this);
		return *this;
	}
	MyString& operator=(MyString&& s)
	{
		if (this == &s) return *this;
		MyString(std::move(s)).swap(*this);
		return *this;
	}
	~MyString()
	{
		if (pstr != nullptr && --pstr->ref == 0)
		{
			free(pstr);
		}
		pstr = nullptr;
	}
	ostream& operator<<(ostream& out) const
	{
		if (nullptr != pstr)
		{
			out << pstr->data;
		}
		return out;
	}
	void PrintString() const
	{
		if (nullptr != pstr)
		{
			cout << "Ref: " << pstr->ref << endl;
			cout << "Len: " << pstr->len << endl;
			cout << "Capa: " << pstr->capa << endl;
			cout << "String: " << pstr->data << endl;
		}
	}
	void swap(MyString& s)
	{
		std::swap(pstr, s.pstr);
	}
};
#endif