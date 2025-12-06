CC = gcc
CFLAGS = -Wall -g

# 目標檔案
OBJS = main.o system.o scheduler.o queue.o

# 最終執行檔名稱
TARGET = sim

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

main.o: main.c task.h system.h scheduler.h queue.h
	$(CC) $(CFLAGS) -c main.c

system.o: system.c system.h task.h queue.h scheduler.h
	$(CC) $(CFLAGS) -c system.c

scheduler.o: scheduler.c scheduler.h task.h queue.h system.h
	$(CC) $(CFLAGS) -c scheduler.c

queue.o: queue.c queue.h task.h
	$(CC) $(CFLAGS) -c queue.c

clean:
	rm -f *.o $(TARGET)