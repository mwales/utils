
test_app: test.o webapi.o
	gcc -g test.o webapi.o -o test_app

%.o: %.c
	gcc -c -g $< -o $@

webapi.o: webapi.c
test.o: test.c webapi.c

clean:
	rm test_app
	rm *.o
