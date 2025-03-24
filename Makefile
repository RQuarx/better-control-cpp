CXX = clang++
CXXFlags = -O3 -Wall -Wextra -std=c++23
Deps = -lgtkmm-3 -ljsoncpp -lnm

SRC = src/*.cpp
INC_DIR = inc/

OUT_DIR
OUT = $(OUT_DIR)/better-control

$(shell mkdir -p $(OUT_DIR))

all: $(OUT)

$(OUT): $(SRC)
	$(CXX) $(CXXFlags) $(Deps) $^ -o $@
clean:
	rm $(OUT)

.PHONY: all clean
