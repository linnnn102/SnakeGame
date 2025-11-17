#ifndef GENERATORS_H
#define GENERATORS_H

// Constants (shared with snake.cpp)
extern const int width;
extern const int height;
extern const int MAX_WALL_BLOCKS;
extern int fruitX;
extern int fruitY;

// Function declarations
void WallGenerator(int wallX[], int wallY[], int& wallCount);
void FruitGenerator(int x, int y, int wallX[], int wallY[], int wallCount);

#endif