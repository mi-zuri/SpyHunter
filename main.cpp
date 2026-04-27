// READ IT AND THE NEXT PAGE: https://www.programiz.com/c-programming/c-enumeration
// HERE YOU CAN CHECK STL LIBRARY HEADERS: https://www.ibm.com/docs/en/zos/2.1.0?topic=files-cstdio
// DYNAMIC ALLOCATION OF RECT: https://stackoverflow.com/questions/3929920/c-and-sdl-how-does-sdl-rect-work-exactly
// PASSING MULTIPLE VARIABLES TO A FUNCTION: https://stackoverflow.com/questions/2625474/how-can-multiple-variables-be-passed-to-a-function-cleanly-in-c
// SOMETHING GAME RELATED: https://gameprogrammingpatterns.com/object-pool.html
// KEYCODES: https://wiki.libsdl.org/SDL2/SDL_Keycode
// ASCII TABLE IN C: https://www.javatpoint.com/ascii-table-in-c
// PIXILART

// MIGHT BE USEFUL:
// 
// TO WORK NEEDS TO BE OPEN IN VISUAL STUDIO
// UPLOAD TO GITHUB ALL BUT SLN FILE
// How to run?
// Open as a project: szablon2vs17.vcxproj`
// Add main.cpp, assets, saves
// 
// SDL_FillRect(sdl.screen, nullptr, BLACK);                                // fills the rectangle with color (nullptr fills the whole surface)
// scrtex = SDL_CreateTextureFromSurface(renderer, screen);                 // pretty slow
// SDL_RenderClear(renderer);                                               // clears the current rendering target with the drawing color
//
// enum seems better than #define as long as it is an int, because:
// define can be redefined
// define does not have a symbol in the debugger symbol table
//
// console window not visible? to see the printf output go to:
// project -> szablon2 properties -> Linker -> System -> Subsystem
// change it to "Console" ("Windows" by default)


// road: turns and forks, trees and flowers around (puddles)
// cars: civilian and enemy (helicopter with bombs)
// score: increases while driving and killing enemies (killing a civilian or driving off-road stops the score counter)
// weapon: shooting and pushing others off the road


#define _USE_MATH_DEFINES	// defines some cool constants
#include<time.h>
#include<stdio.h>
#include<string.h>

extern "C" {
#include"./SDL2-2.0.10/include/SDL.h"
#include"./SDL2-2.0.10/include/SDL_main.h"
}


/* CONSTANTS DECLARATION */

// ! - do not modify (may break the app)

// fps
#define FPS_CAP 60
#define TIME_BETWEEN_FRAMES ((double)1000/FPS_CAP) // in milliseconds       // !
// window
#define SCREEN_WIDTH 1280                                                   // !
#define SCREEN_HEIGHT 1020                                                   // !
#define SH_IN_IN_GAME_METERS (((double)SCREEN_HEIGHT / 145) * 5)            // !
#define TEXT_SIZE 8                                                         // !
#define STRING_WIDTH (strlen(text)*TEXT_SIZE)
#define INFO_SCREEN_LINE_SPACING 8
#define INFO_SCREEN_MARGIN 12
// !
#define INFO_SCREEN_WIDTH (SCREEN_WIDTH/4)
#define INFO_SCREEN_HEIGHT (INFO_SCREEN_LINE_SPACING + (TEXT_SIZE + INFO_SCREEN_LINE_SPACING) * linesOfText)
#define INFO_SCREEN_TEXT_Y_POS (INFO_SCREEN_MARGIN + INFO_SCREEN_LINE_SPACING + (TEXT_SIZE + INFO_SCREEN_LINE_SPACING) * (*lineCount))
#define IMPLEMENTED_LIST_WIDTH (STRING_WIDTH + INFO_SCREEN_LINE_SPACING)
#define IMPLEMENTED_LIST_HEIGHT (TEXT_SIZE + INFO_SCREEN_LINE_SPACING)
#define IMPLEMENTED_LIST_X_POS (SCREEN_WIDTH - (IMPLEMENTED_LIST_WIDTH + INFO_SCREEN_MARGIN))
#define IMPLEMENTED_LIST_Y_POS (SCREEN_HEIGHT - (IMPLEMENTED_LIST_HEIGHT + INFO_SCREEN_MARGIN))
#define IMPLEMENTED_LIST_TEXT_X_POS (IMPLEMENTED_LIST_X_POS + INFO_SCREEN_LINE_SPACING/2)
#define IMPLEMENTED_LIST_TEXT_Y_POS (IMPLEMENTED_LIST_Y_POS + INFO_SCREEN_LINE_SPACING/2)
// surfaces
#define CHARSET_SURFACE assets.surface[0]
#define GRASS_SURFACE assets.surface[1]
#define PLAYER_CAR_SURFACE assets.surface[2]
#define ENEMY_CAR_SURFACE assets.surface[3]
#define CIVILIAN_CAR_SURFACE assets.surface[4]
#define EXPLOSION_START_SURFACE assets.surface[5]
#define EXPLOSION_SURFACE assets.surface[6]
#define FIREBALL_SURFACE assets.surface[7]
// dimensions/placement
#define BASE_ROAD_SIZE 480
#define ROAD_SPREAD 300
#define DISTANCE_BETWEEN_TURNS 4000      // min. (ROAD_SPREAD / TURN_SHARPNESS * 2) + SCREEN_HEIGHT + 30 (error margin)
#define TURN_SHARPNESS 0.2
#define PLAYER_X_POS (int)(SCREEN_WIDTH / 2 + motion->horizontalOffset * SCREEN_WIDTH / 5)
#define PLAYER_Y_POS (int)(SCREEN_HEIGHT / 2 + motion->verticalOffset)
// physics
#define MIN_VELOCITY 320            // pixels / s
#define MAX_VELOCITY 2400
#define DEFAULT_VELOCITY 720
#define ACCELERATION 600           // pixels / s^2
#define DEACCELERATION (-1200)
#define DRAG 100
#define TURN_SPEED 1.1
#define SHOT_VELOCITY 2000
// bots
#define RENDER_DISTANCE 2500
#define CIVILIAN_VELOCITY 880
#define CIVILIAN_CARS_SPREAD (3 * BASE_ROAD_SIZE / 10)
#define ENEMY_VELOCITY 1600
// colors
#define BLACK colors.black
#define DARK_GREY colors.dark_grey
#define RED colors.red

/* STRUCTS USED INSIDE FUNCTIONS */

typedef struct sdl_init_var sdl_init_var;
typedef struct assets_loaded assets_loaded;
typedef struct colors colors;
typedef struct control_var control_var;
typedef struct time_var time_var;
typedef struct dev_var dev_var;
typedef struct motion_var motion_var;
typedef struct environment_var environment_var;
typedef struct bot_var bot_var;
typedef struct enemy_var enemy_var;
typedef struct shot_var shot_var;


/* FUNCTION DECLARATIONS */

/* 1. SYSTEM */

int InitializeSDL(sdl_init_var* sdl);
// required if quitting after full initialization of SDL Library
void QuitApp(const sdl_init_var* sdl, assets_loaded assets);

/* 2. CONTROLS */

int SaveScore(const sdl_init_var* sdl, assets_loaded assets, control_var* controls);
int SaveGame(const sdl_init_var* sdl, assets_loaded assets, control_var controls, time_var time_var, dev_var dev,
             motion_var motion, environment_var environment, bot_var bot, shot_var shot, enemy_var enemy);
int LoadGame(const sdl_init_var* sdl, assets_loaded assets, control_var* controls, time_var* time_var,
             dev_var* dev, motion_var* motion, environment_var* environment, bot_var* bot, shot_var* shot,
             enemy_var* enemy);
void ResetAll(control_var* controls, time_var* time, dev_var* dev, motion_var* motion, environment_var* environment,
              bot_var* bot, shot_var* shot, enemy_var* enemy);
int HandleKeyboardInput(sdl_init_var* sdl, assets_loaded assets, dev_var* dev, time_var* time,
                        motion_var* motion, control_var* controls, environment_var* environment, bot_var* bot,
                        shot_var* shot, enemy_var* enemy);

/* 3. MATH */

void MeasureElapsedTime(time_var* time, dev_var* dev);
// measures refresh rate and fps
void RunPerformanceAnalysis(time_var* time, dev_var* dev);
void PlayersMovement(time_var time, motion_var* motion);
void BotsMovement(motion_var motion, time_var time, bot_var* bot, shot_var shot, enemy_var* enemy);
void ShotMotion(const motion_var* motion, time_var time, shot_var* shot);
void InitializeRoadGeneration(const motion_var* motion, environment_var* environment);
void RoadGeneration(const motion_var* motion, environment_var* environment);
int Death(const sdl_init_var* sdl, assets_loaded assets, control_var* controls, time_var* time, dev_var* dev,
          motion_var* motion, environment_var* environment,
          bot_var* bot, shot_var* shot, enemy_var* enemy);
