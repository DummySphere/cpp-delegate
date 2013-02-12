cpp-delegate
============

Delegate and Multicast Delegate C++ classes.

* [Sources](#sources)
* [Features](#features)
* [Documentation](#documentation)
* [ToDo](#todo)
* [History](#history)
* [License](#license)


Sources
-------

* _static_va.h_: Generic macros to expand templates with variable argument count
* _Delegate.h_: Callback class and default behaviors (Empty, Function, Method)
* _DelegateMulticast.h_: Multicast callback class


Features
--------

Provide a C++ callback class which handle functions and methods (can be extended to handle functors too).

Here is a list of concerns that have been take care of when writing the library:

* Fast enough (no allocation in most cases)
* Easy to use / No obscure syntax / Keep code readability
* Can be stored in vector, map, etc.
* No dependency with non-standard library
* No platform-dependant or compiler-dependant trick
* No need for C++11


Documentation
-------------

### Delegate

	#include "Delegate.h"
	
	void MyFunc(int someArg)
	{
		printf("MyFunc called with someArg = %d\n", someArg);
	}
	
	class MyClass
	{
	public:
		void MyMethod(int someArg)
		{
			printf("MyClass::MyMethod called with someArg = %d\n", someArg);
		}
	};
	
	int main(int argc, char **argv)
	{
		MyClass myObject;
		
		typedef Delegate::Callback<void, int> MyCallbackType; // first template param is the return type
		
		MyCallbackType cb1(&MyFunc);
		MyCallbackType cb2(&myObject, &MyClass::MyMethod);
		
		if(cb1)
			cb1(100);
		if(cb2)
			cb2(200);
		
		return 0;
	}

Some code snippets:

	MyCallbackType cb1(&MyFunc);
	MyCallbackType cb1 = &MyFunc;
	MyCallbackType cb1 = Delegate::Function<void, int>(&MyFunc);
	MyCallbackType cb1 = Delegate::Auto(&MyFunc);
	
	MyCallbackType cb2(&myObject, &MyClass::MyMethod);
	MyCallbackType cb2 = Delegate::Function<void, int>(&MyFunc);
	MyCallbackType cb2 = Delegate::Auto(&MyFunc);
	
	MyCallbackType cb2(cb1);
	MyCallbackType cb2 = cb1;
	
	if(cb1)
		cb1(10);
	if(cb2 && !cb1)
		cb2(20);
	if(cb2 != cb1)
		cb2(30);
	if(cb2 == cb1)
		cb2(40);
	if(cb2 < cb1)
		cb2(50);

### Multicast Delegate

	#include "DelegateMulticast.h"

	void MyFunc(int someArg)
	{
		printf("MyFunc called with someArg = %d\n", someArg);
	}
	
	class MyClass
	{
	public:
		void MyMethod(int someArg)
		{
			printf("MyClass::MyMethod called with someArg = %d\n", someArg);
		}
	};
	
	int main(int argc, char **argv)
	{
		MyClass myObject;
		
		Delegate::Multicast<int> myEvent; // !!! no return type as first template param !!!
		
		myEvent(1); // does nothing
		
		myEvent += &MyFunc;
		myEvent += Delegate::Auto(&myObject, &MyClass::MyMethod);
		myEvent(2);
		
		myEvent -= Delegate::Auto(&myObject, &MyClass::MyMethod);
		myEvent(3);
		
		return 0;
	}


ToDo
----

Nothing to do for now.

Please report bugs or suggestions ...


History
-------

### Version 0.8 (2013-02-11)

* Create a namespace Delegate
* Separate the Callback class from the behaviors (Empty, Function, Method)
* First public version


License
-------

Copyright (c) 2007-2013 Julien Duminil  
This project is released under the [MIT License](http://opensource.org/licenses/MIT).
