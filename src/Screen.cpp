#include "Screen.h"

Screen::Screen(int no, AirGame* game) : m_game {game}, m_no {no}
{
    m_mouseCursor.AddLine(-0.20f, -0.20f, -0.10f, -0.10f);
    m_mouseCursor.AddLine(0.10f, 0.10f, 0.20f, 0.20f);

    m_mouseCursor.AddLine(-0.20f, 0.20f, -0.10f, 0.10f);
    m_mouseCursor.AddLine(0.10f, -0.10f, 0.20f, -0.20f);
}

Screen::~Screen() { }

void Screen::DrawVectors(float thick)
{
    m_vectorBatch.Thick(thick);
    m_vectorBatch.Begin();

    DrawVectors();

    m_vectorBatch.End();
}

// hAlign/vAlign state which corner (x,y) represent: (-1,-1) = top/left, (1,1) = bottom/right
void Screen::DrawText(const std::string& text, float x, float y, Color c, int hAlign, int vAlign, int zoom, int v)
{
    if(hAlign == 0 || hAlign == 1)
    {
        float tw = 0;
        for(auto ch : text)
        {
            auto vo = m_game->m_assets->GetChar(ch, zoom);
            tw += vo->_width + CHAR_KERNING * zoom;
        }
        tw -= CHAR_KERNING * zoom; // last char
        if(hAlign == 0)
        {
            if(v)
                y += tw / 2;
            else
                x -= tw / 2;
        }
        else
        {
            if(v)
                y += tw;
            else
                x -= tw;
        }
    }

    if(vAlign == -1)
    {
        if(v)
            x += zoom * CHAR_HEIGHT / 2;
        else
            y += zoom * CHAR_HEIGHT / 2;
    }
    else if(vAlign == 1)
    {
        if(v)
            x -= zoom * CHAR_HEIGHT / 2;
        else
            y -= zoom * CHAR_HEIGHT / 2;
    }

    for(auto ch : text)
    {
        auto vo = m_game->m_assets->GetChar(ch, zoom);
        if(v)
        {
            m_vectorBatch.DrawObject(vo, (Vector2) {x + zoom * CHAR_HEIGHT / 2, y + (vo->_width / 2)}, v ? -90.0f : 0.0f, c);
            y -= vo->_width + zoom * CHAR_KERNING;
        }
        else
        {
            m_vectorBatch.DrawObject(vo, (Vector2) {x + (vo->_width / 2), y + zoom * CHAR_HEIGHT / 2}, v ? 90.0f : 0.0f, c);
            x += vo->_width + zoom * CHAR_KERNING;
        }
    }
}