void GunShot(time_var* time, shot_var* shot, const motion_var* motion);
int RunCalculations(const sdl_init_var* sdl, assets_loaded assets, control_var* controls, time_var* time, dev_var* dev,
                    motion_var* motion,
                    environment_var* environment, bot_var* bot, shot_var* shot, enemy_var* enemy);

/* 4. GRAPHICS */

int LoadBMP(const sdl_init_var* sdl, assets_loaded assets, SDL_Surface** bmp, char* fileName);
int LoadAssets(const sdl_init_var* sdl, assets_loaded* assets);
// draws a text txt on surface screen, starting from the point (x, y)
void DrawString(SDL_Surface* screen, int x, int y, const char* text, SDL_Surface* charset);
// draws a surface sprite on a surface screen in point (x, y)
// (x, y) is the center of sprite on screen
void DrawSurface(SDL_Surface* screen, SDL_Surface* sprite, int x, int y);
void DrawPixel(const SDL_Surface* surface, int x, int y, Uint32 color);
// draws a vertical (when dx = 0, dy = 1) or horizontal (when dx = 1, dy = 0) line
void DrawLine(const SDL_Surface* screen, int x, int y, int l, int dx, int dy, Uint32 color);
// draws a rectangle of size l by k
void DrawRectangle(const SDL_Surface* screen, int x, int y, int l, int k, Uint32 outlineColor, Uint32 fillColor);
void DrawGrass(sdl_init_var sdl, assets_loaded assets, motion_var* motion, environment_var environment);
void DrawRoad(sdl_init_var sdl, motion_var* motion, environment_var environment, colors colors,
              bot_var* bot, enemy_var* enemy);
void DrawEnvironment(sdl_init_var sdl, assets_loaded assets, motion_var* motion,
                     environment_var environment, colors colors, bot_var* bot, enemy_var* enemy);
void DrawBotCars(sdl_init_var sdl, assets_loaded assets, time_var* time, bot_var* bot, enemy_var* enemy);
void DrawPlayersCar(sdl_init_var sdl, assets_loaded assets, time_var time, const motion_var* motion, shot_var shot);
void PrintLineOfInfo(sdl_init_var sdl, assets_loaded assets, const char* text, int* lineCount);
void DrawInfoScreen(sdl_init_var sdl, assets_loaded assets, control_var controls, time_var time,
                    dev_var dev, motion_var motion, colors colors, shot_var shot);
void DrawImplementedList(sdl_init_var sdl, assets_loaded assets, colors colors);
void Render(sdl_init_var sdl);


/* STRUCT DEFINITIONS */

struct sdl_init_var
{
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Surface* screen;
    SDL_Texture* scrtex;
    SDL_Event event;
};

struct assets_loaded
{
    int count = 0;
    SDL_Surface** surface{};

    int createSurface(const sdl_init_var* sdl, const assets_loaded assets, char* text)
    {
        count++;
        surface = (SDL_Surface**)realloc(surface, count * sizeof(SDL_Surface*));
        if (surface == nullptr)
        {
            printf("Memory allocation error!");
            QuitApp(sdl, assets);
            return 1;
        }

        return LoadBMP(sdl, assets, &surface[count - 1], text);
    }

    void freeAll()
    {
        for (int i = 0; i < count; i++)
        {
            SDL_FreeSurface(surface[i]);
        }
        free(surface);
    }
};

struct colors
{
    int black;
    int dark_grey;
    int white;
    int red;
    int blue;
    int green;

    // ints tend to perform better than Uint32 as undefined signed integer overflow allows the compiler
    // to assume that overflows don't happen, which may introduce optimization opportunities

    void init(const sdl_init_var* sdl)
    {
        black = SDL_MapRGB(sdl->screen->format, 0x00, 0x00, 0x00);
        dark_grey = SDL_MapRGB(sdl->screen->format, 0x38, 0x33, 0x33);
        white = SDL_MapRGB(sdl->screen->format, 0xCC, 0xCC, 0xCC);
        red = SDL_MapRGB(sdl->screen->format, 0xFF, 0x00, 0x00);
        blue = SDL_MapRGB(sdl->screen->format, 0x11, 0x11, 0xCC);
        green = SDL_MapRGB(sdl->screen->format, 0x11, 0x73, 0x1E);
    }
};

struct control_var
{
    bool pause = false;
    bool quit = false;
    int toggleInfo = 1;
    int toggleControlsInfo = 0;
    int toggleRecordsInfo = 0;
    int toggleLoadSave = 0;
    int fileChosen = 0;

    char save1[64] = "";
    char save2[64] = "";
    char save3[64] = "";
    char save4[64] = "";
    char save5[64] = "";

    int score = 0;
    char highestScore[64] = "";
    char highestScoreDate[64] = "";
    bool dead = false;
};

struct time_var
{
    Uint64 now = SDL_GetPerformanceCounter();
    Uint64 lastRefresh = now;
    Uint64 onPause = 0;
    double delta = 0; // refresh time (ms)
    double runTime = 0; // time since game start (s)
    double fTimer = 0; // time since last frame (ms)
    double sTimer = 0; // time since last second (ms)
    int timer = 0; // 01010101
    bool timerLock = false;
    int deathTimer = 4;
    int shotTimer = 1;
    int civilianDeathTimer = 2;
};

struct dev_var
{
    int refreshRate = 0;
    int refreshCount = 0;
    int frameCount = 0;
    int fps = 0;
};

struct motion_var
{
    bool gameStarted = false;

    int accelaration = 0; // pixels / s^2
    double velocity = DEFAULT_VELOCITY; // pixels / s
    double distance = 0; // pixels
    double horizontalVelocity = 0;

    double realVelocity = 0; // km/h
    double realDistance = 0; // km

    double verticalOffset = 0;
    double horizontalOffset = 0;

    int offRoad = 0;
};

struct environment_var
{
    bool roadTypeDrawn = false;
    int roadType = 0;
    int previousRoadType = 0;

    double turnLength = 0;
    double distanceSinceTurnStart = 0;
    double distanceSinceTurnEnd = 0;
    int targetRoadOffset = 0;
    int previousTargetRoadOffset = 0;

    SDL_Rect grassL1{0, -SCREEN_HEIGHT, SCREEN_WIDTH / 2, SCREEN_HEIGHT};
    SDL_Rect grassL2{0, 0, SCREEN_WIDTH / 2, SCREEN_HEIGHT};
    SDL_Rect grassR1{SCREEN_WIDTH / 2, -SCREEN_HEIGHT, SCREEN_WIDTH / 2, SCREEN_HEIGHT};
    SDL_Rect grassR2{SCREEN_WIDTH / 2, 0, SCREEN_WIDTH / 2, SCREEN_HEIGHT};

    static int CalculateGrassOffset(const double distance, const int baseOffset)
    {
        return (int)(distance + (baseOffset + SCREEN_HEIGHT)) % (2 * SCREEN_HEIGHT) - SCREEN_HEIGHT;
    }

    void UpdateGrassPos(const double distance)
    {
        grassL1.y = CalculateGrassOffset(distance, -SCREEN_HEIGHT);
        grassL2.y = CalculateGrassOffset(distance, 0);
        grassR1.y = CalculateGrassOffset(distance, -SCREEN_HEIGHT);
        grassR2.y = CalculateGrassOffset(distance, 0);
    }

    void DrawGrass(const sdl_init_var sdl, const assets_loaded assets)
    {
        SDL_BlitSurface(GRASS_SURFACE, nullptr, sdl.screen, &grassL1);
        SDL_BlitSurface(GRASS_SURFACE, nullptr, sdl.screen, &grassL2);
        SDL_BlitSurface(GRASS_SURFACE, nullptr, sdl.screen, &grassR1);
        SDL_BlitSurface(GRASS_SURFACE, nullptr, sdl.screen, &grassR2);
    }
};

struct shot_var
{
    bool shotShot = false;
    int shotVelocity = SHOT_VELOCITY;
    int shotXPos = 0;
    int shotYPos = 0;
    double shotOffset = 0;
    SDL_Rect shot{0, 0, 32, 32};

    void UpdateShotPos(const motion_var* motion)
    {
        shot.x = shotXPos;
        shot.y = PLAYER_Y_POS + shotOffset;
    }

    void Reset(const motion_var* motion)
    {
        shot.x = PLAYER_X_POS;
        shot.y = PLAYER_Y_POS;
        shotOffset = 0;
    }
};

struct bot_var
{
    int civilianVelocity = CIVILIAN_VELOCITY;
    double civilianOffset = RENDER_DISTANCE / 2;
    int civilianYPos = SCREEN_HEIGHT / 2 + civilianOffset;
    int civilianXPos = SCREEN_WIDTH / 2 + CIVILIAN_CARS_SPREAD;
    SDL_Rect civilian{civilianXPos, civilianYPos, 80, 144};
    bool isAlive = true;

