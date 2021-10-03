#include "SelectScreen.h"
#include "VectorBatch.h"

SelectScreen::SelectScreen(int no, AirGame* game) : Screen(no, game)
{
    tokyo = m_game->m_assets->Tokyo();
    tokyo->Scale(tokyo->_s * 0.25f);
    sf = m_game->m_assets->SF();
    sf->Scale(sf->_s * 0.25f);
    london = m_game->m_assets->London();
    london->Scale(london->_s * 0.25f);

    mapOutline = VectorObject::Rectangle(22 * 0.25f, 18 * 0.25f);
    hoverMap   = Map::None;

    airport = VectorObject::Circle(0.05f);

    auto b = new Board(Map::None);
    for(auto a : b->exits)
    {
        if(a->type == ExitType::Airport)
        {
            float xOfs = 32.0f;
            if(a->code == "SFO" || a->code == "OAK" || a->code == "SJC")
                xOfs = -32.0f;
            else if(a->code == "NRT" || a->code == "HND" || a->code == "OKO" || a->code == "IBR")
                xOfs = 0;
            airports.push_back((Vector2) {a->location.x + xOfs, a->location.y});
        }
    }
    delete b;
}

void SelectScreen::Load()
{
    gameMusic = LoadMusicStream("resources/music.ogg");
}

void SelectScreen::Enter()
{
    PlayMusicStream(gameMusic);
}

void SelectScreen::Tick(double deltaTime, double totalTime)
{
    UpdateMusicStream(gameMusic);

    float mx, my;

    mx = m_game->m_mouseX;
    my = m_game->m_mouseY;

    hoverMap = Map::None;
    if(my >= -1.5f && my <= 5.5f)
    {
        float w2 = 22 * 0.25f / 2;
        //-8 0 8
        if(mx >= -8.0f - w2 && mx <= -8.0f + w2)
            hoverMap = Map::SF;
        else if(mx >= 0.0f - w2 && mx <= 0.0f + w2)
            hoverMap = Map::Tokyo;
        else if(mx >= 8.0f - w2 && mx <= 8.0f + w2)
            hoverMap = Map::London;
    }

    if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && hoverMap != Map::None)
    {
        m_game->StartMap(hoverMap);
    }
}

void SelectScreen::DrawBackground() { }

void SelectScreen::DrawPixels(Color* framebuffer, Rectangle* rect, bool* fullFrame) { }

void SelectScreen::DrawShapes() { }

void SelectScreen::DrawVectors()
{
    DrawText("AIROBICS", 0.0f, -7.5f, C_BLUE0, 0, 0, 4);

    DrawText("AIR TRAFFIC CONTROLLER TRAINING PROGRAMME", 0.0f, -6.0f, C_BLUE1, 0, 0, 2);
    //DrawText("ROUTE PLANES TO AVOID COLLISIONS", 0.0f, -5.5f, C_BLUE1, 0, 0, 1);

    if(m_game->m_flashOn)
        DrawText("CHOOSE TRAINING LOCATION", 0.0f, -3.5f, C_BLUE1, 0, 0, 2);

    float ofsY = 2.0f;
    Color c;

    // choose map
    c = hoverMap == Map::SF ? C_BLUE0 : C_BLUE2;
    DrawText("SAN FRANCISCO", -8.0f, -3.0f + ofsY, c, 0, 0, 2);
    m_vectorBatch.DrawObject(sf, (Vector2) {-8, 0 + ofsY}, 0, c);
    m_vectorBatch.DrawObject(mapOutline, (Vector2) {-8, 0 + ofsY}, 0, c);
    DrawText("EASY", -8.0f, 3.0f + ofsY, c, 0, 0, 2);

    c = hoverMap == Map::Tokyo ? C_BLUE0 : C_BLUE2;
    DrawText("TOKYO", 0.0f, -3.0f + ofsY, c, 0, 0, 2);
    m_vectorBatch.DrawObject(tokyo, (Vector2) {0, 0 + ofsY}, 0, c);
    m_vectorBatch.DrawObject(mapOutline, (Vector2) {0, 0 + ofsY}, 0, c);
    DrawText("MEDIUM", 0.0f, 3.0f + ofsY, c, 0, 0, 2);

    c = hoverMap == Map::London ? C_BLUE0 : C_BLUE2;
    DrawText("LONDON", 8.0f, -3.0f + ofsY, c, 0, 0, 2);
    m_vectorBatch.DrawObject(london, (Vector2) {8, 0 + ofsY}, 0, c);
    m_vectorBatch.DrawObject(mapOutline, (Vector2) {8, 0 + ofsY}, 0, c);
    DrawText("HARD", 8.0f, 3.0f + ofsY, c, 0, 0, 2);

    DrawText("@MAUSIMUS", 15.9f, 8.55f, C_BLUE3, 1, 1, 1);
    DrawText("LUDUM DARE 49", 15.9f, 8.9f, C_BLUE3, 1, 1, 1);

    m_vectorBatch.DrawObject(&m_mouseCursor, (Vector2) {m_game->m_mouseX, m_game->m_mouseY}, 0, WHITE);

    // hack in airports
    for(const auto& a : airports)
        m_vectorBatch.DrawObject(airport, (Vector2) {a.x * 0.25f, a.y * 0.25f + ofsY}, 0, C_BLUE1);

#ifdef PLATFORM_DESKTOP
    DrawText("PRESS F FOR FULLSCREEN", 0.0f, 7.8f, C_BLUE1, 0, 0, 2);
#endif
}

void SelectScreen::DrawGameOver() { }

void SelectScreen::Leave()
{
    StopMusicStream(gameMusic);
}

void SelectScreen::Unload() { }
