// g++ -o Delegate.test.exe Delegate.test.cpp

#include "Delegate.h"

#include <stdio.h>
#include <stdlib.h>
#include <vector>

void TestFunc(int arg1)
{
	printf("TestFunc called with arg1 = %d", arg1);
}

class StaticClass
{
public:
	static void StaticTestFunc(int arg1)
	{
		printf("StaticClass::TestFunc called with arg1 = %d", arg1);
	}
	static int data;
};

class SimpleClass
{
public:
	void SimpleTestMethod(int arg1)
	{
		printf("SimpleClass::TestMethod called with arg1 = %d", arg1);
	}
	int data;
};

class OtherSimpleClass
{
public:
	void OtherSimpleTestMethod(int arg1)
	{
		printf("OtherSimpleClass::TestMethod called with arg1 = %d", arg1);
	}
	int data;
};

class MultiClass : public SimpleClass, public OtherSimpleClass
{
public:
	void MultiTestMethod(int arg1)
	{
		printf("MultiClass::TestMethod called with arg1 = %d", arg1);
	}
	int data;
};

class VirtualClass : public virtual SimpleClass
{
public:
	void VirtualTestMethod(int arg1)
	{
		printf("VirtualClass::TestMethod called with arg1 = %d", arg1);
	}
	int data;
};

class UnknownClass;

/////////////////////
typedef void (*FuncPtr)(int);
typedef Delegate::Helpers::BufferInPlace<12> Buffer;
void test_buffer()
{
	Buffer data;
	data.construct<FuncPtr>(TestFunc);
	data.destruct<FuncPtr>();
}
//////////////////////

typedef Delegate::Callback<void, int> TestCb;

void test(const char *_name, TestCb _cb)
{
	static int i = 0;
	++i;
	
	printf("[%d] %s: ", i, _name);
	if(_cb)
		_cb(i);
	else
		printf("(empty)");
	printf("\n");
}

int main(int argc, char **argv)
{
	printf("int: %d\n", sizeof( (int)0) );
	printf("data ptr: %d\n", sizeof( (void *)0) );
	printf("function ptr: %d\n", sizeof(&TestFunc) );
	printf("method ptr (simple): %d\n", sizeof(void (SimpleClass::*)() ) );
	printf("method ptr (multi): %d\n", sizeof(void (MultiClass::*)() ) );
	printf("method ptr (virtual): %d\n", sizeof(void (VirtualClass::*)() ) );
	printf("method ptr (unknown): %d\n", sizeof(void (UnknownClass::*)() ) );
	printf("\n");

	SimpleClass simpleObject;
	MultiClass multiObject;
	VirtualClass virtualObject;
	
	test("null", TestCb() );
	test("func", Delegate::Function<void, int>(&TestFunc) );
	test("staticFunc", Delegate::Function<void, int>(&StaticClass::StaticTestFunc) );
	test("simpleMethod", Delegate::Method<SimpleClass, void, int>(&simpleObject, &SimpleClass::SimpleTestMethod) );
	test("castMethod", Delegate::Method<OtherSimpleClass, void, int>( (OtherSimpleClass *)&multiObject, &OtherSimpleClass::OtherSimpleTestMethod) );
	test("multiMethod", Delegate::Method<MultiClass, void, int>(&multiObject, &MultiClass::MultiTestMethod) );
	test("virtualMethod", Delegate::Method<VirtualClass, void, int>(&virtualObject, &VirtualClass::VirtualTestMethod) );
	
	test("shortFunc", TestCb(&TestFunc) );
	test("shortMethod", TestCb(&simpleObject, &SimpleClass::SimpleTestMethod) );
	
	test("autoFunc", Delegate::Auto(&TestFunc) );
	test("autoMethod", Delegate::Auto(&simpleObject, &SimpleClass::SimpleTestMethod) );

	printf("\n");
	
	return 0;
}