    void UpdateCivilian(const motion_var motion, const shot_var shot)
    {
        if ((int)(civilianOffset + RENDER_DISTANCE) % (2 * RENDER_DISTANCE) == 0)
        {
            civilianXPos = SCREEN_WIDTH / 2 + SDL_GetTicks() % CIVILIAN_CARS_SPREAD - CIVILIAN_CARS_SPREAD / 2;
            isAlive = true;
        }
        civilianYPos = SCREEN_HEIGHT / 2 + (int)(civilianOffset + RENDER_DISTANCE) % (2 * RENDER_DISTANCE) -
            RENDER_DISTANCE;
        civilian.y = civilianYPos;

        if ((shot.shot.y > civilian.y - 77 && shot.shot.y < civilian.y + 77 && shot.shot.x > civilian.x - 40 && shot.
            shot.x < civilian.x + 40) && (shot.shotShot || motion.velocity > 2 * MAX_VELOCITY / 3))
        {
            isAlive = false;
        }
    }

    void Reset()
    {
        civilianOffset = RENDER_DISTANCE / 2;
        civilianYPos = SCREEN_HEIGHT / 2 + civilianOffset;
        civilianXPos = SCREEN_WIDTH / 2 + CIVILIAN_CARS_SPREAD;
        isAlive = true;
    }
};

struct enemy_var
{
    int enemyVelocity = ENEMY_VELOCITY;
    double enemyOffset = RENDER_DISTANCE / 2;
    int enemyYPos = SCREEN_HEIGHT / 2 + enemyOffset;
    int enemyXPos = SCREEN_WIDTH / 2 + -CIVILIAN_CARS_SPREAD;
    bool isAlive = true;
    SDL_Rect enemy{enemyXPos, enemyYPos, 80, 144};


    void Update(const motion_var motion, const shot_var shot)
    {
        if ((int)(enemyOffset + RENDER_DISTANCE) % (2 * RENDER_DISTANCE) == 0)
        {
            enemyXPos = SCREEN_WIDTH / 2 + (SDL_GetTicks() % CIVILIAN_CARS_SPREAD) - CIVILIAN_CARS_SPREAD / 2;
            isAlive = true;
        }

        if (enemyYPos < -RENDER_DISTANCE)
        {
            Reset();
        }

        enemyYPos = SCREEN_HEIGHT / 2 + (int)(enemyOffset + RENDER_DISTANCE) % (2 * RENDER_DISTANCE) - RENDER_DISTANCE;
        enemy.y = enemyYPos;

        if ((shot.shot.y > enemy.y - 77 && shot.shot.y < enemy.y + 77 && shot.shot.x > enemy.x - 40 && shot.
            shot.x < enemy.x + 40) && (shot.shotShot || motion.velocity > 2 * MAX_VELOCITY / 3))
        {
            isAlive = false;
        }
    }

    void Reset()
    {
        enemyOffset = RENDER_DISTANCE / 2;
        enemyYPos = SCREEN_HEIGHT / 2 + enemyOffset;
        enemyXPos = SCREEN_WIDTH / 2 + -CIVILIAN_CARS_SPREAD;
        isAlive = true;
    }
};


//----------------------------------------------------------------------------------------------------------------------

/* MAIN */

#ifdef __cplusplus
extern "C"
#endif
int main(int argc, char** argv)
{
    /* SDL SETUP */
    sdl_init_var sdl{};
    if (InitializeSDL(&sdl)) return 1;

    /* ASSETS LOADING */
    assets_loaded assets;
    if (LoadAssets(&sdl, &assets)) return 1;
    // setting charset's transparent pixels
    SDL_SetColorKey(CHARSET_SURFACE, true, 0x000000);

    /* VARIABLES INITIALIZATION */
    control_var controls;
    colors colors{};
    colors.init(&sdl);
    time_var time;
    dev_var dev;
    motion_var motion;
    environment_var environment;
    bot_var bot;
    enemy_var enemy;
    shot_var shot;

    if (SaveScore(&sdl, assets, &controls)) return 1;

    /* GAME LOOP */
    while (!controls.quit)
    {
        if (HandleKeyboardInput(&sdl, assets, &dev, &time, &motion, &controls, &environment, &bot, &shot, &enemy))
            return 1;
        if (!controls.pause)
        {
            if (RunCalculations(&sdl, assets, &controls, &time, &dev, &motion, &environment, &bot, &shot, &enemy))
                return 1;
            if (time.fTimer >= TIME_BETWEEN_FRAMES)
            {
                /* GRAPHICS */
                DrawEnvironment(sdl, assets, &motion, environment, colors, &bot, &enemy);
                DrawBotCars(sdl, assets, &time, &bot, &enemy);
                DrawPlayersCar(sdl, assets, time, &motion, shot);

                /* INFO */
                DrawInfoScreen(sdl, assets, controls, time, dev, motion, colors, shot);
                DrawImplementedList(sdl, assets, colors);

                Render(sdl);
                time.fTimer -= TIME_BETWEEN_FRAMES;
                dev.frameCount++;
            }
            MeasureElapsedTime(&time, &dev);
            RunPerformanceAnalysis(&time, &dev);
        }
    }

    /* GAME QUIT */
    QuitApp(&sdl, assets);
    return 0;
}


//----------------------------------------------------------------------------------------------------------------------

/* FUNCTION DEFINITIONS */

/* 1. SYSTEM */

int InitializeSDL(sdl_init_var* sdl)
{
    // initializing all the SDL subsystems
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        printf("SDL_Init error: %s\n", SDL_GetError());
        return 1;
    }

    // creating a window and a renderer
    // fullscreen: 0, 0, SDL_WINDOW_FULLSCREEN_DESKTOP, &sdl->window, &sdl->renderer
    // screen_size screen: SCREEN_WIDTH, SCREEN_HEIGHT, 0, &sdl->window, &sdl->renderer
    if (SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0, &sdl->window, &sdl->renderer) != 0)
    {
        printf("SDL_CreateWindowAndRenderer error: %s\n", SDL_GetError());
        SDL_Quit(); // quit expected because of already initialized subsystems
        return 1;
    }

    // configuring the window and the renderer
    SDL_SetWindowTitle(sdl->window, "SpyHunter");
    SDL_ShowCursor(SDL_DISABLE);
    // setting a device independent resolution for rendering
    SDL_RenderSetLogicalSize(sdl->renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
    // setting the scaling quality
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "nearest");
    // setting the color used for drawing operations
    SDL_SetRenderDrawColor(sdl->renderer, 0, 0, 0, 255);

    // creating base surface and texture
    sdl->screen = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32,
                                       0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
    sdl->scrtex = SDL_CreateTexture(sdl->renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING,
                                    SCREEN_WIDTH, SCREEN_HEIGHT);

    return 0;
}

void QuitApp(const sdl_init_var* sdl, assets_loaded assets)
{
    assets.freeAll();
    SDL_FreeSurface(sdl->screen);
    SDL_DestroyTexture(sdl->scrtex);
    SDL_DestroyRenderer(sdl->renderer);
    SDL_DestroyWindow(sdl->window);
    SDL_Quit();
}

/* 2. CONTROLS */

int SaveScore(const sdl_init_var* sdl, const assets_loaded assets, control_var* controls)
{
    FILE* read = fopen("./saves/score.txt", "r");
    if (read == nullptr)
    {
        printf("Error opening file!\n");
        QuitApp(sdl, assets);
        return 1;
    }

    char text[64];
    int highestScore;
    int lineCount = 1;
    while (fgets(text, sizeof text, read) != nullptr)
    {
        if (lineCount == 1)
        {
            snprintf(controls->highestScoreDate, sizeof controls->highestScore, "%s", text);
        }
        else if (lineCount == 2)
        {
            snprintf(controls->highestScore, sizeof controls->highestScore, "%s", text);
            sscanf(text, "%d", &highestScore);
        }
        lineCount++;
    }
    fclose(read);

    if (highestScore < controls->score)
    {
        FILE* f = fopen("./saves/score.txt", "w");
        if (f == nullptr)
        {
            printf("Error opening file!\n");
            QuitApp(sdl, assets);
            return 1;
        }

        tm* local;
        time_t t = time(nullptr);
        // Get the localtime
        local = localtime(&t);

        fprintf(f, "%s%d\n", asctime(local), controls->score);

        snprintf(controls->highestScore, sizeof controls->highestScore, "%d", controls->score);
        snprintf(controls->highestScoreDate, sizeof controls->highestScore, "%s", asctime(local));
        fclose(f);
    }
    return 0;
}

