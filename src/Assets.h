#pragma once

#include "stdafx.h"
#include "VectorBatch.h"

constexpr float CHAR_SCALE = 0.01f;
constexpr int CHAR_RAW_MIDPOINT = 9;
constexpr float CHAR_HEIGHT = 2 * CHAR_RAW_MIDPOINT * CHAR_SCALE;
constexpr float CHAR_KERNING = 5.0f * CHAR_SCALE;
constexpr float CHAR_MIDPOINT = CHAR_RAW_MIDPOINT * CHAR_SCALE;

class Assets
{
public:
    VectorObject* GetChar(char c, int zoom);
    VectorObject* Plane();
    VectorObject* Airport(float w, float l);
    VectorObject* Outline();
    VectorObject* Inlay();

    VectorObject* Tokyo();
    VectorObject* SF();
    VectorObject* London();

    Assets();
    void Load();
    void Unload();
    void Render(float thick);

private:
    void GenerateChars();
    void AddChar(char c, VectorObject* vo);

    std::map<char, VectorObject*> _chars;
    std::map<char, VectorObject*> _chars2;
    std::map<char, VectorObject*> _chars4;
};
