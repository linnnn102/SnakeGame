# Snake Game - SDL2 GUI Version

A classic Snake game with a graphical window interface using SDL2.

## Quick Start

### 1. Check Dependencies
First, check if all required libraries are installed:
```bash
make check
```

### 2. Install Missing Libraries (macOS)
If any libraries are missing, install them with Homebrew:
```bash
brew install sdl2 sdl2_ttf sdl2_image
```

<!-- ### Linux (Ubuntu/Debian)
```bash
sudo apt-get install libsdl2-dev libsdl2-ttf-dev libsdl2-image-dev
```

### Linux (Fedora/RHEL)
```bash
sudo dnf install SDL2-devel SDL2_ttf-devel SDL2_image-devel
``` -->

## Building and Running

### Build the Game
```bash
make snake
```

Or simply:
```bash
make
```

### Run the Game
```bash
./snake
```

A window will pop up with the Snake game!

### Clean Build Files
```bash
make clean
```

## Makefile Commands

- **`make check`** - Checks if all required libraries are installed
- **`make snake`** - Builds the snake game executable
- **`make`** or **`make all`** - Same as `make snake` (default target)
- **`make clean`** - Removes the compiled executable

## Controls

- **WASD** or **Arrow Keys**: Control the snake direction
  - W/↑: Move Up
  - S/↓: Move Down
  - A/←: Move Left
  - D/→: Move Right
- **Space**: Start the game (when stopped)
- **ESC** or **X**: Quit the game
- **Close Window**: Quit the game

## Gameplay

- Guide the snake to eat the red fruit
- Each fruit increases your score by 10 points
- The snake grows longer with each fruit eaten
- Don't hit the walls or your own tail!

## Cleaning Up

To remove the compiled executable:
```bash
make clean
```

## Features

- Smooth GUI rendering
- Real-time keyboard input
- Score display
- Game Over popup with final score
- Snake grows as it eats fruit
- Collision detection with walls and self

## Technical Details

- Built with C++ and SDL2
- Uses SDL2_ttf for text rendering
- 20x20 game grid
- 25x25 pixel cells
- ~60 FPS rendering
- 100ms game update interval

