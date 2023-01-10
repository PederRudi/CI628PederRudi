#include "MyGame.h"

void MyGame::on_receive(std::string cmd, std::vector<std::string>& args) {
    if (cmd == "GAME_DATA") {
        // we should have exactly 6 arguments
        if (args.size() == 6) {
            game_data.player1Y = stoi(args.at(0));
            game_data.player1X = stoi(args.at(1));
            game_data.player2Y = stoi(args.at(2));
            game_data.player2X = stoi(args.at(3));
            game_data.ballX = stoi(args.at(4));
            game_data.ballY = stoi(args.at(5));
        }
    }

    else {
        std::cout << "Received: " << cmd << std::endl;
    }
    if (cmd == "SCORES") {
        //the scores for player 1 and player 2
        if (args.size() == 2) {
            player1Score = stoi(args.at(0));
            player2Score = stoi(args.at(1));
            wallCollide();
        }
    }
    //The server notifies when the ball hits a bat or a wall
    if (cmd == "BALL_HIT_BAT1") {
        bat1Collide();
    }
    if (cmd == "BALL_HIT_BAT2") {
        bat2Collide();
    }
    if (cmd == "HIT_WALL_DOWN" || cmd == "HIT_WALL_UP") {
        wallCollide();
    }
    
    
}

void MyGame::send(std::string message) {
    messages.push_back(message);
}

void MyGame::input(SDL_Event& event) {
    switch (event.key.keysym.sym) {
        //Player 1 movement
        case SDLK_w:
            send(event.type == SDL_KEYDOWN ? "W_DOWN" : "W_UP");
            break;
        case SDLK_s:
            send(event.type == SDL_KEYDOWN ? "S_DOWN" : "S_UP");
            break;
        case SDLK_a:
            send(event.type == SDL_KEYDOWN ? "A_DOWN" : "A_UP");
            break;
        case SDLK_d:
            send(event.type == SDL_KEYDOWN ? "D_DOWN" : "D_UP");
            break;
            //Player 2 movement
        case SDLK_i:
            send(event.type == SDL_KEYDOWN ? "I_DOWN" : "I_UP");
            break;
        case SDLK_k:
            send(event.type == SDL_KEYDOWN ? "K_DOWN" : "K_UP");
            break;
        case SDLK_j:
            send(event.type == SDL_KEYDOWN ? "J_DOWN" : "J_UP");
            break;
        case SDLK_l:
            send(event.type == SDL_KEYDOWN ? "L_DOWN" : "L_UP");
            break;
    }
}

void MyGame::init_audio() {
    // Initializing audio
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) == -1) {
        printf("Mix_OpenAudio: %s\n", Mix_GetError());
        return;
    }

    //Loading sound files
    hitBat = Mix_LoadWAV("../assets/hit_bat.wav");
    if (hitBat == nullptr) {
        printf("Mix_LoadWAV: %s\n", Mix_GetError());
    }
    else {
        std::cout << "Sound effect loaded" << std::endl;
    }

    hitWall = Mix_LoadWAV("../assets/hit_wall.wav");
    if (hitBat == nullptr) {
        printf("Mix_LoadWAV: %s\n", Mix_GetError());
    }
    else {
        std::cout << "Sound effect loaded" << std::endl;
    }
}

void MyGame::init(SDL_Renderer* renderer) {

    if (TTF_Init() < 0)
    {
        printf("Couldn't initialize SDL TTF: %s\n", SDL_GetError());
        exit(1);
    }
}


int getRandomOffset() {
    //Creates a pseudorandom integer between -2 and 2. Used to slightly offset the bat.
    return (rand() % 5) - 2;
}

float getDeltaTime() {
    //calculates and returns the time difference, or "delta time," between consecutive calls to the function.
    static Uint32 previousTicks = 0;
    Uint32 currentTicks = SDL_GetTicks();
    float deltaTime = (currentTicks - previousTicks) / 1000.0f;
    previousTicks = currentTicks;
    return deltaTime;
}

void MyGame::bat1Collide() {
    //Plays a sound and sets a timer for how long the bat will shake
    if (Mix_PlayChannel(-1, hitBat, 0) == -1) {
        printf("Error playing sound. Mix_PlayChannel: %s\n", Mix_GetError());
    }

    p1shakeTimer = 0.3;
}
void MyGame::bat2Collide() {
    //Plays a sound and sets a timer for how long the bat will shake
    if (Mix_PlayChannel(-1, hitBat, 0) == -1) {
        printf("Error playing sound. Mix_PlayChannel: %s\n", Mix_GetError());
    }

    p2shakeTimer = 0.3;
}

