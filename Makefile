CC = gcc
CFLAGS = -Wall -g
SERVER = server
CLIENT = client
MAIN = main.exe  # Ana program (main.c)

# Derleme hedefleri
all: $(SERVER) $(CLIENT) $(MAIN)

# Sunucu derleme
$(SERVER): src/server.c
	$(CC) $(CFLAGS) -o bin/$(SERVER) src/server.c

# İstemci derleme
$(CLIENT): src/client.c
	$(CC) $(CFLAGS) -o bin/$(CLIENT) src/client.c

# Ana program (main.c) derleme
$(MAIN): src/main.c
	$(CC) $(CFLAGS) -o bin/$(MAIN) src/main.c

# Temizleme
clean:
	rm -f bin/$(SERVER) bin/$(CLIENT) bin/$(MAIN)