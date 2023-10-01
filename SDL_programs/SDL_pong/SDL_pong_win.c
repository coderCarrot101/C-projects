#define SDL_MAIN_HANDLED

#include <stdio.h>
#include <windows.h>
#include <stdbool.h>
#include <unistd.h>
#include <limits.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include "./constants.h"

//TODO: make it so that after you score the paddles spawn in the correct locations,
// also make it so that after someone wins the ball does not revert to larger size

///////////////////////////////////////////////////////////////////////////////
// Global variables
///////////////////////////////////////////////////////////////////////////////

int game_is_running = false;
int last_frame_time = 0;
int input_update[20];
SDL_Window *window = NULL;
int flag = 0;
SDL_Renderer *renderer = NULL;
char score[20] = "0";
int pOneWin = 0;
char scoreB[20] = "0";
int pTwoWin = 0;
double ballSpeed = 1.0;
char path[50];
char * pPath;;
char cwd[50];

///////////////////////////////////////////////////////////////////////////////
// Declare two game objects for the ball and the paddle
///////////////////////////////////////////////////////////////////////////////
struct game_object {
    float x;
    float y;
    float width;
    float height;
    float vel_x;
    float vel_y;
} ball, paddle, paddleB;

///////////////////////////////////////////////////////////////////////////////
// Function to initialize our SDL window
///////////////////////////////////////////////////////////////////////////////
int initialize_window(void) {

    
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        fprintf(stderr, "Error initializing SDL.\n");
        return false;
    }
    window = SDL_CreateWindow(
        "PONG",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINWINDOW_WIDTH,
        WINWINDOW_HEIGHT,
        0
    );

    if (!window) {
        fprintf(stderr, "Error creating SDL Window.\n");
        return false;
    }
    renderer = SDL_CreateRenderer(window, -1, 0);
    if (!renderer) {
        fprintf(stderr, "Error creating SDL Renderer.\n");
        return false;
    }
    return true;
}

///////////////////////////////////////////////////////////////////////////////
// Function to poll SDL events and process keyboard input
///////////////////////////////////////////////////////////////////////////////
int * process_input(int * input_update) {
    SDL_Event event;
    static bool keys[SDL_NUM_SCANCODES] = { false };  // Initialize all keys to false
    int * pInput_update = input_update;

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                game_is_running = false;
                break;
            case SDL_KEYDOWN:
                keys[event.key.keysym.scancode] = true;
                break;
            case SDL_KEYUP:
                keys[event.key.keysym.scancode] = false;
                break;
        }
    }

    // Move the paddle based on key states
    if (keys[SDL_SCANCODE_W]) {
        *pInput_update = 'w';
        pInput_update++;
    } 
    if (keys[SDL_SCANCODE_S]) {
        *pInput_update = 's';
        pInput_update++;
    }
    if (keys[SDL_SCANCODE_I]) {
        *pInput_update = 'i';
        pInput_update++;
    }
    if (keys[SDL_SCANCODE_K]) {
        *pInput_update = 'k';
        pInput_update++;
    }

    *pInput_update = '0';
    return input_update;
}

///////////////////////////////////////////////////////////////////////////////
// Setup function that runs once at the beginning of our program
///////////////////////////////////////////////////////////////////////////////
void setup(void) {
    // Initialize the ball object moving down at a constant velocity
    ball.x = (WINDOW_WIDTH - ball.width) / 2;
    ball.y = (WINDOW_HEIGHT - ball.height) / 2;
    ball.width = 10;
    ball.height = 10;
    ball.vel_x = 200;
    ball.vel_y = 150;

    paddle.x = 60;
    paddle.y = 300;
    paddle.width = 10;
    paddle.height = 100;
    paddle.vel_x = 0;
    paddle.vel_y = 50;

    paddleB.x = 740;
    paddleB.y = 300;
    paddleB.width = 10;
    paddleB.height = 100;
    paddleB.vel_x = 0;
    paddleB.vel_y = 125;
}

