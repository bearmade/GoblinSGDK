#ifndef CAMERA_H
#define CAMERA_H


#include <genesis.h>

void camera();
void startTransition(int direction);
void finishTransition();

int current_cam_x;
int current_cam_y;
bool isTransitioning;
int transitionDirection; // 0=right, 1=left, 2=down, 3=up
fix32 transitionOffset;

#define HOW_FAR_LEFT_BEFORE_CAMERA_MOVES 118
#define HOW_FAR_RIGHT_BEFORE_CAMERA_MOVES 138
#define HOW_FAR_TOP_BEFORE_CAMERA_MOVES 104
#define HOW_FAR_BOTTOM_BEFORE_CAMERA_MOVES 124

#define HORIZONTAL_RESOLUTION 256
#define VERTICAL_RESOLUTION 224

#endif


