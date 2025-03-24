CXX = clang++
CXXFlags = -O3 -Wall -Wextra -std=c++23
Deps = -lgtkmm-3.0 -ljsoncpp -lnm -lglibmm-2.4

SRC = src/*.cpp
INC_DIR = inc/

OUT_DIR = target
OUT = $(OUT_DIR)/better-control

$(shell mkdir -p $(OUT_DIR))

all: $(OUT)

$(OUT): $(SRC)
	$(CXX) $(CXXFlags) $(Deps) -I$(INC_DIR) $^ -o $@
clean:
	rm $(OUT)

.PHONY: all clean
