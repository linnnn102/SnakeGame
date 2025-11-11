#include <SDL.h>
#include <SDL_ttf.h>
#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <SDL_image.h>

using namespace std;

// Game variables
bool gameOver;
const int width = 20;
const int height = 20;
int x, y, fruitX, fruitY, score;
int tailX[100], tailY[100];
int nTail;

enum eDirection {STOP = 0, LEFT, RIGHT, UP, DOWN};
eDirection dir;

// SDL variables
const int CELL_SIZE = 25;
const int SCREEN_WIDTH = width * CELL_SIZE;
const int SCREEN_HEIGHT = height * CELL_SIZE + 50; // Extra space for score
SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
TTF_Font* font = nullptr;

bool InitSDL() {

  // Initialize SDL Video Subsystem -- for window creation and rendering
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << endl;
    return false;
  }

  // Initialize SDL TTF Subsystem -- for text like scores display 
  if (TTF_Init() == -1) {
    cout << "TTF could not initialize! TTF_Error: " << TTF_GetError() << endl;
    return false;
  }

  // Create game window
  window = SDL_CreateWindow("Snake Game", 
                            SDL_WINDOWPOS_CENTERED, 
                            SDL_WINDOWPOS_CENTERED, 
                            SCREEN_WIDTH, 
                            SCREEN_HEIGHT, 
                            SDL_WINDOW_SHOWN);
  if (window == nullptr) {
    cout << "Window could not be created! SDL_Error: " << SDL_GetError() << endl;
    return false;
  }

  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  if (renderer == nullptr) {
    cout << "Renderer could not be created! SDL_Error: " << SDL_GetError() << endl;
    return false;
  }

  // Initialize bricks
  if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
    cerr << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << endl;
  }
  SDL_Texture* brickTexture = IMG_LoadTexture(renderer, "assets/brick.png");
  if (!brickTexture) {
    cerr << "Failed to load brick texture: " << IMG_GetError() << endl;
  }

  // Score font display 
  font = TTF_OpenFont("/System/Library/Fonts/Helvetica.ttc", 24);
  if (font == nullptr) {
    cout << "Warning: Could not load font! TTF_Error: " << TTF_GetError() << endl;
  }

  return true;
}

void CloseSDL() {
  if (font != nullptr) {
    TTF_CloseFont(font);
    font = nullptr;
  }
  
  if (renderer != nullptr) {
    SDL_DestroyRenderer(renderer);
    renderer = nullptr;
  }
  
  if (window != nullptr) {
    SDL_DestroyWindow(window);
    window = nullptr;
  }
  
  TTF_Quit();
  SDL_Quit();
}

void Setup(){
  srand(time(0));
  gameOver = false;
  dir = STOP;
  x = width / 2;
  y = height / 2;
  fruitX = rand() % width;
  fruitY = rand() % height;
  score = 0;
  nTail = 0;
}

// Draw a filled circle at the given center point with the given radius
void DrawFilledCircle(SDL_Renderer* renderer, int centerX, int centerY, int radius) {
  for (int w = 0; w < radius * 2; w++) {
    for (int h = 0; h < radius * 2; h++) {
      int dx = radius - w; // horizontal offset
      int dy = radius - h; // vertical offset
      if ((dx*dx + dy*dy) <= (radius * radius)) {
        SDL_RenderDrawPoint(renderer, centerX + dx, centerY + dy);
      }
    }
  }
}

void Draw(){
  // Clear screen with black background
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);

  // Draw game board border
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  SDL_Rect border = {0, 0, SCREEN_WIDTH, height * CELL_SIZE};
  SDL_RenderDrawRect(renderer, &border);

  //Draw bricks
  vector<SDL_Rect> walls = {
      {100, 100, 20, 20},
      {120, 100, 20, 20},
      {140, 100, 20, 20}
  };

  // Render walls
  for (const auto& wall : walls) {
      SDL_RenderCopy(renderer, brickTexture, nullptr, &wall);
  }

  // Draw snake head
  SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
  SDL_Rect head = {x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE};
  SDL_RenderFillRect(renderer, &head);

  // Draw snake tail
  SDL_SetRenderDrawColor(renderer, 0, 200, 0, 255);
  for (int i = 0; i < nTail; i++) {
    SDL_Rect tail = {tailX[i] * CELL_SIZE, tailY[i] * CELL_SIZE, CELL_SIZE, CELL_SIZE};
    SDL_RenderFillRect(renderer, &tail);
  }

  // Draw fruit -- as a red circle, positioned from upper-left corner
  SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
  int fruitCenterX = fruitX * CELL_SIZE + CELL_SIZE / 2;
  int fruitCenterY = fruitY * CELL_SIZE + CELL_SIZE / 2;
  int fruitRadius = CELL_SIZE / 2 - 2; // Slightly smaller than cell
  DrawFilledCircle(renderer, fruitCenterX, fruitCenterY, fruitRadius);

  // Draw score
  if (font != nullptr) {
    string scoreText = "Score: " + to_string(score);
    SDL_Color white = {255, 255, 255, 255};
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, scoreText.c_str(), white);
    if (textSurface != nullptr) {
      SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
      if (textTexture != nullptr) {
        SDL_Rect textRect = {10, height * CELL_SIZE + 10, textSurface->w, textSurface->h};
        SDL_RenderCopy(renderer, textTexture, nullptr, &textRect);
        SDL_DestroyTexture(textTexture);
      }
      SDL_FreeSurface(textSurface);
    }
  }

  // Update screen
  SDL_RenderPresent(renderer);
}

