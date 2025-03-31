CXX = clang++
CXX_LD = lld

BuildDir = target

Prefix ?= /usr
InstallDir = $(Prefix)/share/applications
BinDir = $(Prefix)/bin

all:
	@echo "Run 'make build' and 'make install' to install BetterControl"

build:
	if [ ! -d "$(BuildDir)" ]; then \
		CXX=$(CXX) CXX_LD=$(CXX_LD) meson setup $(BuildDir); \
	fi

	@meson compile -C $(BuildDir)

install: build
	@cp $(BuildDir)/control $(BinDir)
	@cp control.desktop $(InstallDir)

clean:
	@rm -rf $(BuildDir)
	@rm -f $(BinDir)/control
	@rm 0f $(InstallDir)/control.desktop

.PHONY: all build install clean