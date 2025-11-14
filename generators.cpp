#include "generators.h"
#include <cstdlib>

// Wall Generator -- create a vertical wall in the middle of the screen
void WallGenerator(int wallX[], int wallY[], int& wallCount) {
  int middleX = width / 2;
  for (int i = height / 4; i < height * 3 / 4; i++) {
    wallX[wallCount] = middleX;
    wallY[wallCount] = i;
    wallCount++;
  }
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