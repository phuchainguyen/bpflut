CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -Iinclude
TARGET = bpflut
SRCDIR = src
BUILDDIR = build
BINDIR = bin

# Source files
SOURCES = $(SRCDIR)/main.c \
          $(SRCDIR)/utils/logger.c \
          $(SRCDIR)/utils/config.c \
          $(SRCDIR)/utils/file_utils.c \
          $(SRCDIR)/commands/init.c \
          $(SRCDIR)/commands/init_ios.c \
          $(SRCDIR)/commands/init_android.c \
          $(SRCDIR)/commands/build.c \
          $(SRCDIR)/commands/build_ios.c \
          $(SRCDIR)/commands/build_android.c \
          $(SRCDIR)/commands/publish.c \
          $(SRCDIR)/commands/publish_ios.c \
          $(SRCDIR)/commands/publish_android.c\
          $(SRCDIR)/commands/clean.c

# Object files (in build directory)
OBJECTS = $(SOURCES:$(SRCDIR)/%.c=$(BUILDDIR)/%.o)

.PHONY: all clean install uninstall dirs

all: dirs $(BINDIR)/$(TARGET)

# Create necessary directories
dirs:
	@mkdir -p $(BUILDDIR)/utils
	@mkdir -p $(BUILDDIR)/commands
	@mkdir -p $(BINDIR)

$(BINDIR)/$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@

# Pattern rule for object files
$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILDDIR) $(BINDIR)

install: $(BINDIR)/$(TARGET)
	cp $(BINDIR)/$(TARGET) /usr/local/bin/

uninstall:
	rm -f /usr/local/bin/$(TARGET)

# Debug target
debug: CFLAGS += -g -DDEBUG
debug: all
