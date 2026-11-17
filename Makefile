client: smtp.c client.c tls.c
	gcc -g -O0 -Wall -ggdb  client.c smtp.c -o client -lssl -lcrypto

clean:
	rm client