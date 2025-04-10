#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include <SDL.h>

#define WINDOW_HEIGHT 4000
#define WINDOW_WIDTH 3000
#define METERS_ON_X_AXIS 16
#define METERS_ON_Y_AXIS 9
#define TOPBAR_HEIGHT 80
#define RAY_AMOUNT 300
#define FPS 60
#define FRAME_TARGET_TIME (1000 / FPS)


class App;
class Ball;
class Ray;
class SourceObject;
class BlockerObject;
class MirrorObject;
class TopBar;
class Button;
class AddSourceButton;
class AddBlockerButton;
class DeleteSourceButton;
class DeleteBlockerButton;
class AddMirrorButton;
class DeleteMirrorButton;
class Renderer1;
class Update;

extern SDL_Window* window;
extern SDL_Renderer* renderer;
extern SDL_Texture* buttonTexture;
extern bool running;
extern int window_width;

#endif