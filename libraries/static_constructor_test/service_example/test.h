#pragma once

#include <string>
#include <iostream>

class ServiceDevice
{
public:
	
	virtual std::string getName() = 0;
	
	virtual void doServiceThing()
	{
		std::cout << getName() << " executing it's service!" << std::endl;
	};
};


typedef ServiceDevice* (*createServiceFunction)();

// Call this to register your device
void registrationHandler(std::string serviceName, createServiceFunction csf);



