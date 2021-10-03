#pragma once

#include "AirGame.h"
#include "Assets.h"

class Screen
{
protected:
    AirGame *m_game;
    VectorBatch m_vectorBatch;
    VectorObject m_mouseCursor;

    void DrawText(const std::string& text, float x, float y, Color c, int hAlign = 0, int vAlign = 0, int zoom = 1, int v = 0);

    inline Assets* Assets() { return m_game->m_assets; }

public:
    int m_no;

    void DrawVectors(float thick);

    Screen(int no, AirGame* game);
    virtual void Load() = 0;
    virtual void Enter() = 0;
    virtual void Tick(double deltaTime, double totalTime) = 0;
    virtual void DrawBackground() = 0;
    virtual void DrawPixels(Color* framebuffer, Rectangle *rect, bool* fullFrame) = 0;
    virtual void DrawShapes() = 0;
    virtual void DrawVectors() = 0;
    virtual void Leave() = 0;
    virtual void Unload() = 0;
    virtual ~Screen();
};