#include "test.h"
#include "force_execute.h"

/// Our service
class NormalExample : public ServiceDevice
{
public:
	std::string getName();
};

std::string NormalExample::getName()
{
	return "NormalExample";

}

/// Our function that instantiates the service
ServiceDevice* createNormalExampleObject()
{
	return new NormalExample();
}



/// The "arg" here just needs to simply be unique
FORCE_EXECUTE(dummy_normal_example)
{
	std::string serviceName = "NormalExample";
	registrationHandler(serviceName, &createNormalExampleObject);
}


