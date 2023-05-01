CC = mpic++
CFLAGS = -Wall -I/usr/bin/mpic++
TARGET = myprogram
SRC = main.cpp utils.cpp

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)