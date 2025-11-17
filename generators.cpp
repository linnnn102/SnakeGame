#include "generators.h"
#include <cstdlib>

// Wall Generator -- create a vertical wall in random position of the screen
void WallGenerator(int wallX[], int wallY[], int& wallCount) {
  int randomX = rand() % width;
  int randomY = rand() % height;
  wallX[wallCount] = randomX;
  wallY[wallCount] = randomY;
  wallCount++;
}

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