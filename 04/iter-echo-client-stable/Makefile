CC = gcc
CFLAGS = -g3

CLIENT = iter-echo-client-stable

all: $(CLIENT)

$(CLIENT): $(CLIENT).c
	$(CC) -o $@.out $^

clean:
	rm -rf $(CLIENT).out