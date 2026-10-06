/**
* Author: Jason Legarda
* Assignment: Simple 2D Scene
* Date due: 10/05/2026
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/

#include "raylib.h"
#include "CS3113/cs3113.h"
#include <cmath>

// Global Constants
constexpr int   SCREEN_WIDTH  = 1200,
                SCREEN_HEIGHT = 675,
                FPS           = 60;

constexpr char  MESSI_FP[]    = "assets/messi.png";
constexpr char  BALL_FP[]     = "assets/ball.png";
constexpr char  NET_FP[]      = "assets/net.png";
constexpr char  GRASS_FP[]    = "assets/grass.png";
constexpr char  VICTORY_FP[]  = "assets/victory.png";

constexpr float RUN_DURATION         = 5.25f;
constexpr float KICK_DURATION        = 1.75f;
constexpr float CELEBRATION_DURATION = 4.0f;
constexpr float CYCLE_DURATION       = RUN_DURATION + KICK_DURATION + CELEBRATION_DURATION;

constexpr float GRASS_HEIGHT = 180.0f;


constexpr Vector2 MESSI_BASE_SIZE = { 160.0f, 220.0f };
constexpr float   MESSI_RUN_Y     = SCREEN_HEIGHT - GRASS_HEIGHT + 30.0f - MESSI_BASE_SIZE.y * 0.5f;
constexpr Vector2 MESSI_START     = { 60.0f,                 MESSI_RUN_Y };
constexpr Vector2 MESSI_KICK_SPOT = { SCREEN_WIDTH * 0.35f,  MESSI_RUN_Y };
constexpr float   MESSI_BOB_AMP   = 18.0f;
constexpr float   MESSI_STEPS_PER_RUN = 10.0f;
constexpr float   MESSI_LEAN_DEG  = 6.0f;
constexpr float   MESSI_KICK_SWING = 15.0f;

constexpr Vector2 BALL_BASE_SIZE        = { 55.0f, 55.0f };
constexpr Vector2 BALL_FOOT_OFFSET      = { 55.0f, 90.0f };
constexpr float   BALL_DRIBBLE_WOBBLE_X = 20.0f;
constexpr float   BALL_DRIBBLE_BOUNCE_Y = 12.0f;
constexpr float   BALL_DRIBBLE_FREQ     = 12.0f;
constexpr float   BALL_SPIN_SPEED       = 720.0f;
constexpr float   BALL_PULSE_AMP        = 10.0f;
constexpr float   BALL_PULSE_FREQ       = 6.0f;

constexpr Vector2 NET_BASE_SIZE  = { 450.0f, 300.0f };
constexpr Vector2 NET_ANCHOR     = { SCREEN_WIDTH - NET_BASE_SIZE.x * 0.5f - 15.0f,
                                     SCREEN_HEIGHT - GRASS_HEIGHT - NET_BASE_SIZE.y * 0.5f + 70.0f };
constexpr float   NET_SWAY_RADIUS = 3.0f;
constexpr float   NET_SWAY_SPEED  = 2.5f;

constexpr Vector2 VICTORY_POSITION      = { SCREEN_WIDTH / 2.0f, 130.0f };
constexpr Vector2 VICTORY_FULL_SIZE     = { 500.0f, 500.0f };
constexpr float   VICTORY_SCALE_IN_TIME = 0.4f;
constexpr float   VICTORY_WOBBLE_RADIUS = 20.0f;
constexpr float   VICTORY_WOBBLE_SPEED  = 3.0f;

// Global Variables
AppStatus gAppStatus = RUNNING;

Texture2D gMessiTexture;
Texture2D gBallTexture;
Texture2D gNetTexture;
Texture2D gGrassTexture;
Texture2D gVictoryTexture;

float gPreviousTicks = 0.0f;
float gCycleTime     = 0.0f;

// Messi state
Vector2 gMessiPosition = MESSI_START;
float   gMessiAngle    = 0.0f;

// Ball state
Vector2 gBallPosition  = { MESSI_START.x + BALL_FOOT_OFFSET.x,
                           MESSI_START.y + BALL_FOOT_OFFSET.y };
Vector2 gBallScale     = BALL_BASE_SIZE;
float   gBallSpinAngle = 0.0f;

// Net state
Vector2 gNetPosition  = NET_ANCHOR;
float   gNetSwayAngle = 0.0f;

// Victory banner state
Vector2 gVictoryPosition = VICTORY_POSITION;
Vector2 gVictoryScale    = { 0.0f, 0.0f };
bool    gShowVictory     = false;

// Function Declarations
void initialise();
void processInput();
void update();
void render();
void shutdown();

void drawTexture(Texture2D tex, Vector2 position, Vector2 size, float angleDeg);
void drawGrassStrip();

// Function Definitions
void initialise()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Project 1 - Simple 2D Scene");

    gMessiTexture   = LoadTexture(MESSI_FP);
    gBallTexture    = LoadTexture(BALL_FP);
    gNetTexture     = LoadTexture(NET_FP);
    gGrassTexture   = LoadTexture(GRASS_FP);
    gVictoryTexture = LoadTexture(VICTORY_FP);

    SetTargetFPS(FPS);
}

void processInput()
{
    if (WindowShouldClose()) gAppStatus = TERMINATED;
}

void update()
{
    float ticks     = static_cast<float>(GetTime());
    float deltaTime = ticks - gPreviousTicks;
    gPreviousTicks  = ticks;

    gCycleTime += deltaTime;
    if (gCycleTime > CYCLE_DURATION) gCycleTime -= CYCLE_DURATION;

    float kickStartTime  = RUN_DURATION;
    float celebStartTime = RUN_DURATION + KICK_DURATION;

    if (gCycleTime < kickStartTime)
    {
        float runProgress = gCycleTime / RUN_DURATION;
        float stridePhase = runProgress * MESSI_STEPS_PER_RUN * 2.0f * PI;

        gMessiPosition = {
            MESSI_START.x + (MESSI_KICK_SPOT.x - MESSI_START.x) * runProgress,
            MESSI_RUN_Y   - MESSI_BOB_AMP * fabs(sin(stridePhase))
        };

        gMessiAngle = -MESSI_LEAN_DEG;
    }
    else if (gCycleTime < celebStartTime)
    {
        float kickProgress = (gCycleTime - kickStartTime) / KICK_DURATION;

        gMessiPosition = { MESSI_KICK_SPOT.x, MESSI_RUN_Y };
        gMessiAngle = -MESSI_LEAN_DEG + MESSI_KICK_SWING * sin(kickProgress * PI);
    }
    else
    {
        gMessiPosition = { MESSI_KICK_SPOT.x, MESSI_RUN_Y };
        gMessiAngle = -MESSI_LEAN_DEG;
    }

    gBallSpinAngle += BALL_SPIN_SPEED * deltaTime;

    float ballPulse = BALL_PULSE_AMP * cos(gCycleTime * BALL_PULSE_FREQ);
    gBallScale = {
        BALL_BASE_SIZE.x + ballPulse,
        BALL_BASE_SIZE.y + ballPulse
    };

    if (gCycleTime < kickStartTime)
    {
        float dribblePhase = gCycleTime * BALL_DRIBBLE_FREQ;

        gBallPosition = {
            gMessiPosition.x + BALL_FOOT_OFFSET.x + BALL_DRIBBLE_WOBBLE_X * sin(dribblePhase),
            gMessiPosition.y + BALL_FOOT_OFFSET.y - BALL_DRIBBLE_BOUNCE_Y * fabs(sin(dribblePhase * 2.0f))
        };
    }
    else if (gCycleTime < celebStartTime)
    {
        float kickTime   = gCycleTime - kickStartTime;
        float kickStartX = MESSI_KICK_SPOT.x + BALL_FOOT_OFFSET.x;
        float kickStartY = MESSI_KICK_SPOT.y + BALL_FOOT_OFFSET.y;

        float targetX = NET_ANCHOR.x;
        float targetY = (NET_ANCHOR.y - NET_BASE_SIZE.y * 0.5f) + 50.0f;

        float progress = kickTime / KICK_DURATION;

        gBallPosition = {
            kickStartX + (targetX - kickStartX) * progress,
            kickStartY + (targetY - kickStartY) * progress
        };
    }
    else
    {
        float targetY = (NET_ANCHOR.y - NET_BASE_SIZE.y * 0.5f) + 50.0f;
        gBallPosition = { NET_ANCHOR.x, targetY };
    }

    gNetSwayAngle += NET_SWAY_SPEED * deltaTime;

    gNetPosition = {
        NET_ANCHOR.x + NET_SWAY_RADIUS * cos(gNetSwayAngle),
        NET_ANCHOR.y + NET_SWAY_RADIUS * sin(gNetSwayAngle)
    };

    if (gCycleTime < celebStartTime)
    {
        gShowVictory = false;
    }
    else
    {
        gShowVictory = true;
        float celebTime = gCycleTime - celebStartTime;

        float scaleProgress = fmin(celebTime / VICTORY_SCALE_IN_TIME, 1.0f);
        float eased = 1.0f - (1.0f - scaleProgress) * (1.0f - scaleProgress);

        gVictoryScale = {
            VICTORY_FULL_SIZE.x * eased,
            VICTORY_FULL_SIZE.y * eased
        };

        float wobbleAngle = celebTime * VICTORY_WOBBLE_SPEED;
        gVictoryPosition = {
            VICTORY_POSITION.x + VICTORY_WOBBLE_RADIUS * cos(wobbleAngle),
            VICTORY_POSITION.y + VICTORY_WOBBLE_RADIUS * sin(wobbleAngle)
        };
    }
}

void drawTexture(Texture2D tex, Vector2 position, Vector2 size, float angleDeg)
{
    Rectangle sourceArea = {
        0.0f, 0.0f,
        static_cast<float>(tex.width),
        static_cast<float>(tex.height)
    };

    Rectangle destinationArea = {
        position.x, position.y,
        size.x, size.y
    };

    Vector2 originOffset = { size.x / 2.0f, size.y / 2.0f };

    DrawTexturePro(tex, sourceArea, destinationArea, originOffset, angleDeg, WHITE);
}

void drawGrassStrip()
{
    Rectangle sourceArea = {
        0.0f, 0.0f,
        static_cast<float>(gGrassTexture.width),
        static_cast<float>(gGrassTexture.height)
    };

    Rectangle destinationArea = {
        0.0f,
        SCREEN_HEIGHT - GRASS_HEIGHT,
        static_cast<float>(SCREEN_WIDTH),
        GRASS_HEIGHT
    };

    Vector2 originOffset = { 0.0f, 0.0f };

    DrawTexturePro(gGrassTexture, sourceArea, destinationArea, originOffset, 0.0f, WHITE);
}

void render()
{
    BeginDrawing();

    float celebStartTime = RUN_DURATION + KICK_DURATION;
    Color sky;
    if (gCycleTime < celebStartTime)
    {
        unsigned char b = static_cast<unsigned char>(205 + 30 * sin(gCycleTime * 1.5f));
        sky = Color{ 135, 206, b, 255 };
    }
    else
    {
        float celebTime = gCycleTime - celebStartTime;
        unsigned char pulse = static_cast<unsigned char>(195 + 60 * sin(celebTime * 6.0f));
        sky = Color{ pulse, pulse, 60, 255 };
    }

    ClearBackground(sky);

    drawGrassStrip();
    drawTexture(gNetTexture,   gNetPosition,   NET_BASE_SIZE, 0.0f);
    drawTexture(gMessiTexture, gMessiPosition, MESSI_BASE_SIZE, gMessiAngle);
    drawTexture(gBallTexture,  gBallPosition,  gBallScale,  gBallSpinAngle);

    if (gShowVictory)
    {
        drawTexture(gVictoryTexture, gVictoryPosition, gVictoryScale, 0.0f);
    }

    EndDrawing();
}

void shutdown()
{
    UnloadTexture(gMessiTexture);
    UnloadTexture(gBallTexture);
    UnloadTexture(gNetTexture);
    UnloadTexture(gGrassTexture);
    UnloadTexture(gVictoryTexture);

    CloseWindow();
}

int main(void)
{
    initialise();

    while (gAppStatus == RUNNING)
    {
        processInput();
        update();
        render();
    }

    shutdown();

    return 0;
}
