#include <iostream>
#include <SDL2/SDL.h>
#include <vector>
#include <cmath>

constexpr int WINDOW_HEIGHT = 540;
constexpr int WINDOW_WIDTH = 960;
constexpr int CUBE_LENGTH = 100; // Length of a side in px.
constexpr int MID_X = WINDOW_WIDTH / 2;
constexpr int MID_Y = WINDOW_HEIGHT / 2;

struct coord3D{float x, y, z;};
struct coord2D{float x, y;};
struct line{int start, end;};
