// g++ -o DelegateMulticast.test.exe DelegateMulticast.test.cpp

#include "DelegateMulticast.h"
#include <stdio.h>
#include <stdlib.h>

void TestFunc(int arg1)
{
	printf("TestFunc called with arg1 = %d\n", arg1);
}

class StaticClass
{
public:
	static void StaticTestFunc(int arg1)
	{
		printf("StaticClass::TestFunc called with arg1 = %d\n", arg1);
	}
	static int data;
};

class SimpleClass
{
public:
	void SimpleTestMethod(int arg1)
	{
		printf("SimpleClass::TestMethod called with arg1 = %d\n", arg1);
	}
	int data;
};

int main(int argc, char **argv)
{
	SimpleClass simpleObject;
	
	Delegate::Multicast<int> myEvent;
	
	myEvent(1);
	printf("\n");
	
	myEvent += &TestFunc;
	myEvent += &StaticClass::StaticTestFunc;
	myEvent += Delegate::Auto(&simpleObject, &SimpleClass::SimpleTestMethod);
	myEvent(10);
	printf("\n");
	
	myEvent -= Delegate::Auto(&simpleObject, &SimpleClass::SimpleTestMethod);
	myEvent(100);
	printf("\n");
	
	return 0;
}
