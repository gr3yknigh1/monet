MKDIR = mkdir -p
RM    = rm -rf

CC     = clang
CFLAGS = -g -Wall -Wpedantic -std=c11
AR     = ar rcs


BUILD_DIR = build
OBJS_DIR  = $(BUILD_DIR)/objs
LIBRARY   = $(BUILD_DIR)/libimage.a

CLI         = $(BUILD_DIR)/readimage
CLI_DIR     = cli
CLI_SOURCES = $(CLI_DIR)/*.c

SOURCES_DIR = src
INCLUDE_DIR = include

SOURCES     = $(wildcard $(SOURCES_DIR)/*.c)
HEADERS     = $(wildcard $(INCLUDE_DIR)/*.h)
OBJS        = $(patsubst $(SOURCES_DIR)/%.c, $(OBJS_DIR)/%.o, $(SOURCES))

default: all

all: $(LIBRARY) $(CLI)

release: CFLAGS=-O3 -Werror
release: clean
release: $(LIBRARY)

$(LIBRARY): $(OBJS)
	$(RM) $(LIBRARY)
	$(AR) $@ $<

$(CLI): $(LIBRARY)
	$(RM) $(CLI)
	$(CC) $(CFLAGS) $(CLI_SOURCES) $(OBJS) -I$(INCLUDE_DIR) -o $@

$(OBJS): $(BUILD_DIR) $(OBJS_DIR)

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
	@$(LIBRARY)

.PHONY: all, clean

