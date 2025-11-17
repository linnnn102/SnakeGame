CXX = g++
CXXFLAGS = -std=c++11 -Wall
LDFLAGS = -lSDL2_ttf -lSDL2_image

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
SRC = src/snake.cpp src/generators.cpp

# Default target
all: $(TARGET)

# Check if all necessary libraries are installed
check:
	@echo "=== Checking Required Libraries ==="
	@echo ""
	@echo "Checking for SDL2..."
	@if command -v sdl2-config >/dev/null 2>&1; then \
		echo "SDL2 is installed"; \
		sdl2-config --version; \
	else \
		echo "SDL2 is NOT installed"; \
		echo "Install with: brew install sdl2"; \
	fi
	@echo ""
	@echo "Checking for SDL2_ttf..."
	@if [ -f /opt/homebrew/lib/libSDL2_ttf.dylib ] || [ -f /usr/local/lib/libSDL2_ttf.dylib ] || pkg-config --exists SDL2_ttf 2>/dev/null; then \
		echo "SDL2_ttf is installed"; \
	else \
		echo "SDL2_ttf is NOT installed"; \
		echo "Install with: brew install sdl2_ttf"; \
	fi
	@echo ""
	@echo "Checking for SDL2_image..."
	@if [ -f /opt/homebrew/lib/libSDL2_image.dylib ] || [ -f /usr/local/lib/libSDL2_image.dylib ] || pkg-config --exists SDL2_image 2>/dev/null; then \
		echo "SDL2_image is installed"; \
	else \
		echo "SDL2_image is NOT installed"; \
		echo "Install with: brew install sdl2_image"; \
	fi
	@echo ""
	@echo "Checking for C++ compiler..."
	@if command -v $(CXX) >/dev/null 2>&1; then \
		echo "$(CXX) is installed"; \
		$(CXX) --version | head -n 1; \
	else \
		echo "$(CXX) is NOT installed"; \
	fi
	@echo ""
	@echo "=== Check Complete ==="

# Build the snake game
snake: $(SRC)
	@echo "Building Snake Game..."
	$(CXX) $(CXXFLAGS) $(SDL2_CFLAGS) -o $(TARGET) $(SRC) $(SDL2_LDFLAGS)
	@echo "Build complete! Run with: ./$(TARGET)"

# Clean build artifacts
clean:
	@echo "Cleaning build artifacts..."
	rm -f $(TARGET)
	@echo "Clean complete!"

.PHONY: all check snake clean

