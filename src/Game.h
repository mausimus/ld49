#pragma once

#include "stdafx.h"

constexpr int   screenWidth  = 1840;//1920;
constexpr int   screenHeight = 1035;//1080;
constexpr int   aspectH      = 16;
constexpr int   aspectV      = 9;
constexpr int   screenSize   = screenWidth * screenHeight;
constexpr float thickFactor  = 0.065f;

#define WINDOW_TITLE "Airobics"
#define FULLSCREEN_KEY KEY_F
//#define GLOW
#define TARGET_FPS 0

#define DRAW_PIXELS
#define DRAW_SHAPES
#define DRAW_BACKGROUND

class Screen;
class Assets;

class Game
{
protected:
    std::vector<Screen*> m_screens;
    Screen*              m_activeScreen;

    virtual void CreateScreens()                                = 0;
    virtual void OnTick(double deltaTime, double totalTime)     = 0;
    virtual void OnStart(double totalTime)                      = 0;
    virtual void OnSwitchScreen(int prevScreen, int nextScreen) = 0;

public:
    Assets* m_assets;

    int    m_frameCounter;
    float  m_mouseX;
    float  m_mouseY;
    double m_screenTime;

    Game();

    void Load();
    void Start(double totalTime);
    void Render(float thick);
    void Tick(double deltaTime, double totalTime);
    void DrawBackground();
    void DrawPixels(Color* framebuffer, Rectangle* rect, bool* fullFrame);
    void DrawShapes();
    void DrawVectors(float thick);
    void End();

    void SwitchScreen(int sn);
};
