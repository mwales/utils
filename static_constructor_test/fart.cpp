#include <iostream>
#include "test.h"




class DummyClass
{
public:
	DummyClass();


};


DummyClass::DummyClass()
{
	registrationHandler("fart");
}

static DummyClass dc;