void Input(SDL_Event& e){
  if (e.type == SDL_KEYDOWN) {
    switch (e.key.keysym.sym) {
      case SDLK_a:
      case SDLK_LEFT:
        if (dir != RIGHT) dir = LEFT;
        break;
      case SDLK_d:
      case SDLK_RIGHT:
        if (dir != LEFT) dir = RIGHT;
        break;
      case SDLK_w:
      case SDLK_UP:
        if (dir != DOWN) dir = UP;
        break;
      case SDLK_s:
      case SDLK_DOWN:
        if (dir != UP) dir = DOWN;
        break;
      case SDLK_x:
      case SDLK_ESCAPE:
        gameOver = true;
        break;
      case SDLK_SPACE:
        if (dir == STOP) dir = RIGHT; // Start the game
        break;
    }
  }
}

void Logic(){
  int prevX = tailX[0];
  int prevY = tailY[0];
  int prev2X, prev2Y;
  tailX[0] = x;
  tailY[0] = y;
  for (int i = 1; i < nTail; i++){
    prev2X = tailX[i];
    prev2Y = tailY[i];
    tailX[i] = prevX;
    tailY[i] = prevY;
    prevX = prev2X;
    prevY = prev2Y;
  }

  switch(dir){
    case LEFT:
      x--;
      break;
    case RIGHT:
      x++;
      break;
    case UP:
      y--;
      break;
    case DOWN:
      y++;
      break;
    default:
      break;
  }

  if (x >= width || x < 0 || y >= height || y < 0)
    gameOver = true;
  for (int i = 0; i < nTail; i++){
    if (tailX[i] == x && tailY[i] == y)
      gameOver = true;
  }

  for (const auto& wall : walls) {
    if (snakeHeadRect.x == wall.x && snakeHeadRect.y == wall.y) {
        gameOver = true;
    }
  }

  if (x == fruitX && y == fruitY){
    score += 10;
    
    // Generate new fruit position (avoid current position)
    do {
      fruitX = rand() % width;
      fruitY = rand() % height;
    } while (fruitX == x && fruitY == y); // Repeat if fruit spawns on snake head
    
    nTail++;
  }
}

int ShowGameOverDialog(int finalScore) {
  const SDL_MessageBoxButtonData buttons[] = {
    { SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT, 0, "Quit" },
    { SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 1, "Restart" }
  };
  
  string message = "Your score: " + to_string(finalScore) + "\n\nWould you like to play again?";
  
  const SDL_MessageBoxData messageboxdata = {
    SDL_MESSAGEBOX_INFORMATION,
    window,
    "Game Over",
    message.c_str(),
    SDL_arraysize(buttons),
    buttons,
    nullptr
  };
  
  int buttonid;
  if (SDL_ShowMessageBox(&messageboxdata, &buttonid) < 0) {
    cout << "Error displaying message box" << endl;
    return 0; // Default to quit on error
  }
  
  return buttonid; // 0 = Quit, 1 = Restart
}

int main(){
  if (!InitSDL()) {
    cout << "Failed to initialize SDL!" << endl;
    return 1;
  }

  bool quit = false;
  
  while (!quit) {
    Setup();
    
    SDL_Event e;
    Uint32 lastUpdate = SDL_GetTicks();
    const Uint32 GAME_SPEED = 100; // milliseconds per frame

    while (!quit && !gameOver) {
      // Handle events
      while (SDL_PollEvent(&e) != 0) {
        if (e.type == SDL_QUIT) {
          quit = true;
        }
        Input(e);
      }

      // Update game logic at fixed intervals
      Uint32 currentTime = SDL_GetTicks();
      if (currentTime - lastUpdate >= GAME_SPEED) {
        Logic();
        lastUpdate = currentTime;
      }

      // Draw
      Draw();
      
      // Cap frame rate
      SDL_Delay(16); // ~60 FPS
    }

    // Show game over dialog with Restart/Quit options
    if (gameOver) {
      int choice = ShowGameOverDialog(score);
      if (choice == 0) {
        quit = true; // User chose Quit
      }
    }
  }
  
  CloseSDL();
  return 0;
}