int SaveGame(const sdl_init_var* sdl, const assets_loaded assets, const control_var controls, const time_var time_var,
             const dev_var dev, const motion_var motion, const environment_var environment, const bot_var bot,
             const shot_var shot, enemy_var enemy)
{
    FILE* f = fopen("./saves/saves.txt", "a");
    if (f == nullptr)
    {
        printf("Error opening file!\n");
        QuitApp(sdl, assets);
        return 1;
    }

    tm* local;
    time_t t = time(nullptr);
    // Get the localtime
    local = localtime(&t);

    fprintf(f, "%s", asctime(local));
    fprintf(f, "%d\n", controls.pause);
    fprintf(f, "%d\n", controls.score);
    fprintf(f, "%d\n", controls.dead);
    fprintf(f, "%d\n", controls.fileChosen);
    fprintf(f, "%llu\n", time_var.onPause);
    fprintf(f, "%f\n", time_var.delta);
    fprintf(f, "%f\n", time_var.runTime);
    fprintf(f, "%f\n", time_var.fTimer);
    fprintf(f, "%f\n", time_var.sTimer);
    fprintf(f, "%d\n", time_var.timer);
    fprintf(f, "%d\n", time_var.timerLock);
    fprintf(f, "%d\n", time_var.deathTimer);
    fprintf(f, "%d\n", dev.refreshRate);
    fprintf(f, "%d\n", dev.refreshCount);
    fprintf(f, "%d\n", dev.frameCount);
    fprintf(f, "%d\n", dev.fps);
    fprintf(f, "%d\n", motion.gameStarted);
    fprintf(f, "%d\n", motion.accelaration);
    fprintf(f, "%f\n", motion.velocity);
    fprintf(f, "%f\n", motion.distance);
    fprintf(f, "%f\n", motion.horizontalVelocity);
    fprintf(f, "%f\n", motion.realVelocity);
    fprintf(f, "%f\n", motion.realDistance);
    fprintf(f, "%f\n", motion.verticalOffset);
    fprintf(f, "%f\n", motion.horizontalOffset);
    fprintf(f, "%d\n", motion.offRoad);
    fprintf(f, "%d\n", environment.roadTypeDrawn);
    fprintf(f, "%d\n", environment.roadType);
    fprintf(f, "%d\n", environment.previousRoadType);
    fprintf(f, "%f\n", environment.turnLength);
    fprintf(f, "%f\n", environment.distanceSinceTurnStart);
    fprintf(f, "%f\n", environment.distanceSinceTurnEnd);
    fprintf(f, "%d\n", environment.targetRoadOffset);
    fprintf(f, "%d\n", environment.previousTargetRoadOffset);
    fprintf(f, "%d\n", bot.civilianVelocity);
    fprintf(f, "%f\n", bot.civilianOffset);
    fprintf(f, "%d\n", bot.civilianYPos);
    fprintf(f, "%d\n", bot.civilianXPos);
    fprintf(f, "%d\n", time_var.shotTimer);
    fprintf(f, "%d\n", time_var.civilianDeathTimer);
    fprintf(f, "%d\n", bot.isAlive);
    fprintf(f, "%d\n", shot.shotShot);
    fprintf(f, "%d\n", shot.shotVelocity);
    fprintf(f, "%d\n", shot.shotXPos);
    fprintf(f, "%d\n", shot.shotYPos);
    fprintf(f, "%f\n", shot.shotOffset);
    fprintf(f, "%d\n", enemy.enemyVelocity);
    fprintf(f, "%f\n", enemy.enemyOffset);
    fprintf(f, "%d\n", enemy.enemyYPos);
    fprintf(f, "%d\n", enemy.enemyXPos);
    fprintf(f, "%d\n", enemy.isAlive);

    fclose(f);
    return 0;
}

int LoadGame(const sdl_init_var* sdl, const assets_loaded assets, control_var* controls, time_var* time_var,
             dev_var* dev, motion_var* motion, environment_var* environment, bot_var* bot, shot_var* shot,
             enemy_var* enemy)
{
    const int numberOfVariables = 51; // SET ACCORDINGLY !!!
    FILE* f = fopen("./saves/saves.txt", "r");
    if (f == nullptr)
    {
        printf("Error opening file!\n");
        QuitApp(sdl, assets);
        return 1;
    }

    char text[64];
    int lineCount = 1;
    int varGroup = 1;
    if (controls->fileChosen == 0)
    {
        while (fgets(text, sizeof text, f) != nullptr)
        {
            if (lineCount % (numberOfVariables + 1) == 1)
            {
                switch (varGroup)
                {
                case 1:
                    snprintf(controls->save1, sizeof controls->save1, "%s", text);
                    break;
                case 2:
                    snprintf(controls->save2, sizeof controls->save2, "%s", text);
                    break;
                case 3:
                    snprintf(controls->save3, sizeof controls->save3, "%s", text);
                    break;
                case 4:
                    snprintf(controls->save4, sizeof controls->save4, "%s", text);
                    break;
                default: //case 5
                    snprintf(controls->save5, sizeof controls->save5, "%s", text);
                    break;
                }
                varGroup++;
            }
            lineCount++;
        }
    }
    else
    {
        while (fgets(text, sizeof text, f) != nullptr)
        {
            if (lineCount % (numberOfVariables + 1) == 1)
            {
                controls->fileChosen--;
                if (controls->fileChosen == -1)
                {
                    break;
                }
            }
            else if (controls->fileChosen == 0)
            {
                if (lineCount % (numberOfVariables + 1) == 2) sscanf(text, "%hhd", &controls->pause);
                else if (lineCount % (numberOfVariables + 1) == 3) sscanf(text, "%d", &controls->score);
                else if (lineCount % (numberOfVariables + 1) == 4) sscanf(text, "%hhd", &controls->dead);
                else if (lineCount % (numberOfVariables + 1) == 5) sscanf(text, "%d", &controls->fileChosen);
                else if (lineCount % (numberOfVariables + 1) == 6) sscanf(text, "%llu", &time_var->onPause);
                else if (lineCount % (numberOfVariables + 1) == 7) sscanf(text, "%lf", &time_var->delta);
                else if (lineCount % (numberOfVariables + 1) == 8) sscanf(text, "%lf", &time_var->runTime);
                else if (lineCount % (numberOfVariables + 1) == 9) sscanf(text, "%lf", &time_var->fTimer);
                else if (lineCount % (numberOfVariables + 1) == 10) sscanf(text, "%lf", &time_var->sTimer);
                else if (lineCount % (numberOfVariables + 1) == 11) sscanf(text, "%d", &time_var->timer);
                else if (lineCount % (numberOfVariables + 1) == 12) sscanf(text, "%hhd", &time_var->timerLock);
                else if (lineCount % (numberOfVariables + 1) == 13) sscanf(text, "%d", &time_var->deathTimer);
                else if (lineCount % (numberOfVariables + 1) == 14) sscanf(text, "%d", &dev->refreshRate);
                else if (lineCount % (numberOfVariables + 1) == 15) sscanf(text, "%d", &dev->refreshCount);
                else if (lineCount % (numberOfVariables + 1) == 16) sscanf(text, "%d", &dev->frameCount);
                else if (lineCount % (numberOfVariables + 1) == 17) sscanf(text, "%d", &dev->fps);
                else if (lineCount % (numberOfVariables + 1) == 18) sscanf(text, "%hhd", &motion->gameStarted);
                else if (lineCount % (numberOfVariables + 1) == 19) sscanf(text, "%d", &motion->accelaration);
                else if (lineCount % (numberOfVariables + 1) == 20) sscanf(text, "%lf", &motion->velocity);
                else if (lineCount % (numberOfVariables + 1) == 21) sscanf(text, "%lf", &motion->distance);
                else if (lineCount % (numberOfVariables + 1) == 22) sscanf(text, "%lf", &motion->horizontalVelocity);
                else if (lineCount % (numberOfVariables + 1) == 23) sscanf(text, "%lf", &motion->realVelocity);
                else if (lineCount % (numberOfVariables + 1) == 24) sscanf(text, "%lf", &motion->realDistance);
                else if (lineCount % (numberOfVariables + 1) == 25) sscanf(text, "%lf", &motion->verticalOffset);
                else if (lineCount % (numberOfVariables + 1) == 26) sscanf(text, "%lf", &motion->horizontalOffset);
                else if (lineCount % (numberOfVariables + 1) == 27) sscanf(text, "%d", &motion->offRoad);
                else if (lineCount % (numberOfVariables + 1) == 28) sscanf(text, "%hhd", &environment->roadTypeDrawn);
                else if (lineCount % (numberOfVariables + 1) == 29) sscanf(text, "%d", &environment->roadType);
                else if (lineCount % (numberOfVariables + 1) == 30) sscanf(text, "%d", &environment->previousRoadType);
                else if (lineCount % (numberOfVariables + 1) == 31) sscanf(text, "%lf", &environment->turnLength);
                else if (lineCount % (numberOfVariables + 1) == 32) sscanf(
                    text, "%lf", &environment->distanceSinceTurnStart);
                else if (lineCount % (numberOfVariables + 1) == 33) sscanf(
                    text, "%lf", &environment->distanceSinceTurnEnd);
                else if (lineCount % (numberOfVariables + 1) == 34) sscanf(text, "%d", &environment->targetRoadOffset);
                else if (lineCount % (numberOfVariables + 1) == 35) sscanf(
                    text, "%d", &environment->previousTargetRoadOffset);
                else if (lineCount % (numberOfVariables + 1) == 36) sscanf(text, "%d", &bot->civilianVelocity);
                else if (lineCount % (numberOfVariables + 1) == 37) sscanf(text, "%lf", &bot->civilianOffset);
                else if (lineCount % (numberOfVariables + 1) == 38) sscanf(text, "%d", &bot->civilianYPos);
                else if (lineCount % (numberOfVariables + 1) == 39) sscanf(text, "%d", &bot->civilianXPos);
                else if (lineCount % (numberOfVariables + 1) == 40) sscanf(text, "%d", &time_var->shotTimer);
                else if (lineCount % (numberOfVariables + 1) == 41) sscanf(text, "%d", &time_var->civilianDeathTimer);
                else if (lineCount % (numberOfVariables + 1) == 42) sscanf(text, "%hhd", &bot->isAlive);
                else if (lineCount % (numberOfVariables + 1) == 43) sscanf(text, "%hhd", &shot->shotShot);
                else if (lineCount % (numberOfVariables + 1) == 44) sscanf(text, "%d", &shot->shotVelocity);
                else if (lineCount % (numberOfVariables + 1) == 45) sscanf(text, "%d", &shot->shotXPos);
                else if (lineCount % (numberOfVariables + 1) == 46) sscanf(text, "%d", &shot->shotYPos);
                else if (lineCount % (numberOfVariables + 1) == 47) sscanf(text, "%lf", &shot->shotOffset);
                else if (lineCount % (numberOfVariables + 1) == 48) sscanf(text, "%d", &enemy->enemyVelocity);
                else if (lineCount % (numberOfVariables + 1) == 49) sscanf(text, "%lf", &enemy->enemyOffset);
                else if (lineCount % (numberOfVariables + 1) == 50) sscanf(text, "%d", &enemy->enemyYPos);
                else if (lineCount % (numberOfVariables + 1) == 51) sscanf(text, "%d", &enemy->enemyXPos);
                else if (lineCount % (numberOfVariables + 1) == 0) sscanf(text, "%hhd", &enemy->isAlive);
            }
            lineCount++;
        }
        controls->fileChosen = 0;
        controls->toggleLoadSave = 0;
    }

    fclose(f);
    return 0;
}

