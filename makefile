.PHONY: server client

default: server client

server: server.o networking.o
	@gcc -o server server.o networking.o -w -lm

client: client.o networking.o
	@gcc -o client client.o networking.o -w -lm

server.o: server.c
	@gcc -c server.c

client.o: client.c
	@gcc -c client.c

networking.o: networking.c
	@gcc -c networking.c

clean:
	@rm -f client
	@rm -f server
	@rm -f *.o