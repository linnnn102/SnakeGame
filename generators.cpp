#include "generators.h"
#include <cstdlib>

// Fruit Generator -- random position avoid walls and snake head
void FruitGenerator(int x, int y, int wallX[], int wallY[], int wallCount) {
  bool validPosition_fruit = false;
  while (!validPosition_fruit) {
    fruitX = rand() % width;
    fruitY = rand() % height;
    validPosition_fruit = true;
    
    // Check if fruit spawns on snake head
    if (fruitX == x && fruitY == y) {
      validPosition_fruit = false;
      continue;
    }
    
    // Check if fruit spawns on wall
    for (int i = 0; i < wallCount; i++) {
      if (fruitX == wallX[i] && fruitY == wallY[i]) {
        validPosition_fruit = false;
        break;
      }
    }
  }
}


// Wall Generator -- create a vertical wall in random position of the screen
void WallGenerator(int wallX[], int wallY[], int& wallCount) {
  wallCount = 0;

  // Create 2-4 random walls
  int numWalls = 2 + rand() % 3;

  for (int w = 0; w < numWalls; w++) {

    // Vertical or horizontal
    int direction = rand() % 2;

    // Wall length: 2-5
    int wallLength = 2 + rand() % 4;

    if (wallCount + wallLength > MAX_WALL_BLOCKS) {
      break;
    }
    if (direction == 0) {
      // VERTICAL WALL
      int wallX_pos = 2 + rand() % (width - 4);
      int startY = 2 + rand() % (height - wallLength - 4);
      
      for (int i = 0; i < wallLength; i++) {
        wallX[wallCount] = wallX_pos;
        wallY[wallCount] = startY + i;
        wallCount++;
      }
      
    } else {
      // HORIZONTAL WALL
      int wallY_pos = 2 + rand() % (height - 4);
      int startX = 2 + rand() % (width - wallLength - 4);
      
      for (int i = 0; i < wallLength; i++) {
        wallX[wallCount] = startX + i;
        wallY[wallCount] = wallY_pos;
        wallCount++;
      }
    }
  }
}
