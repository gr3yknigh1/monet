MKDIR = mkdir -p
RM    = rm -rf

CC     = clang
CFLAGS = -g -Wall -std=c99

BUILD_DIR = build
OBJS_DIR  = $(BUILD_DIR)/objs
TARGET    = $(BUILD_DIR)/rimg

SOURCES_DIR = src
INCLUDE_DIR = include

SOURCES     = $(wildcard $(SOURCES_DIR)/*.c)
HEADERS     = $(wildcard $(INCLUDE_DIR)/*.h)
OBJS        = $(patsubst $(SOURCES_DIR)/%.c, $(OBJS_DIR)/%.o, $(SOURCES))

default: all

all: $(TARGET)

release: CFLAGS=-Wall -O3 -DNDEBUG
release: clean
release: $(TARGET)

$(TARGET): $(BUILD_DIR) $(OBJS_DIR) $(OBJS)
	$(RM) $(TARGET)
	$(CC) $(CFLAGS) $(OBJS) -o $@

$(OBJS_DIR)/%.o: $(SOURCES_DIR)/%.c $(INCLUDE_DIR)/**/%.h
	$(CC) $(CFLAGS) -c $< -o $@ -I $(INCLUDE_DIR)

$(OBJS_DIR)/%.o: $(SOURCES_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@ -I $(INCLUDE_DIR)

$(OBJS_DIR):
	$(MKDIR) $@

$(BUILD_DIR):
	$(MKDIR) $@

clean:
	$(RM) $(BUILD_DIR)

run:
	@$(TARGET)

.PHONY: all, clean

