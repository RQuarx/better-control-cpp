PROG_NAME = control

CXX = clang++
LD = clang++

DEPS = gdkmm-3.0 glibmm-2.68 gtkmm-3.0 giomm-2.68 libnm

CXX_DEPS = $(shell pkg-config --cflags $(DEPS))
LD_DEPS = $(shell pkg-config --libs $(DEPS))

CXXFLAGS = -std=c++23 -O1 -Wall -Wextra -Iinc $(CXX_DEPS)
LDFLAGS = -fuse-ld=lld $(LD_DEPS)

SRC = $(shell find src/ -name '*.cpp')
TARGET_DIR = target
TARGET = $(TARGET_DIR)/$(PROG_NAME)
OBJ_DIR = $(TARGET_DIR)/$(PROG_NAME).o
OBJ = $(patsubst src/%.cpp, $(OBJ_DIR)/%.cpp.o, $(SRC))

BIN_DIR = /usr/bin
APP_DIR = /usr/share/applications
NUM_CORES = $(shell nproc)

all:
	@echo "Run 'make compile' and 'make install' to install BetterControl"
$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

$(TARGET_DIR):
	@mkdir -p $(TARGET_DIR)

compile: $(TARGET_DIR) $(OBJ_DIR) $(OBJ)
	$(LD) $(LDFLAGS) -o $(TARGET) $(OBJ)

$(OBJ_DIR)/%.cpp.o: src/%.cpp | $(OBJ_DIR)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

install: compile
	cp $(TARGET) $(BIN_DIR)
	cp $(PROG_NAME).desktop $(APP_DIR)

clean:
	rm -rf $(TARGET_DIR)
	rm -f $(BIN_DIR)/$(PROG_NAME)
	rm -f $(APP_DIR)/$(PROG_NAME).desktop

.PHONY: all compile install clean

MAKEFLAGS += -j$(NUM_CORES)