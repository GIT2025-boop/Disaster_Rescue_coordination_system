CC = gcc
CFLAGS = -Wall -lrt -lpthread

SRC = src
BUILD = build

all: $(BUILD)/c1 $(BUILD)/c2 $(BUILD)/c3 $(BUILD)/control

$(BUILD)/c1: $(SRC)/alert_manager.c
	mkdir -p $(BUILD)
	$(CC) $< -o $@ $(CFLAGS)

$(BUILD)/c2: $(SRC)/rescue_coord.c
	mkdir -p $(BUILD)
	$(CC) $< -o $@ $(CFLAGS)

$(BUILD)/c3: $(SRC)/monitoring.c
	mkdir -p $(BUILD)
	$(CC) $< -o $@ $(CFLAGS)

$(BUILD)/control: $(SRC)/control_center.c
	mkdir -p $(BUILD)
	$(CC) $< -o $@ $(CFLAGS)

clean:
	rm -rf $(BUILD)
	rm -f /tmp/rescue_pipe
	rm -f /dev/mqueue/disaster_queue
