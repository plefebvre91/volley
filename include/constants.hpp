/* MIT License

Copyright (c) 2022 Pierre Lefebvre

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE. */


#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

// Windows parameters
#define VL_APP_TITLE    "Volley"
#define VL_WINDOW_WIDTH    1200
#define VL_WINDOW_HEIGHT    720
#define VL_FPS               60
#define VL_MARGIN            30

// Threads parameters
#define VL_EVENT_THREAD_MS  50
#define VL_UPDATE_THREAD_MS 16

// Physics
#define VL_BOUND_RESTITUTION 0.20f
#define VL_GRAVITY           9.81f
#define VL_DEFAULT_FRICTION  0.96f
#define VL_PLAYER_FRICTION   0.50f
#define VL_BALL_FRICTION     0.99f

// Collider properties
#define VL_COLLIDER_BALL_R    32
#define VL_COLLIDER_PLAYER_R  62
#define VL_SUM_DIST_BEFORE_COLLISION (VL_COLLIDER_BALL_R+VL_COLLIDER_PLAYER_R)
#define VL_DIST_BEFORE_COLLISION ((VL_SUM_DIST_BEFORE_COLLISION)*(VL_SUM_DIST_BEFORE_COLLISION))

// Game
#define VL_NB_PLAYERS     2
#define VL_NB_SHADOWS     3
#define VL_NB_NP_ENTITIES 3
#define VL_NB_SOUNDS      2
#define VL_FLOOR          700
#define VL_SHADOW_WIDTH   60

// Movements
#define VL_JUMP_STEP  8.0f
#define VL_MOVE_STEP 13.0f
#define VL_MOVE_LEFT (-VL_MOVE_STEP)
#define VL_MOVE_RIGHT (VL_MOVE_STEP)

#endif
