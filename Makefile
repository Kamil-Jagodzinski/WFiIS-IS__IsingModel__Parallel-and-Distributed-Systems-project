CC = mpic++
# Lokalnie
# CFLAGS = -Wall -I/usr/bin/mpic++
# Na pracowni
# CFLAGS = -Wall -I/opt/nfs/mpich-4.0.1/bin
LDFLAGS = -L/opt/nfs/mpich-4.0.1/lib -lmpi
TARGET = ising.out
SRC = main.cpp utils.cpp

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)
