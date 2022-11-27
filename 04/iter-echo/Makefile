CC = gcc
CFLAGS = -g3

SERVER = iter-echo-server
CLIENT = iter-echo-client

all: $(SERVER) $(CLIENT)

clean:
	rm -rf $(SERVER) $(CLIENT)

$(SERVER): $(SERVER).c
	$(CC) $(CFLAGS) -o $@.out $^

$(CLIENT): $(CLIENT).c
	$(CC) $(CFLAGS) -o $@.out $^

.PHONY: all, clean