void ResetAll(control_var* controls, time_var* time, dev_var* dev, motion_var* motion, environment_var* environment,
              bot_var* bot, shot_var* shot, enemy_var* enemy)
{
    controls->pause = false;
    controls->score = 0;
    controls->dead = false;
    controls->fileChosen = 0;
    time->now = SDL_GetPerformanceCounter();
    time->lastRefresh = time->now;
    time->onPause = 0;
    time->delta = 0;
    time->runTime = 0;
    time->fTimer = 0;
    time->sTimer = 0;
    time->timer = 0;
    time->timerLock = false;
    time->deathTimer = 4;
    time->shotTimer = 1;
    time->civilianDeathTimer = 2;
    dev->refreshRate = 0;
    dev->refreshCount = 0;
    dev->frameCount = 0;
    dev->fps = 0;
    motion->gameStarted = false;
    motion->accelaration = 0;
    motion->velocity = DEFAULT_VELOCITY;
    motion->distance = 0;
    motion->horizontalVelocity = 0;
    motion->realVelocity = 0;
    motion->realDistance = 0;
    motion->verticalOffset = 0;
    motion->horizontalOffset = 0;
    motion->offRoad = 0;
    environment->roadTypeDrawn = false;
    environment->roadType = 0;
    environment->previousRoadType = 0;
    environment->turnLength = 0;
    environment->distanceSinceTurnStart = 0;
    environment->distanceSinceTurnEnd = 0;
    environment->targetRoadOffset = 0;
    environment->previousTargetRoadOffset = 0;
    bot->civilianVelocity = CIVILIAN_VELOCITY;
    bot->civilianOffset = RENDER_DISTANCE / 2;
    bot->civilianYPos = SCREEN_HEIGHT / 2 + bot->civilianOffset;
    bot->civilianXPos = SCREEN_WIDTH / 2 + CIVILIAN_CARS_SPREAD;
    bot->isAlive = true;
    shot->shotShot = false;
    shot->shotVelocity = SHOT_VELOCITY;
    shot->shotXPos = 0;
    shot->shotYPos = 0;
    shot->shotOffset = 0;
    enemy->enemyVelocity = ENEMY_VELOCITY;
    enemy->enemyOffset = RENDER_DISTANCE / 2;
    enemy->enemyYPos = SCREEN_HEIGHT / 2 + enemy->enemyOffset;
    enemy->enemyXPos = SCREEN_WIDTH / 2 + -CIVILIAN_CARS_SPREAD;
    enemy->isAlive = true;
}

int HandleKeyboardInput(sdl_init_var* sdl, const assets_loaded assets, dev_var* dev, time_var* time,
                        motion_var* motion, control_var* controls, environment_var* environment, bot_var* bot,
                        shot_var* shot, enemy_var* enemy)
{
    const Uint8* keys = SDL_GetKeyboardState(nullptr);
    while (SDL_PollEvent(&sdl->event))
    {
        switch (sdl->event.type)
        {
        case SDL_KEYDOWN:
            if (sdl->event.key.keysym.sym == SDLK_ESCAPE) controls->quit = true;
            else if (sdl->event.key.keysym.sym == SDLK_UP) motion->accelaration = ACCELERATION;
            else if (sdl->event.key.keysym.sym == SDLK_DOWN) motion->accelaration = DEACCELERATION;
            else if (sdl->event.key.keysym.sym == SDLK_RIGHT) motion->horizontalVelocity = TURN_SPEED;
            else if (sdl->event.key.keysym.sym == SDLK_LEFT) motion->horizontalVelocity = -TURN_SPEED;
            else if (sdl->event.key.keysym.sym == SDLK_SPACE) shot->shotShot = true;
            else if (sdl->event.key.keysym.sym == SDLK_p && controls->pause == false)
            {
                controls->pause = true;
                time->onPause = time->now;
            }
            else if (sdl->event.key.keysym.sym == SDLK_p && controls->pause == true)
            {
                controls->pause = false;
                time->onPause = SDL_GetPerformanceCounter() - time->onPause;
            }
            else if (sdl->event.key.keysym.sym == SDLK_s)
            {
                if (SaveGame(sdl, assets, *controls, *time, *dev, *motion, *environment, *bot, *shot, *enemy)) return 1;
            }
            else if (sdl->event.key.keysym.sym == SDLK_l)
            {
                controls->toggleLoadSave = 1;
                if (LoadGame(sdl, assets, controls, time, dev, motion, environment, bot, shot, enemy)) return 1;
            }
            else if (controls->toggleLoadSave == 1 && sdl->event.key.keysym.sym == SDLK_1)
            {
                controls->fileChosen = 1;
                if (LoadGame(sdl, assets, controls, time, dev, motion, environment, bot, shot, enemy)) return 1;
            }
            else if (controls->toggleLoadSave == 1 && sdl->event.key.keysym.sym == SDLK_2)
            {
                controls->fileChosen = 2;
                if (LoadGame(sdl, assets, controls, time, dev, motion, environment, bot, shot, enemy)) return 1;
            }
            else if (controls->toggleLoadSave == 1 && sdl->event.key.keysym.sym == SDLK_3)
            {
                controls->fileChosen = 3;
                if (LoadGame(sdl, assets, controls, time, dev, motion, environment, bot, shot, enemy)) return 1;
            }
            else if (controls->toggleLoadSave == 1 && sdl->event.key.keysym.sym == SDLK_4)
            {
                controls->fileChosen = 4;
                if (LoadGame(sdl, assets, controls, time, dev, motion, environment, bot, shot, enemy)) return 1;
            }
            else if (controls->toggleLoadSave == 1 && sdl->event.key.keysym.sym == SDLK_5)
            {
                controls->fileChosen = 5;
                if (LoadGame(sdl, assets, controls, time, dev, motion, environment, bot, shot, enemy)) return 1;
            }
            else if (sdl->event.key.keysym.sym == SDLK_n)
            {
                ResetAll(controls, time, dev, motion, environment, bot, shot, enemy);
            }
            else if (sdl->event.key.keysym.sym == SDLK_TAB && controls->toggleInfo == 0)
            {
                controls->toggleInfo = 1;
                controls->toggleControlsInfo = 0;
            }
            else if (sdl->event.key.keysym.sym == SDLK_TAB && controls->toggleInfo == 1) controls->toggleInfo = 0;
            else if (sdl->event.key.keysym.sym == SDLK_c && controls->toggleControlsInfo == 0)
            {
                controls->toggleControlsInfo = 1;
                controls->toggleInfo = 0;
            }
            else if (sdl->event.key.keysym.sym == SDLK_c && controls->toggleControlsInfo == 1)
                controls->
                    toggleControlsInfo = 0;
            else if (sdl->event.key.keysym.sym == SDLK_r && controls->toggleRecordsInfo == 0)
                controls->
                    toggleRecordsInfo = 1;
            else if (sdl->event.key.keysym.sym == SDLK_r && controls->toggleRecordsInfo == 1)
                controls->
                    toggleRecordsInfo = 0;
            break;
        case SDL_KEYUP:
            if (!keys[SDL_SCANCODE_UP] && !keys[SDL_SCANCODE_DOWN]) motion->accelaration = 0;
            else if (keys[SDL_SCANCODE_UP] && !keys[SDL_SCANCODE_DOWN]) motion->accelaration = ACCELERATION;
            else if (!keys[SDL_SCANCODE_UP] && keys[SDL_SCANCODE_DOWN]) motion->accelaration = DEACCELERATION;
            if (!keys[SDL_SCANCODE_RIGHT] && !keys[SDL_SCANCODE_LEFT]) motion->horizontalVelocity = 0;
            else if (keys[SDL_SCANCODE_RIGHT] && !keys[SDL_SCANCODE_LEFT]) motion->horizontalVelocity = TURN_SPEED;
            else if (!keys[SDL_SCANCODE_RIGHT] && keys[SDL_SCANCODE_LEFT]) motion->horizontalVelocity = -TURN_SPEED;
            break;
        case SDL_QUIT:
            controls->quit = true;
            break;
        default: // -do nothing-
            break;
        }
    }
    return 0;
}

