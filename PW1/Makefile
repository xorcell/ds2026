all: clean server client

test: clean
	mkdir -p tests/{server_test,client_test}
	gcc -Wall -pthread -g3 server.c -o tests/server_test/server
	gcc -Wall -g3 client.c -o tests/client_test/client

server: server.c
	gcc -Wall -pthread server.c -o server

client: client.c
	gcc -Wall client.c -o client

clean:
	rm -f server client 

.PHONY: all clean test
