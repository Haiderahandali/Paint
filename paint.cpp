#include <SDL2/SDL.h>
#include <iostream>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

SDL_Surface* WindowSurface = NULL;
SDL_Window* gWindow        = NULL;

uint8_t brush_width  = 1;
uint8_t brush_height = 1;

#define MAX_BRUSH_SIZE 10

const Uint8* Keyboard;

struct Pixel
{

    uint8_t red;
    uint8_t blue;
    uint8_t green;
};

struct Vec2D
{
    int x;
    int y;
};

enum COLORS
{
    WHITE = 0,
    BLACK,
    RED,
    BLUE,
    GREEN,
    CYAN,
    CLEAR_SCREEN_COLOR,
    YELLOW,
    NICE_COLOR

};

uint32_t pixels[9];
COLORS current_color = WHITE;

bool init();

void pixel_init();
void UpdateWindow();
void eventLoop();
Vec2D getMousePosition();
bool running = true;
void paint(SDL_Surface* surface, uint32_t color, Vec2D pos);
int main(int argc, char const* argv[])
{
    if (!init())
    {
        printf("ERROR COULDNT INIT %s\n", SDL_GetError());
        exit(2);
    }
       

    while (running)
    {
        eventLoop();
        UpdateWindow();
    }

    return 0;
}

bool init()
{
    bool success = true;
    //Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
        success = false;
    }
    else
    {

        //Create window
        gWindow = SDL_CreateWindow("Paint", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE);
    }

    WindowSurface = SDL_GetWindowSurface(gWindow);
    if (WindowSurface == NULL)
    {
        printf("Couldn't create Window surface Error:%s \n", SDL_GetError());
    }

    //--------------------- init some of our stuff --------------------//
    Keyboard = SDL_GetKeyboardState(NULL);
    pixel_init();

    //-----------------------------------------------------------------------------//
    return success;
}

void eventLoop()
{
    SDL_Event e;

    while (SDL_PollEvent(&e))
    {
        switch (e.type)
        {
        case SDL_QUIT:

            running = false;
            break;
        case SDL_WINDOWEVENT:
            switch (e.window.event)
            {
            case SDL_WINDOWEVENT_SIZE_CHANGED:
                printf(" Window size %d and window size %d \n ", e.window.data1, e.window.data2);
                break;
            }

            //
        default:
            break;
        }

        if (Keyboard[SDL_SCANCODE_SPACE])
        {
            Vec2D mousePos = getMousePosition();
            paint(WindowSurface, pixels[current_color], mousePos);
        }
        else if (Keyboard[SDL_SCANCODE_C])
        {
            SDL_FillRect(WindowSurface, NULL, pixels[CLEAR_SCREEN_COLOR]);
        }
        else if (Keyboard[SDL_SCANCODE_1])
        {
            current_color = RED;
        }

        else if (Keyboard[SDL_SCANCODE_2])
        {
            current_color = GREEN;
        }
        else if (Keyboard[SDL_SCANCODE_3])
        {
            current_color = BLUE;
        }
        else if (Keyboard[SDL_SCANCODE_4])
        {
            current_color = YELLOW;
        }
        else if (Keyboard[SDL_SCANCODE_5])
        {
            current_color = CYAN;
        }
        else if (Keyboard[SDL_SCANCODE_6])
        {
            current_color = NICE_COLOR;
        }

        else if (Keyboard[SDL_SCANCODE_UP])
        {
            if (brush_width < MAX_BRUSH_SIZE)
            {
                ++brush_width;
                ++brush_height;
            }
        }

        if (Keyboard[SDL_SCANCODE_DOWN])
        {
            if (brush_width > 1)
            {
                --brush_width;
                --brush_height;
            }
        }
    }
}

void UpdateWindow()
{
    SDL_UpdateWindowSurface(gWindow);
}

Vec2D getMousePosition()
{
    int x, y;
    SDL_GetMouseState(&x, &y);
    return { x, y };
}

void pixel_init()
{

    pixels[WHITE]              = SDL_MapRGB(WindowSurface->format, 255, 255, 255);
    pixels[BLACK]              = SDL_MapRGB(WindowSurface->format, 0, 0, 0);
    pixels[RED]                = SDL_MapRGB(WindowSurface->format, 255, 0, 0);
    pixels[BLUE]               = SDL_MapRGB(WindowSurface->format, 0, 0, 255);
    pixels[GREEN]              = SDL_MapRGB(WindowSurface->format, 0, 255, 0);
    pixels[CYAN]               = SDL_MapRGB(WindowSurface->format, 0, 255, 255);
    pixels[CLEAR_SCREEN_COLOR] = 0x00000000;
    pixels[NICE_COLOR]         = 0x1f1c3bff;
    pixels[YELLOW]             = 0x00ffffff;
}

void paint(SDL_Surface* surface, uint32_t color, Vec2D pos)
{
    SDL_Rect area;

    area.x = pos.x, area.y = pos.y;
    area.w = brush_width, area.h = brush_height;

    SDL_FillRect(surface, &area, color);
}