/* 3. MATH */

void MeasureElapsedTime(time_var* time, dev_var* dev)
{
    time->lastRefresh = time->now;
    time->now = SDL_GetPerformanceCounter() - time->onPause;
    time->delta = (time->now - time->lastRefresh) * 1000 / (double)SDL_GetPerformanceFrequency();
    time->runTime += time->delta / 1000;
    time->fTimer += time->delta;
    time->sTimer += time->delta;
    dev->refreshCount++;
}

void RunPerformanceAnalysis(time_var* time, dev_var* dev)
{
    if (time->sTimer >= 1000)
    {
        dev->refreshRate = dev->refreshCount;
        dev->refreshCount = 0;
        dev->fps = dev->frameCount;
        dev->frameCount = 0;
        time->sTimer -= 1000;
        time->timer++;
        time->timer %= 2;
    }
}

void PlayersMovement(const time_var time, motion_var* motion)
{
    if (time.deathTimer == 4)
    {
        if (motion->accelaration == 0)
        {
            if (motion->velocity < DEFAULT_VELOCITY - 1) motion->velocity += DRAG * time.delta / 1000;
            else if (motion->velocity > DEFAULT_VELOCITY + 1) motion->velocity -= DRAG * time.delta / 1000;
        }

        if ((motion->velocity > MIN_VELOCITY || motion->accelaration > 0) && (motion->velocity < MAX_VELOCITY || motion
            ->accelaration < 0))
        {
            motion->velocity += motion->accelaration * time.delta / 1000;
        }

        motion->horizontalOffset += motion->horizontalVelocity * time.delta / 1000;
    }
    else if (motion->velocity > 0) motion->velocity += 2 * DEACCELERATION * time.delta / 1000;
    else motion->velocity = 0;

    motion->distance += motion->velocity * time.delta / 1000;

    motion->verticalOffset = motion->velocity / 8;
    motion->realVelocity = motion->velocity / SCREEN_HEIGHT * SH_IN_IN_GAME_METERS * 3.6;
    motion->realDistance = motion->distance / SCREEN_HEIGHT * SH_IN_IN_GAME_METERS / 1000;
}

void BotsMovement(const motion_var motion, const time_var time, bot_var* bot, const shot_var shot, enemy_var* enemy)
{
    bot->civilianOffset += (motion.velocity - bot->civilianVelocity) * time.delta / 1000;
    bot->UpdateCivilian(motion, shot);

    enemy->enemyOffset += (motion.velocity - enemy->enemyVelocity) * time.delta / 1000;
    enemy->Update(motion, shot);
}

void ShotMotion(const motion_var* motion, const time_var time, shot_var* shot)
{
    if (shot->shotShot)
    {
        shot->shotOffset += -shot->shotVelocity * time.delta / 1000;
        shot->shotXPos = PLAYER_X_POS;
        shot->UpdateShotPos(motion);
    }
    else
    {
        shot->shot.x = PLAYER_X_POS;
        shot->shot.y = PLAYER_Y_POS;
    }
}

void InitializeRoadGeneration(const motion_var* motion, environment_var* environment)
{
    if (motion->gameStarted && (int)motion->distance % DISTANCE_BETWEEN_TURNS < 50)
    // less than 50 as a room for error (low performance)
    {
        if (!environment->roadTypeDrawn)
        {
            // ROAD TYPE DRAW
            environment->previousRoadType = environment->roadType;
            environment->roadType = SDL_GetTicks() % 3 - 1;
            environment->roadTypeDrawn = true;

            // SETTING UP ROAD TYPE
            switch (environment->roadType)
            {
            case -1:
                environment->targetRoadOffset = -ROAD_SPREAD;
                break;
            default:
                environment->targetRoadOffset = 0;
                break;
            case 1:
                environment->targetRoadOffset = ROAD_SPREAD;
                break;
            }

            // CALCULATING TURN LENGTH
            if (abs(environment->roadType - environment->previousRoadType) == 2)
            {
                environment->turnLength = ROAD_SPREAD / TURN_SHARPNESS * 2;
            }
            else
            {
                environment->turnLength = ROAD_SPREAD / TURN_SHARPNESS;
            }
        }
    }
}

void RoadGeneration(const motion_var* motion, environment_var* environment)
{
    InitializeRoadGeneration(motion, environment);

    environment->distanceSinceTurnStart = (int)motion->distance % DISTANCE_BETWEEN_TURNS;
    environment->distanceSinceTurnEnd = (int)motion->distance % DISTANCE_BETWEEN_TURNS - environment->turnLength;
    if (environment->distanceSinceTurnEnd > SCREEN_HEIGHT) // if turn passed
    {
        environment->distanceSinceTurnEnd = SCREEN_HEIGHT;
        environment->roadTypeDrawn = false;
    }
}

int Death(const sdl_init_var* sdl, const assets_loaded assets, control_var* controls, time_var* time, dev_var* dev,
          motion_var* motion, environment_var* environment,
          bot_var* bot, shot_var* shot, enemy_var* enemy)
{
    if (motion->offRoad == 2)
    {
        controls->dead = true;
    }

    if (controls->dead)
    {
        if (time->timer == 1 && !time->timerLock)
        {
            time->deathTimer--;
            time->timerLock = true;
        }
        else if (time->timer == 0 && time->timerLock)
        {
            time->deathTimer--;
            time->timerLock = false;
        }

        if (time->deathTimer == 0)
        {
            if (SaveScore(sdl, assets, controls)) return 1;
            ResetAll(controls, time, dev, motion, environment, bot, shot, enemy);
        }
    }
    return 0;
}

void GunShot(time_var* time, shot_var* shot, const motion_var* motion)
{
    if (shot->shotShot)
    {
        if (time->shotTimer == 0 && ((time->timer == 1 && !time->timerLock) || (time->timer == 0 && time->timerLock)))
        {
            shot->shotShot = false;
            time->shotTimer = 1;
            shot->Reset(motion);
        }
        else if (time->timer == 1 && !time->timerLock)
        {
            time->shotTimer--;
            time->timerLock = true;
        }
        else if (time->timer == 0 && time->timerLock)
        {
            time->shotTimer--;
            time->timerLock = false;
        }
    }
}

int RunCalculations(const sdl_init_var* sdl, const assets_loaded assets, control_var* controls, time_var* time,
                    dev_var* dev, motion_var* motion,
                    environment_var* environment, bot_var* bot, shot_var* shot, enemy_var* enemy)
{
    PlayersMovement(*time, motion);
    BotsMovement(*motion, *time, bot, *shot, enemy);
    if (motion->distance >= DISTANCE_BETWEEN_TURNS) motion->gameStarted = true;
    RoadGeneration(motion, environment);
    if (motion->offRoad == 0 && bot->isAlive)
    {
        controls->score += motion->velocity * time->delta / 1000;
        if (!enemy->isAlive) controls->score += motion->velocity * time->delta / 1000;
    }
    if (Death(sdl, assets, controls, time, dev, motion, environment, bot, shot, enemy)) return 1;
    GunShot(time, shot, motion);
    ShotMotion(motion, *time, shot);

    return 0;
}

/* 4. GRAPHICS */

int LoadBMP(const sdl_init_var* sdl, const assets_loaded assets, SDL_Surface** bmp, char* fileName)
{
    char filePath[32]; // fileName max length: 18 characters
    snprintf(filePath, sizeof filePath, "%s%s%s", "./assets/", fileName, ".bmp");
    *bmp = SDL_LoadBMP(filePath);
    if (bmp == nullptr)
    {
        printf("SDL_LoadBMP(%s.bmp) error: %s\n", fileName, SDL_GetError());
        QuitApp(sdl, assets);
        return 1;
    }
    return 0;
}

