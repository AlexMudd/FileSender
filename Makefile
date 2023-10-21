CC=gcc
CFLAGS=-Wall

BIN=server.exe client.exe

all: clean $(BIN)

%.exe:%.c
	@echo "Build $@"
	@$(CC) $(CFLAGS) -o $@ $<
	
clean:
	@rm -fv *.exe
