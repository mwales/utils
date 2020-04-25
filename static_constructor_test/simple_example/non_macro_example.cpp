#include "test.h"

class ExplicitExampleClass
{
public:
	ExplicitExampleClass();
};


ExplicitExampleClass::ExplicitExampleClass()
{
	registrationHandler("explicit example!");
}

static ExplicitExampleClass eec;

