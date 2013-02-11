
CC=g++ -O2
LIBS=

all: Delegate.test.exe DelegateMulticast.test.exe
	Delegate.test.exe
	DelegateMulticast.test.exe

clean:
	del Delegate.test.exe DelegateMulticast.test.exe *.o *.cc *.s

Delegate.test.exe: Delegate.test.cpp Delegate.h static_va.h Makefile
	$(CC) -E -o $<.cc $< $(LIBS)
	$(CC) -S -o $<.s $< $(LIBS)
	$(CC) -o $@ $< $(LIBS)

DelegateMulticast.test.exe: DelegateMulticast.test.cpp DelegateMulticast.h Delegate.h static_va.h Makefile
	$(CC) -E -o $<.cc $< $(LIBS)
	$(CC) -o $@ $< $(LIBS)

.PHONY: all clean
