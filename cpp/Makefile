CXX = g++
CXXFLAGS = -std=c++11 -Wall
LDFLAGS = -lSDL2_ttf

# Detect SDL2 installation path
UNAME := $(shell uname)
ifeq ($(UNAME), Darwin)
    # macOS
    SDL2_CFLAGS = $(shell sdl2-config --cflags 2>/dev/null || echo "-I/opt/homebrew/include -I/usr/local/include")
    SDL2_LDFLAGS = $(shell sdl2-config --libs 2>/dev/null || echo "-L/opt/homebrew/lib -L/usr/local/lib") $(LDFLAGS)
else
    # Linux
    SDL2_CFLAGS = $(shell sdl2-config --cflags)
    SDL2_LDFLAGS = $(shell sdl2-config --libs) $(LDFLAGS)
endif


TARGET = snake
SRC = snake.cpp
INSTALL_CMD = brew install sdl2 sdl2_ttf

# SDL2 check
check_sdl2:
	@echo "Checking for SDL2..."
	@if ! command -v sdl2-config >/dev/null 2>&1; then \
		echo "SDL2 not found"; \
		read -p "Do you want to install SDL2 now? (y/n): " yn; \
		if [ "$$yn" = "y" ]; then \
			echo "Installing SDL2..."; \
			$(INSTALL_CMD); \
			echo "SDL2 installed successfully"; \
		else \
			echo "SDL2 not installed. The build may fail"; \
		fi \
	else \
		echo "SDL2 detected"; \
	fi

    
# Build target
all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SDL2_CFLAGS) -o $(TARGET) $(SRC) $(SDL2_LDFLAGS)

clean:
	rm -f $(TARGET)

.PHONY: all clean