int LoadAssets(const sdl_init_var* sdl, assets_loaded* assets)
{
    char text[64];

    // CHARSET - a 128x128 bitmap containing 8x8 character images
    snprintf(text, sizeof text, "cs8x8");
    if (assets->createSurface(sdl, *assets, text)) return 1;

    // GRASS
    snprintf(text, sizeof text, "grass");
    if (assets->createSurface(sdl, *assets, text)) return 1;

    // PLAYER'S CAR
    snprintf(text, sizeof text, "car");
    if (assets->createSurface(sdl, *assets, text)) return 1;
    // ENEMY CAR
    snprintf(text, sizeof text, "car_enemy");
    if (assets->createSurface(sdl, *assets, text)) return 1;
    // CIVILIAN CAR
    snprintf(text, sizeof text, "car_civilian");
    if (assets->createSurface(sdl, *assets, text)) return 1;

    // EXPLOSION
    snprintf(text, sizeof text, "boom0");
    if (assets->createSurface(sdl, *assets, text)) return 1;
    snprintf(text, sizeof text, "boom");
    if (assets->createSurface(sdl, *assets, text)) return 1;

    // FIREBALL
    snprintf(text, sizeof text, "fireball");
    if (assets->createSurface(sdl, *assets, text)) return 1;
    return 0;
}

void DrawString(SDL_Surface* screen, int x, const int y, const char* text, SDL_Surface* charset)
{
    SDL_Rect src, dest;
    src.w = 8;
    src.h = 8;
    dest.w = 8;
    dest.h = 8;
    while (*text)
    {
        const int c = *text & 255;
        const int px = (c % 16) * 8;
        const int py = (c / 16) * 8;
        src.x = px;
        src.y = py;
        dest.x = x;
        dest.y = y;
        SDL_BlitSurface(charset, &src, screen, &dest);
        x += 8;
        text++;
    }
}

void DrawSurface(SDL_Surface* screen, SDL_Surface* sprite, const int x, const int y)
{
    SDL_Rect dest;
    dest.x = x - sprite->w / 2;
    dest.y = y - sprite->h / 2;
    dest.w = sprite->w;
    dest.h = sprite->h;
    SDL_BlitSurface(sprite, nullptr, screen, &dest); // copies a surface to another surface
}

void DrawPixel(const SDL_Surface* surface, const int x, const int y, const Uint32 color)
{
    const int bpp = surface->format->BytesPerPixel;
    Uint8* p = (Uint8*)surface->pixels + y * surface->pitch + x * bpp;
    *(Uint32*)p = color;
}

void DrawLine(const SDL_Surface* screen, int x, int y, const int l, const int dx, const int dy, const Uint32 color)
{
    for (int i = 0; i < l; i++)
    {
        DrawPixel(screen, x, y, color);
        x += dx;
        y += dy;
    }
}

void DrawRectangle(const SDL_Surface* screen, const int x, const int y, const int l, const int k,
                   const Uint32 outlineColor, const Uint32 fillColor)
{
    DrawLine(screen, x, y, k, 0, 1, outlineColor);
    DrawLine(screen, x + l - 1, y, k, 0, 1, outlineColor);
    DrawLine(screen, x, y, l, 1, 0, outlineColor);
    DrawLine(screen, x, y + k - 1, l, 1, 0, outlineColor);
    for (int i = y + 1; i < y + k - 1; i++)
    {
        DrawLine(screen, x + 1, i, l - 2, 1, 0, fillColor);
    }
}

void DrawGrass(const sdl_init_var sdl, const assets_loaded assets, motion_var* motion, environment_var environment)
{
    environment.UpdateGrassPos(motion->distance);
    environment.DrawGrass(sdl, assets);
}

void DrawRoad(const sdl_init_var sdl, motion_var* motion, environment_var environment, const colors colors,
              bot_var* bot, enemy_var* enemy)
{
    double tempRoadOffset = 0;

    if (environment.roadType != environment.previousRoadType)
    {
        switch (environment.previousRoadType)
        {
        case -1:
            environment.previousTargetRoadOffset = -ROAD_SPREAD;
            break;
        case 1:
            environment.previousTargetRoadOffset = ROAD_SPREAD;
            break;
        default:
            environment.previousTargetRoadOffset = 0;
            break;
        }

        tempRoadOffset = environment.previousTargetRoadOffset;

        switch (environment.roadType - environment.previousRoadType)
        {
        case -2:
        case -1:
            tempRoadOffset -= environment.distanceSinceTurnStart * TURN_SHARPNESS;
            break;
        case 1:
        default: //case 2
            tempRoadOffset += environment.distanceSinceTurnStart * TURN_SHARPNESS;
            break;
        }
    }

    double tempDistanceSinceTurnEnd = environment.distanceSinceTurnEnd;
    for (int i = 0; i < SCREEN_HEIGHT; i++)
    {
        if (environment.roadType == environment.previousRoadType)
        {
            tempRoadOffset = environment.targetRoadOffset;
        }
        else
        {
            if (tempDistanceSinceTurnEnd > 0)
            {
                tempRoadOffset = environment.targetRoadOffset;
                tempDistanceSinceTurnEnd--;
            }
            else
            {
                switch (environment.roadType - environment.previousRoadType)
                {
                case -2:
                case -1:
                    if (tempRoadOffset < environment.previousTargetRoadOffset) tempRoadOffset += TURN_SHARPNESS;
                    break;
                case 1:
                default: //case 2
                    if (tempRoadOffset > environment.previousTargetRoadOffset) tempRoadOffset -= TURN_SHARPNESS;
                    break;
                }
            }
        }
        DrawLine(sdl.screen, sdl.screen->w / 2 - BASE_ROAD_SIZE / 2 + tempRoadOffset, i, BASE_ROAD_SIZE, 1, 0,
                 DARK_GREY);

        // GENERATING BOTS MOVEMENT
        int botCivilYPos = bot->civilian.y;
        if (bot->civilian.y < 0) botCivilYPos = 0;
        else if (bot->civilian.y >= SCREEN_HEIGHT) botCivilYPos = SCREEN_HEIGHT - 1;
        if (botCivilYPos == i)
        {
            bot->civilian.x = bot->civilianXPos + tempRoadOffset;
        }

        // ENEMY
        int botEnemyYPos = enemy->enemy.y;
        if (enemy->enemy.y < 0) botEnemyYPos = 0;
        else if (enemy->enemy.y >= SCREEN_HEIGHT) botEnemyYPos = SCREEN_HEIGHT - 1;
        if (botEnemyYPos == i)
        {
            enemy->enemy.x = enemy->enemyXPos + tempRoadOffset;
        }

        // CHECKING IF PLAYER IS ON ROAD
        if (PLAYER_Y_POS == i)
        {
            if (PLAYER_X_POS > SCREEN_WIDTH / 2 + tempRoadOffset + BASE_ROAD_SIZE / 2 + 75 || PLAYER_X_POS <
                SCREEN_WIDTH / 2 + tempRoadOffset - BASE_ROAD_SIZE / 2 - 75)
            {
                motion->offRoad = 2;
            }
            else if (PLAYER_X_POS > SCREEN_WIDTH / 2 + tempRoadOffset + BASE_ROAD_SIZE / 2 - 30 || PLAYER_X_POS <
                SCREEN_WIDTH / 2 + tempRoadOffset - BASE_ROAD_SIZE / 2 + 30)
            {
                motion->offRoad = 1;
            }
            else motion->offRoad = 0;
        }
    }
}

void DrawEnvironment(const sdl_init_var sdl, const assets_loaded assets, motion_var* motion,
                     environment_var environment, const colors colors, bot_var* bot, enemy_var* enemy)
{
    DrawGrass(sdl, assets, motion, environment);
    DrawRoad(sdl, motion, environment, colors, bot, enemy);
}

void DrawBotCars(const sdl_init_var sdl, const assets_loaded assets, time_var* time, bot_var* bot, enemy_var* enemy)
{
    DrawSurface(sdl.screen, CIVILIAN_CAR_SURFACE, bot->civilian.x, bot->civilian.y);
    if (!bot->isAlive)
    {
        if (time->civilianDeathTimer == 0 && ((time->timer == 1 && !time->timerLock) || (time->timer == 0 && time->
            timerLock)))
        {
            time->civilianDeathTimer = 2;
            bot->Reset();
        }
        else if (time->timer == 1 && !time->timerLock)
        {
            time->civilianDeathTimer--;
            time->timerLock = true;
        }
        else if (time->timer == 0 && time->timerLock)
        {
            time->civilianDeathTimer--;
            time->timerLock = false;
        }
        DrawSurface(sdl.screen, EXPLOSION_SURFACE, bot->civilian.x, bot->civilian.y);
    }

    DrawSurface(sdl.screen, ENEMY_CAR_SURFACE, enemy->enemy.x, enemy->enemy.y);
    if (!enemy->isAlive)
    {
        if (time->civilianDeathTimer == 0 && ((time->timer == 1 && !time->timerLock) || (time->timer == 0 && time->
            timerLock)))
        {
            time->civilianDeathTimer = 2;
            enemy->Reset();
        }
        else if (time->timer == 1 && !time->timerLock)
        {
            time->civilianDeathTimer--;
            time->timerLock = true;
        }
        else if (time->timer == 0 && time->timerLock)
        {
            time->civilianDeathTimer--;
            time->timerLock = false;
        }
        DrawSurface(sdl.screen, EXPLOSION_SURFACE, enemy->enemy.x, enemy->enemy.y);
    }
}

