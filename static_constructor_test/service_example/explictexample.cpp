#include "test.h"

/// Our service
class ExplicitExample : public ServiceDevice
{
public:
	std::string getName();
};

std::string ExplicitExample::getName()
{
	return "ExplicitExample";
}

/// Our function that instantiates the service
ServiceDevice* createExplictExampleObject()
{
	return new ExplicitExample();
}



/// This is what the macro provides normally
class DummyClass
{
public:
	DummyClass();
};

DummyClass::DummyClass()
{
	registrationHandler("ExplicitExample", &createExplictExampleObject);
}

static DummyClass dc;

