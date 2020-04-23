I created a macro that you can use to force execution of a function by simply
building in and linking in a source file.  No code will have to depend on this
file at all.

'''
g++ test.cpp
./a.cout
Hello World
'''

But now I will include a file with the FORCE_EXECUTE macro, and it will call
the registration function within test.cpp without changing anything about
test.cpp, which has a single line main function.

'''
g++ test.cpp poop.cpp
./a.out
Thank you for registering: poop
Hello World
'''

The file fart.cpp has a more explicit example of what the macro does.