void DrawPlayersCar(const sdl_init_var sdl, const assets_loaded assets, time_var time, const motion_var* motion,
                    shot_var shot)
{
    if (shot.shotShot)
    {
        DrawSurface(sdl.screen, FIREBALL_SURFACE, shot.shot.x, shot.shot.y);
    }

    int shake = 0;
    if (motion->offRoad == 1)
    {
        shake = SDL_GetTicks() % 6;
    }
    if (time.deathTimer > 1) DrawSurface(sdl.screen, PLAYER_CAR_SURFACE, PLAYER_X_POS + shake, PLAYER_Y_POS + shake);

    if (time.deathTimer == 3) DrawSurface(sdl.screen, EXPLOSION_START_SURFACE, PLAYER_X_POS, PLAYER_Y_POS);
    else if (time.deathTimer == 2) DrawSurface(sdl.screen, EXPLOSION_SURFACE, PLAYER_X_POS, PLAYER_Y_POS);
}

void PrintLineOfInfo(const sdl_init_var sdl, const assets_loaded assets, const char* text, int* lineCount)
{
    DrawString(sdl.screen, (SCREEN_WIDTH - STRING_WIDTH) / 2, INFO_SCREEN_TEXT_Y_POS, text, CHARSET_SURFACE);
    (*lineCount)++;
}

void DrawInfoScreen(const sdl_init_var sdl, assets_loaded assets, control_var controls, time_var time,
                    dev_var dev, motion_var motion, colors colors, shot_var shot)
{
    char text[64];
    int lineCount = 0;
    if (controls.toggleLoadSave)
    {
        int linesOfText = 6; // SET ACCORDINGLY !!!
        DrawRectangle(sdl.screen, (SCREEN_WIDTH - INFO_SCREEN_WIDTH) / 2, INFO_SCREEN_MARGIN,
                      INFO_SCREEN_WIDTH, INFO_SCREEN_HEIGHT, RED, BLACK);
        snprintf(text, sizeof text, "CHOOSE YOUR SAVE:");
        PrintLineOfInfo(sdl, assets, text, &lineCount);
        snprintf(text, sizeof text, "1. %s", controls.save1);
        PrintLineOfInfo(sdl, assets, text, &lineCount);
        snprintf(text, sizeof text, "2. %s", controls.save2);
        PrintLineOfInfo(sdl, assets, text, &lineCount);
        snprintf(text, sizeof text, "3. %s", controls.save3);
        PrintLineOfInfo(sdl, assets, text, &lineCount);
        snprintf(text, sizeof text, "4. %s", controls.save4);
        PrintLineOfInfo(sdl, assets, text, &lineCount);
        snprintf(text, sizeof text, "5. %s", controls.save5);
        PrintLineOfInfo(sdl, assets, text, &lineCount);
    }
    else if (controls.toggleRecordsInfo)
    {
        int linesOfText = 3; // SET ACCORDINGLY !!!
        DrawRectangle(sdl.screen, (SCREEN_WIDTH - INFO_SCREEN_WIDTH) / 2, INFO_SCREEN_MARGIN,
                      INFO_SCREEN_WIDTH, INFO_SCREEN_HEIGHT, RED, BLACK);
        snprintf(text, sizeof text, "HIGHEST SCORE:");
        PrintLineOfInfo(sdl, assets, text, &lineCount);
        snprintf(text, sizeof text, "%s", controls.highestScore);
        PrintLineOfInfo(sdl, assets, text, &lineCount);
        snprintf(text, sizeof text, "%s", controls.highestScoreDate);
        PrintLineOfInfo(sdl, assets, text, &lineCount);
    }
    else if (controls.toggleInfo == 1)
    {
        int linesOfText = 6; // SET ACCORDINGLY !!!
        DrawRectangle(sdl.screen, (SCREEN_WIDTH - INFO_SCREEN_WIDTH) / 2, INFO_SCREEN_MARGIN,
                      INFO_SCREEN_WIDTH, INFO_SCREEN_HEIGHT, RED, BLACK);
        snprintf(text, sizeof text, "Michal Zurawski 193287");
        PrintLineOfInfo(sdl, assets, text, &lineCount);
        snprintf(text, sizeof text, "score: %d   ---   time: %.1lf s", controls.score, time.runTime);
        PrintLineOfInfo(sdl, assets, text, &lineCount);
        snprintf(text, sizeof text, "refresh rate: %d   fps: %d", dev.refreshRate, dev.fps);
        PrintLineOfInfo(sdl, assets, text, &lineCount);
        snprintf(text, sizeof text, "speed: %.1f km/h   distance: %.2f km", motion.realVelocity, motion.realDistance);
        PrintLineOfInfo(sdl, assets, text, &lineCount);
        if (shot.shotShot) snprintf(text, sizeof text, "RELOADING...");
        else snprintf(text, sizeof text, "SHOOT!");
        PrintLineOfInfo(sdl, assets, text, &lineCount);
        if (motion.accelaration > 0) snprintf(text, sizeof text, "GAS");
        else if (motion.accelaration < 0) snprintf(text, sizeof text, "BREAK");
        else snprintf(text, sizeof text, "-");
        PrintLineOfInfo(sdl, assets, text, &lineCount);
    }
    else if (controls.toggleControlsInfo == 1)
    {
        int linesOfText = 13; // SET ACCORDINGLY !!!
        DrawRectangle(sdl.screen, (SCREEN_WIDTH - INFO_SCREEN_WIDTH) / 2, INFO_SCREEN_MARGIN,
                      INFO_SCREEN_WIDTH, INFO_SCREEN_HEIGHT, RED, BLACK);
        snprintf(text, sizeof text, "GAME CONTROLS");
        PrintLineOfInfo(sdl, assets, text, &lineCount);
        snprintf(text, sizeof text, "\30 - gas");
        PrintLineOfInfo(sdl, assets, text, &lineCount);
        snprintf(text, sizeof text, "\31 - break");
        PrintLineOfInfo(sdl, assets, text, &lineCount);
        snprintf(text, sizeof text, "\32 - turn left");
        PrintLineOfInfo(sdl, assets, text, &lineCount);
        snprintf(text, sizeof text, "\33 - turn right");
        PrintLineOfInfo(sdl, assets, text, &lineCount);
        snprintf(text, sizeof text, "space - shoot");
        PrintLineOfInfo(sdl, assets, text, &lineCount);
        snprintf(text, sizeof text, "tab - info");
        PrintLineOfInfo(sdl, assets, text, &lineCount);
        snprintf(text, sizeof text, "r - show highest score");
        PrintLineOfInfo(sdl, assets, text, &lineCount);
        snprintf(text, sizeof text, "p - pause");
        PrintLineOfInfo(sdl, assets, text, &lineCount);
        snprintf(text, sizeof text, "n - new game");
        PrintLineOfInfo(sdl, assets, text, &lineCount);
        snprintf(text, sizeof text, "s - save game");
        PrintLineOfInfo(sdl, assets, text, &lineCount);
        snprintf(text, sizeof text, "l - load game");
        PrintLineOfInfo(sdl, assets, text, &lineCount);
        snprintf(text, sizeof text, "esc - quit");
        PrintLineOfInfo(sdl, assets, text, &lineCount);
    }
}

void DrawImplementedList(const sdl_init_var sdl, const assets_loaded assets, const colors colors)
{
    char text[64];
    snprintf(text, sizeof text, "ABCDEFGHIJKO");
    DrawRectangle(sdl.screen, IMPLEMENTED_LIST_X_POS, IMPLEMENTED_LIST_Y_POS,
                  IMPLEMENTED_LIST_WIDTH, IMPLEMENTED_LIST_HEIGHT, RED, BLACK);
    DrawString(sdl.screen, IMPLEMENTED_LIST_TEXT_X_POS, IMPLEMENTED_LIST_TEXT_Y_POS, text, CHARSET_SURFACE);
}

void Render(const sdl_init_var sdl)
{
    SDL_UpdateTexture(sdl.scrtex, nullptr, sdl.screen->pixels, sdl.screen->pitch);
    SDL_RenderCopy(sdl.renderer, sdl.scrtex, nullptr, nullptr);
    SDL_RenderPresent(sdl.renderer);
}