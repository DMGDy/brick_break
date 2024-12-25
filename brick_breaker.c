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

#define BG (Color) {0x09, 0x0a , 0x13, 255}
#define BALL_RED (Color) {0xDC,0x14,0x3c,0xFF}

const int screen_w = 1200;
const int screen_h = 800;

const int radius = 10;
const int rect_w = radius*12;

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int 
main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------

    //Vector2 scale = GetWindowScaleDPI();
    InitWindow(screen_w,screen_h,"Sample Window");

    Vector2 ball_vel = {
      .x = 10,
      .y = 10
    };

    Vector2 ball_pos = {
      .x = radius,
      .y =radius 
    };

    Rectangle rec = {
      .x = (float)screen_w/2,
      .y = screen_h - 100,
      .width = rect_w,
      .height = radius
    };

    Rectangle brick = {
      .x = (float)screen_w/2,
      .y = 0,
      .width = (int)(rect_w/2),
      .height = radius*2
    };

    int should_exist = 1;


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
            rec.x -= 10;
          }
          break;
        case(KEY_RIGHT):
          if(rec.x+rec.width + 10 <= screen_w) {
            rec.x += 10;
          }
          break;
      }
      if(IsKeyDown(KEY_RIGHT)) {
        if(rec.x+rec.width + 10 <= screen_w) {
          rec.x += 10;
        }
      }

      if(IsKeyDown(KEY_LEFT)) {
        if(rec.x - 10 >= 0) {
          rec.x -= 10;
        }
      }



        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
            ClearBackground(BG);

            DrawRectangleRec(rec,WHITE);
            if(should_exist) {
              DrawRectangleRec(brick,GRAY);
            }
            DrawCircleV(ball_pos,radius,BALL_RED);
            ball_pos = Vector2Add(ball_pos,ball_vel);


        EndDrawing();

        if(ball_pos.x >= screen_w - radius || ball_pos.x <= radius) {
          ball_vel.x *=-1;
        }

        if(ball_pos.y>= screen_h - radius || ball_pos.y <= radius|| 
            CheckCollisionCircleRec(ball_pos,radius,rec)) {
          ball_vel.y *= -1;
        }

        if(CheckCollisionCircleRec(ball_pos,radius,rec)) {
          float vel_mag = Vector2Length(ball_vel);
          float relative_hit_pos = (((ball_pos.x - rec.x)/rec.width) * 2)-1; 
          float angle_modifier = relative_hit_pos * PI/4;
          float angle = -PI/2 + angle_modifier;

          ball_vel.x = vel_mag * cos(angle);
          ball_vel.y = vel_mag * sin(angle);


        }

        if(CheckCollisionCircleRec(ball_pos,radius,brick)) {
          should_exist = 0;
          ball_vel.y *= -1;
        }


        //----------------------------------------------------------------------------------
        //
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
