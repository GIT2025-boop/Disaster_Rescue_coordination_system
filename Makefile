CC = gcc
CFLAGS = -Wall -pthread -lrt
BUILD = build

all: main alert_manager rescue_coord monitoring

main:
	$(CC) $(CFLAGS) src/main.c -o $(BUILD)/main

alert_manager:
	$(CC) $(CFLAGS) src/alert_manager.c -o $(BUILD)/alert_manager

rescue_coord:
	$(CC) $(CFLAGS) src/rescue_coord.c -o $(BUILD)/rescue_coord

monitoring:
	$(CC) $(CFLAGS) src/monitoring.c -o $(BUILD)/monitoring

clean:
	rm -f $(BUILD)/*
