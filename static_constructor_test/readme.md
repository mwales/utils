I created a macro that you can use to force execution of a function by simply
building in and linking in a source file.  No code will have to depend on this
file at all.

When no other files are compiled with it:

```
shell:~/utils/static_constructor_test/simple_example$ g++ test.cpp
shell:~/utils/static_constructor_test/simple_example$ ./a.out 
Hello World
```

This file shows how a static object's constructor gets called before main() function is called.

```
shell:~/utils/static_constructor_test/simple_example$ g++ test.cpp non_macro_example.cpp 
shell:~/utils/static_constructor_test/simple_example$ ./a.out 
Thank you for registering: explicit example!
Hello World
```

The regular_usage shows how you to the macro based on this feature to define a
special function this is called whenever it gets linked into an application.


```
shell:~/utils/static_constructor_test/simple_example$ g++ test.cpp non_macro_example.cpp regular_usage.cpp 
shell:~/utils/static_constructor_test/simple_example$ ./a.out 
Thank you for registering: explicit example!
Thank you for registering: macro use example
Hello World
```

One weird thing I did notice about this, and I'm not exactly sure as to why
this occurs, but the build order seems to matter.


```
shell:~/utils/static_constructor_test/simple_example$ g++ regular_usage.cpp test.cpp 
shell:~/utils/static_constructor_test/simple_example$ ./a.out 
Segmentation fault (core dumped)
```

I then also showed how you can create a Factory / Service Locator that has
way less dependencies than a traditonal pattern. Notice that test.cpp never
depends on any of the files that implement the classes / behaviors that they
call.

When the services are compiled in, they use the FORCE_EXECUTE macro to register
an object creation function with the Factory / Service Locator service.

A user can then call the application and supply a name for a service, if a
service of that name was regsitered, then the service will be called.

Here it is compiled without any service files / classes being registered:

```
shell:~/utils/static_constructor_test/service_example$ g++ test.cpp
shell:~/utils/static_constructor_test/service_example$ ./a.out NormalExample
Demonstration of factory that doens't depend on the objects it creates!
Pass in the names of the devices you want us to call service for
Couldn't find service: NormalExample
```

Now an example where I compile in 2 service classes

```
shell:~/utils/static_constructor_test/service_example$ g++ test.cpp explictexample.cpp normalexample.cpp 
shell:~/utils/static_constructor_test/service_example$ ./a.out NormalExample
Thank you for registering: ExplicitExample
Thank you for registering: NormalExample
Demonstration of factory that doens't depend on the objects it creates!
Pass in the names of the devices you want us to call service for
NormalExample executing it's service!
```


