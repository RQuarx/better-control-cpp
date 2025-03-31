CXX = clang++

TARGET_DIR = target

PREFIX ?= /usr
INSTALL_DIR = $(PREFIX)/share/applications
BIN_DIR = $(PREFIX)/bin

all:
	@echo "Run 'make build' and 'make install' to install BetterControl"

build:
	@rm -rf $(TARGET_DIR)
	@CXX=$(CXX) meson setup $(TARGET_DIR)
	@meson compile -C $(TARGET_DIR)

install: build
	@cp $(TARGET_DIR)/control $(BIN_DIR)
	@cp control.desktop $(INSTALL_DIR)

clean:
	@rm -rf $(TARGET_DIR)
	@rm -f $(BIN_DIR)/control
	@rm 0f $(INSTALL_DIR)/control.desktop

.PHONY: all build install clean