///////////////////////////////////////////////////////////////////////////////
// Update function with a fixed time step
///////////////////////////////////////////////////////////////////////////////
void update(int * input_update) {
    //contains how many ticks are left before the next second
    int time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks()- last_frame_time);

    //caps frame rate
    if (time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME) {
        SDL_Delay(time_to_wait);
    }

    // Get delta_time factor converted to seconds to be used to update objects
    float delta_time = (SDL_GetTicks() - last_frame_time) / 1000.0;

    // Store the milliseconds of the current frame to be used in the next one
    last_frame_time = SDL_GetTicks();

    if (flag > 0 && flag < 6) {
        flag++;
        score[0] = '0';
    } else if (flag >= 2) {
        flag = 0;
    }
    // Move paddle based on input
    while (*input_update != '0') {
        switch (*input_update) {
            case 'w':
                if (paddle.y > 10) {
                    paddle.y -= PADDLE_SPEED;
                }
                break;
            case 's':
                if (paddle.y < 510) {
                    paddle.y += PADDLE_SPEED;
                }
                break;
            case 'i':
                if (paddleB.y > 10) {
                    paddleB.y -= PADDLE_SPEED;
                }
                break;
            case 'k':
                if (paddleB.y < 510) {
                    paddleB.y += PADDLE_SPEED;
                }
                break;
        }
        input_update++;
    }
    // Move ball as a function of delta time
    ball.x += (ball.vel_x  * ballSpeed) * delta_time;
    ball.y += (ball.vel_y  * ballSpeed) * delta_time;

    // Check for ball collision with the window borders
    if (ball.x < 0) {
        ball.x = 0;
        ball.vel_x = -ball.vel_x;
    }
    if (ball.x + ball.height > WINDOW_WIDTH) {
        ball.x = WINDOW_WIDTH - ball.width;
        ball.vel_x = -ball.vel_x;
        score[0]++;
        ballSpeed += 0.15;
        if (score[0] >= '5') {
            pOneWin = 1;
            int i = 0;
            char win[] = "P1 wins";
            while (strcmp(score, win) != 0) {
                score[i] = win[i];
                i++;
            }
            ballSpeed = 1;
        } else {
            ball.x = (WINDOW_WIDTH - ball.width) / 2;
            ball.y = (WINDOW_HEIGHT - ball.height) / 2;

            paddle.x = 60;
            paddle.y = 300;

            paddleB.x = 740;
            paddleB.y = 300;
            
        }
    } else if (ball.x - ball.height <= 0) {
        ball.x = 0 + ball.width;
        ball.vel_x = -ball.vel_x;
        scoreB[0]++;
        ballSpeed += 0.15;
        if (scoreB[0] >= '5') {
            pTwoWin = 1;
            int i = 0;
            char win[] = "P2 wins";
            while (strcmp(scoreB, win) != 0) {
                scoreB[i] = win[i];
                i++;
            }
            ballSpeed = 1;
        } else {
            ball.x = (WINDOW_WIDTH - ball.width) / 2;
            ball.y = (WINDOW_HEIGHT - ball.height) / 2;

            paddle.x = 60;
            paddle.y = 300;

            paddleB.x = 740;
            paddleB.y = 300;
        }
    }

    if (ball.y < 0) {
        ball.y = 0;
        ball.vel_y = -ball.vel_y;
    }
    if (ball.y + ball.height > WINDOW_HEIGHT) {
        ball.y = WINDOW_HEIGHT - ball.height;
        ball.vel_y = -ball.vel_y;
    }

    if ((ball.x + ball.height <= (paddle.x + paddle.width + 10) && ball.x + ball.height >= (paddle.x)) && ball.y + ball.height <= (paddle.y + paddle.height + 10) && ball.y + ball.height >= (paddle.y)) {
        ball.x = (paddle.x + paddle.width + 10) - ball.width;
        ball.vel_x = -ball.vel_x;
    }

    if ((ball.x + ball.height <= (paddleB.x + paddleB.width + 10) && ball.x + ball.height >= (paddleB.x)) && ball.y + ball.height <= (paddleB.y + paddleB.height + 10) && ball.y + ball.height >= (paddleB.y)) {
        ball.x = (paddleB.x) - ball.width;
        ball.vel_x = -ball.vel_x;
    }


}


