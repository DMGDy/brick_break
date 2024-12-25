/*
   brick-break
   Copyright (C) 2024 Dylan Dy or Dylan-Matthew Garza
   <dylangarza1909@gmail.com>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 3 of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "raylib.h"
#include "raymath.h"
#include <time.h>
#include <stdlib.h>

#define BG (Color) {0x09, 0x0a , 0x13, 255}
#define BALL_RED (Color) {0xDC,0x14,0x3c,0xFF}

typedef struct {
  Rectangle brick;
  int should_exist;
  Color color;
}Brick;

const int screen_w = 1200;
const int screen_h = 800;

const int radius = 10;
const int rect_w = radius*16;


//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int 
main(void)
{
  // Initialization
  //--------------------------------------------------------------------------------------
  size_t brick_count_x = 50;
  size_t brick_count_y = (int)(screen_h * .65)/50;


  srand(time(NULL));

  //Vector2 scale = GetWindowScaleDPI();
  InitWindow(screen_w,screen_h,"Sample Window");

  Vector2 ball_vel = {
    .x = 10,
    .y = 10
  };

  Rectangle rec = {
    .x = (float)screen_w/2,
    .y = screen_h - 100,
    .width = rect_w,
    .height = radius
  };

  Vector2 ball_pos = {
    .x = (0.5 * rec.width) + rec.x,
    .y = rec.y - (1.5* radius)
  };


  float brick_side_len = ((float)screen_w/50);

  Brick*** bricks = realloc(NULL, brick_count_y * sizeof(Brick**)); 

  for(size_t dy = 0; dy < brick_count_y; ++dy){
    bricks[dy] = realloc(NULL,brick_count_x * sizeof(Brick*));
    for(size_t dx = 0; dx < brick_count_x; ++dx) {
      Brick* brick = malloc(sizeof(Brick));

      *brick = (Brick){
        .brick = {
          .x = brick_side_len *dx,
          .y = brick_side_len *dy,
          .width = brick_side_len,
          .height = brick_side_len
        },

        .should_exist = 1,
        .color = (Color) {rand()%127,rand()%127,rand()%127,255}

      };

      bricks[dy][dx] = brick;
    }
  }

  Brick brick = {
    .brick = {
      .x = (float)screen_w/2,
      .y = 0,
      .width = (int)(screen_w/50),
      .height = (int)(screen_w/50)
    },

    .should_exist = 1,
    .color = (Color) {rand()%127,rand()%127,rand()%127,255}
  };

  float rect_speed = Vector2Length(ball_vel)*0.90;

  SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
                                  //--------------------------------------------------------------------------------------

                                  // Main game loop
  while (!WindowShouldClose())    // Detect window close button or ESC key
  {
    // Update
    //----------------------------------------------------------------------------------
    // TODO: Update your variables here
    //----------------------------------------------------------------------------------
    switch(GetKeyPressed()) {
      case(KEY_F):
        ToggleFullscreen();
        break;
      case(KEY_LEFT):
        if(rec.x - 10 >= 0) {
          rec.x -= rect_speed;
        }
        break;
      case(KEY_RIGHT):
        if(rec.x+rec.width + 10 <= screen_w) {
          rec.x += rect_speed;
        }
        break;
    }
    if(IsKeyDown(KEY_RIGHT)) {
      if(rec.x+rec.width + 10 <= screen_w) {
        rec.x += rect_speed;
      }
    }

    if(IsKeyDown(KEY_LEFT)) {
      if(rec.x - 10 >= 0) {
        rec.x -= rect_speed;
      }
    }

    // Draw
    //----------------------------------------------------------------------------------
    BeginDrawing();
    ClearBackground(BG);

    DrawRectangleRec(rec,WHITE);
    if(brick.should_exist) {
      DrawRectangleRec(brick.brick,brick.color);
    }
    DrawCircleV(ball_pos,radius,BALL_RED);
    ball_pos = Vector2Add(ball_pos,ball_vel);

    
    for(size_t dy = 0;dy<brick_count_y;++dy) {
      for(size_t dx = 0;dx < brick_count_x;++dx) {
        Brick* brick = bricks[dy][dx];
        if(brick->should_exist) {
          DrawRectangleRec(brick->brick,brick->color);
        }
      }
    }
    

    EndDrawing();

    if(ball_pos.x >= screen_w - radius || ball_pos.x <= radius) {
      ball_vel.x *=-1;
    }

    if(ball_pos.y>= screen_h - radius || ball_pos.y <= radius){
      ball_vel.y *= -1;
    }

    
    for(size_t dy = 0;dy < brick_count_y;++dy) {
      int hit = 0;
      for(size_t dx = 0; dx < brick_count_x; ++dx) {
        Brick* brick = bricks[dy][dx];
        if(CheckCollisionCircleRec(ball_pos,radius,brick->brick)&&brick->should_exist) {
          Vector2 closest = {
            .x = fmax(brick->brick.x, fmin(ball_pos.x, brick->brick.x + brick->brick.width)),
            .y = fmax(brick->brick.y, fmin(ball_pos.y, brick->brick.y + brick->brick.height))
          };

          brick->should_exist = 0;
          // ball hit horizontal side
          if(closest.x == brick->brick.x || closest.x == brick->brick.x + brick->brick.width) {
            ball_vel.x *= -1;
          }else { // else vertical side
            ball_vel.y *= -1;
          }
          hit = 1;
        }
      }
    }
    

    if(CheckCollisionCircleRec(ball_pos,radius,rec)) {
      float vel_mag = Vector2Length(ball_vel);
      float relative_hit_pos = (((ball_pos.x - rec.x)/rec.width) * 2)-1; 
      float angle_modifier = relative_hit_pos * PI/4;
      float angle = -PI/2 + angle_modifier;

      ball_vel.x = vel_mag * cos(angle);
      ball_vel.y = vel_mag * sin(angle);


    }

    /*
    if(CheckCollisionCircleRec(ball_pos,radius,brick.brick)) {
      brick.should_exist = 0;
      ball_vel.y *= -1;
    }
    */


    //----------------------------------------------------------------------------------
    //
  }

  // De-Initialization
  //--------------------------------------------------------------------------------------
  CloseWindow();        // Close window and OpenGL context
                        //--------------------------------------------------------------------------------------

  return 0;
}
