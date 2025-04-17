

# For Ubuntu, you will need to install the following package
# libmbedtls-dev

CFLAGS=-lmbedtls -lmbedx509 -lmbedcrypto

test_app: test.o webapi.o
	gcc -g test.o webapi.o -o test_app $(CFLAGS)

%.o: %.c
	gcc -c -g $< -o $@ $(CFLAGS)

webapi.o: webapi.c
test.o: test.c webapi.c

clean:
	rm test_app
	rm *.o