///////////////////////////////////////////////////////////////////////////////
// Render function to draw game objects in the SDL window
///////////////////////////////////////////////////////////////////////////////
void render(void) {

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Draw a rectangle for the ball object
    SDL_Rect ball_rect = {
        (int)ball.x,
        (int)ball.y,
        (int)ball.width,
        (int)ball.height
    };
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &ball_rect);

    SDL_Rect paddle_rect = {
        (int)paddle.x,
        (int)paddle.y,
        (int)paddle.width,
        (int)paddle.height
    };

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &paddle_rect);

    SDL_Rect paddleB_rect = {
        (int)paddleB.x,
        (int)paddleB.y,
        (int)paddleB.width,
        (int)paddleB.height
    };

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &paddleB_rect);

    //this opens a font style and sets a size
    TTF_Font* Sans = TTF_OpenFont(pPath, 24);

    // this is the color in rgb format,
    // maxing out all would give you the color white,
    // and it will be your text's color
    SDL_Color White = {255, 255, 255};

    SDL_Surface* surfaceMessage = TTF_RenderText_Solid(Sans, score, White);

    // now you can convert it into a texture
    SDL_Texture* Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);

    SDL_Rect Message_rect; //create a rect
    Message_rect.x = 200;  //controls the rect's x coordinate 
    Message_rect.y = 50; // controls the rect's y coordinte
    Message_rect.w = 50; // controls the width of the rect
    Message_rect.h = 50; // controls the height of the rect

    // (0,0) is on the top left of the window/screen,
    // think a rect as the text's box,
    // that way it would be very simple to understand

    // Now since it's a texture, you have to put RenderCopy
    // in your game loop area, the area where the whole code executes

    // you put the renderer's name first, the Message,
    // the crop size (you can ignore this if you don't want
    // to dabble with cropping), and the rect which is the size
    // and coordinate of your texture
    SDL_RenderCopy(renderer, Message, NULL, &Message_rect);

    SDL_Surface* surfaceMessageB = TTF_RenderText_Solid(Sans, scoreB, White);

    // now you can convert it into a texture
    SDL_Texture* MessageB = SDL_CreateTextureFromSurface(renderer, surfaceMessageB);

    SDL_Rect MessageB_rect; //create a rect
    MessageB_rect.x = 500;  //controls the rect's x coordinate 
    MessageB_rect.y = 50; // controls the rect's y coordinte
    MessageB_rect.w = 50; // controls the width of the rect
    MessageB_rect.h = 50; // controls the height of the rect

    // (0,0) is on the top left of the window/screen,
    // think a rect as the text's box,
    // that way it would be very simple to understand

    // Now since it's a texture, you have to put RenderCopy
    // in your game loop area, the area where the whole code executes

    // you put the renderer's name first, the Message,
    // the crop size (you can ignore this if you don't want
    // to dabble with cropping), and the rect which is the size
    // and coordinate of your texture
    SDL_RenderCopy(renderer, MessageB, NULL, &MessageB_rect);


    SDL_FreeSurface(surfaceMessage);
    SDL_FreeSurface(surfaceMessageB);

    SDL_RenderPresent(renderer);

    if (pOneWin == 1) {
        score[0] = '0';
        scoreB[0] = '0';
        for (int i = 1; i < 20; i++) {
            scoreB[i] = '\0';
            score[i] = '\0';
        }

        pOneWin = 0;
        ball.x = 600;
        ball.y = 500;
        ball.width = 10;
        ball.height = 10;
        ball.vel_x = 400;
        ball.vel_y = 300;

        SDL_Delay(5000);

        pOneWin = 0;
        ball.x = 600;
        ball.y = 500;
        ball.width = 10;
        ball.height = 10;
        ball.vel_x = 400;
        ball.vel_y = 300;
        flag = 1;
    }

    if (pTwoWin == 1) {
        scoreB[0] = '0';
        score[0] = '0';
        for (int i = 1; i < 20; i++) {
            scoreB[i] = '\0';
            score[i] = '\0';
        }

        pTwoWin = 0;
        ball.x = 600;
        ball.y = 500;
        ball.width = 10;
        ball.height = 10;
        ball.vel_x = 400;
        ball.vel_y = 300;

        SDL_Delay(5000);

        pTwoWin = 0;
        ball.x = 600;
        ball.y = 500;
        ball.width = 10;
        ball.height = 10;
        ball.vel_x = 400;
        ball.vel_y = 300;
        flag = 1;
    }

    TTF_CloseFont(Sans);

}

///////////////////////////////////////////////////////////////////////////////
// Function to destroy SDL window and renderer
///////////////////////////////////////////////////////////////////////////////
void destroy_window(void) {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

///////////////////////////////////////////////////////////////////////////////
// Main function
///////////////////////////////////////////////////////////////////////////////
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

    if(TTF_Init()==-1) {
    printf("TTF_Init: %s\n", TTF_GetError());
    exit(2);
    }

    getcwd(cwd, sizeof(cwd));
    pPath = strcat(cwd, "/Roboto-Black.ttf");

    game_is_running = initialize_window();
    int * pInput_update = input_update;

    setup();
    while (game_is_running) {
        pInput_update = process_input(input_update);
        update(pInput_update);
        render();
    }
    	
    TTF_Quit();
    destroy_window();
    return 0;
}