void MyGame::wallCollide() {
    //Plays a sound
    if (Mix_PlayChannel(-1, hitWall, 0) == -1) {
        printf("Error playing sound. Mix_PlayChannel: %s\n", Mix_GetError());
    }
    printf("wall");
}

void MyGame::update() {
    player1.y = game_data.player1Y;
    player1.x = game_data.player1X;
    player2.y = game_data.player2Y;
    player2.x = game_data.player2X;
    ball.centerY = game_data.ballY;
    ball.centerX = game_data.ballX;

    //Offsets the bat every update until timer runs out
    //Gives the illusion of shaking from impact with the ball
    if (p1shakeTimer > 0) {
        p1shakeTimer -= getDeltaTime();
        player1.x += getRandomOffset();
        player1.y += getRandomOffset();
    }
    if (p2shakeTimer > 0) {
        p2shakeTimer -= getDeltaTime();
        player2.x += getRandomOffset();
        player2.y += getRandomOffset();
    }
    
}




void drawCircle(SDL_Renderer* renderer, const Circle* circle)
{
    //function for drawing circles
    const int NUM_SEGMENTS = 360;
    const double DEG_TO_RAD = M_PI / 180.0;

    for (int i = 0; i < NUM_SEGMENTS; i++)
    {
        double degInRad = i * DEG_TO_RAD;
        double x = circle->centerX + cos(degInRad) * circle->radius;
        double y = circle->centerY + sin(degInRad) * circle->radius;

        SDL_RenderDrawPoint(renderer, x, y);
    }
}


void MyGame::render(SDL_Renderer* renderer) {

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); //White
    SDL_RenderDrawRect(renderer, &player1);
    SDL_RenderDrawRect(renderer, &player2);
    drawCircle(renderer, &ball);
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);  // blue 
    SDL_RenderFillRect(renderer, &player1);  //fill player1 rectangle with blue color
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);  // red color
    SDL_RenderFillRect(renderer, &player2);  //fill player2 rectangle with red color

   
    
    TTF_Font* Sans = TTF_OpenFont("../assets/8bitOperatorPlus8-Bold.ttf", 24);

    //Turn Score into string
    std::string p1ScoreText = std::to_string(player1Score);
    std::string p2ScoreText = std::to_string(player2Score);
    
    SDL_Color White = { 255, 255, 255 };
    SDL_Color Blue = { 0, 0, 255, 255 };
    SDL_Color Red = { 255, 0, 0, 255 };

    // Create surface
    SDL_Surface* p1scoreSurface =
        TTF_RenderText_Solid(Sans, p1ScoreText.c_str(), Blue);
    SDL_Surface* p2scoreSurface =
        TTF_RenderText_Solid(Sans, p2ScoreText.c_str(), Red);

    // convert into texture
    SDL_Texture* p1ScoreTexture = SDL_CreateTextureFromSurface(renderer, p1scoreSurface);

    SDL_Texture* p2ScoreTexture = SDL_CreateTextureFromSurface(renderer, p2scoreSurface);

    //Rectangles to contain the score
    SDL_Rect p1Score_rect; 
    p1Score_rect.x = 0;
    p1Score_rect.y = 0;
    p1Score_rect.w = 50;
    p1Score_rect.h = 50;
    SDL_RenderCopy(renderer, p1ScoreTexture, NULL, &p1Score_rect);

    SDL_Rect p2Score_rect;
    p2Score_rect.x = 750;
    p2Score_rect.y = 0;
    p2Score_rect.w = 50;
    p2Score_rect.h = 50;
    SDL_RenderCopy(renderer, p2ScoreTexture, NULL, &p2Score_rect);

    //free surface and texture
    SDL_FreeSurface(p1scoreSurface);
    SDL_FreeSurface(p2scoreSurface);
    SDL_DestroyTexture(p1ScoreTexture);
    SDL_DestroyTexture(p2ScoreTexture);

    SDL_RenderPresent(renderer);
}

void MyGame::cleanup() {
    //Release any resources allocated by the game and close any open resources before the program exits
    Mix_FreeChunk(hitBat);
    hitBat = nullptr;
    hitWall = nullptr;

    Mix_CloseAudio();
}
