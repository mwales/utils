#include <iostream>
#include <string>
#include <map>
#include "test.h"

/** 
 * This is my "factory".  It's a map of service names and the functions that
 * will instantiate them
 */
std::map<std::string, createServiceFunction> g_possibleDevices;

void registrationHandler(std::string name, createServiceFunction creationMethod)
{
	std::cout << "Thank you for registering: " << name << std::endl;
	g_possibleDevices[name] = creationMethod;
}

int main(int argc, char** argv)
{
	printf("Demonstration of factory that doens't depend on the objects it creates!\n");
	printf("Pass in the names of the devices you want us to call service for\n");
	
	for(int i = 1; i < argc; i++)
	{
		auto curDev = g_possibleDevices.find(argv[i]);
		if (curDev != g_possibleDevices.end())
		{
			createServiceFunction csf = curDev->second;
			ServiceDevice* sd = csf();
			sd->doServiceThing();
			delete sd;
		}
		else
		{
			std::cout << "Couldn't find service: " << argv[i] << std::endl;
		}
	}
	return 0;
}
