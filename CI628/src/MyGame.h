#ifndef __MY_GAME_H__
#define __MY_GAME_H__

#include <iostream>
#include <vector>
#include <string>


#include "SDL.h"
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include "SDL_image.h"


static struct GameData {
    int player1Y = 0;
    int player1X = 0;
    int player2Y = 0;
    int player2X = 0;
    int ballX = 0;
    int ballY = 0;
} game_data;

struct Circle {
    int centerX;
    int centerY;
    int radius;
};

class MyGame {
    private:
        SDL_Rect player1 = { 200, 0, 50, 50 };
        SDL_Rect player2 = { 580, 0, 50, 50 };
        Circle ball = { 0, 0, 10};
        int player1Score = 0;
        int player2Score = 0;
        float p1shakeTimer = 0;
        float p2shakeTimer = 0;
        Mix_Chunk* hitBat;
        Mix_Chunk* hitWall;
    public:
        std::vector<std::string> messages;

        void on_receive(std::string message, std::vector<std::string>& args);
        void send(std::string message);
        void input(SDL_Event& event);
        void update();
        void init(SDL_Renderer* renderer);
        void render(SDL_Renderer* renderer);
        void init_audio();
        void bat1Collide();
        void bat2Collide();
        void wallCollide();
        void cleanup();
};

#endif