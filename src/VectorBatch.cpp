#include "stdafx.h"
#include "rlgl.h"
#include "VectorBatch.h"

#define BATCH_SIZE 1200

Vector2 texCoords[6] = {(Vector2) {1, -1}, (Vector2) {1, 1}, (Vector2) {-1, 1}, (Vector2) {-1, 1}, (Vector2) {-1, -1}, (Vector2) {1, -1}};

VectorObject::VectorObject() : _thick(0), _width(0), _height(0), _solid(false), _s(1), _cx(0), _cy(0), _x(0), _y(0) { }

void VectorObject::AddLine(float x0, float y0, float x1, float y1)
{
    lines.push_back((Vector4) {x0, y0, x1, y1});
}

void VectorObject::RenderLine(float x0, float y0, float x1, float y1, float thick)
{
    float dx  = x1 - x0;
    float dy  = y1 - y0;
    float len = sqrtf(dx * dx + dy * dy);
    dx /= len;
    dy /= len;

    // extend each line by half thick
    x0 -= dx * 0.5f * thick;
    x1 += dx * 0.5f * thick;
    y0 -= dy * 0.5f * thick;
    y1 += dy * 0.5f * thick;

    const float px = 0.5f * thick * (-dy);
    const float py = 0.5f * thick * dx;

    vertices.push_back((Vector2) {x0 + px, y0 + py});
    vertices.push_back((Vector2) {x1 + px, y1 + py});
    vertices.push_back((Vector2) {x1 - px, y1 - py});

    vertices.push_back((Vector2) {x1 - px, y1 - py});
    vertices.push_back((Vector2) {x0 - px, y0 - py});
    vertices.push_back((Vector2) {x0 + px, y0 + py});

    len += thick;
    len = thick / len;
    lengths.push_back(len);
    lengths.push_back(len);
    lengths.push_back(len);
    lengths.push_back(len);
    lengths.push_back(len);
    lengths.push_back(len);
}

void VectorObject::RenderSolid()
{
    // render triangles vs center
    for(unsigned i = 0; i < lines.size(); i++)
    {
        float x0 = _s * (lines[i].x - _cx);
        float y0 = _s * (lines[i].y - _cy);
        float x1 = _s * (lines[i].z - _cx);
        float y1 = _s * (lines[i].w - _cy);

        solids.push_back((Vector2) {x0, y0});
        solids.push_back((Vector2) {x1, y1});
        solids.push_back((Vector2) {0, 0});
    }
}

void VectorObject::Render(float thick)
{
    vertices.clear();

    if(_solid)
        RenderSolid();

    float maxX = -1000000, minX = 1000000;
    float maxY = -1000000, minY = 1000000;
    for(unsigned i = 0; i < lines.size(); i++)
    {
        float x0 = _s * (lines[i].x - _cx);
        float y0 = _s * (lines[i].y - _cy);
        float x1 = _s * (lines[i].z - _cx);
        float y1 = _s * (lines[i].w - _cy);
        RenderLine(x0, y0, x1, y1, thick);
        maxX = std::max(maxX, std::max(x0, x1));
        minX = std::min(minX, std::min(x0, x1));
        maxY = std::max(maxY, std::max(y0, y1));
        minY = std::min(minY, std::min(y0, y1));
    }
    if(_width == 0) // unless overriden
    {
        _width = maxX - minX;
    }
    if(_height == 0) // unless overriden
    {
        _height = maxY - minY;
    }
    _thick = thick;
}

void VectorObject::Draw(Color color)
{
    if(solids.size())
    {
        rlCheckRenderBatchLimit(solids.size());

        for(unsigned i = 0; i < solids.size(); i++)
        {
            rlTexCoord2f(0, 0);
            rlColor4ub(color.r, color.g, color.b, color.a);
            rlVertex3f(solids[i].x, solids[i].y, 0);
        }
    }

    unsigned bc = BATCH_SIZE;
    for(unsigned i = 0; i < vertices.size(); i++)
    {
        if(bc++ == BATCH_SIZE)
        {
            rlCheckRenderBatchLimit(BATCH_SIZE);
            bc = 0;
        }
        const auto& tc = texCoords[i % 6];
        rlTexCoord2f(tc.x, tc.y);
        rlColor4ub(color.r, color.g, color.b, color.a);
        rlVertex3f(vertices[i].x, vertices[i].y, lengths[i]);
    }
}

void VectorObject::moveTo(double x, double y)
{
    _x = (float)x;
    _y = (float)y;
}

void VectorObject::lineTo(double x, double y)
{
    AddLine((float)_x, (float)_y, (float)x, (float)y);
    moveTo(x, y);
}

void VectorObject::Flip(int x, int y)
{
    for(unsigned i = 0; i < lines.size(); i++)
    {
        if(x)
        {
            lines[i].x *= -1;
            lines[i].z *= -1;
        }
        if(y)
        {
            lines[i].y *= -1;
            lines[i].w *= -1;
        }
    }
}

void VectorObject::Center(float x, float y)
{
    _cx = x;
    _cy = y;
}

void VectorObject::CenterY(float y)
{
    _cy = y;
}

void VectorObject::Scale(float s)
{
    _s = s;
}

void VectorObject::Center()
{
    if(!lines.size())
        return;

    float maxX = -1000000, minX = 1000000, maxY = -1000000, minY = 1000000;
    for(unsigned i = 0; i < lines.size(); i++)
    {
        minX = std::min(minX, std::min(lines[i].x, lines[i].z));
        minY = std::min(minY, std::min(lines[i].y, lines[i].w));
        maxX = std::max(maxX, std::max(lines[i].x, lines[i].z));
        maxY = std::max(maxY, std::max(lines[i].y, lines[i].w));
    }
    _cx = (maxX + minX) / 2;
    _cy = (maxY + minY) / 2;
}

VectorObject* VectorObject::Rectangle(float w, float h)
{
    auto vo = new VectorObject();
    vo->lineTo(w, 0);
    vo->lineTo(w, h);
    vo->lineTo(0, h);
    vo->lineTo(0, 0);
    vo->Center();
    return vo;
}

VectorObject* VectorObject::Triangle(float w, float h)
{
    auto vo = new VectorObject();
    vo->lineTo(w, 0);
    vo->lineTo(w / 2, h);
    vo->lineTo(0, 0);
    vo->Center();
    return vo;
}

VectorObject* VectorObject::Regular(float r, int n, int dotted)
{
    auto vo = new VectorObject();
    vo->moveTo(0, r);
    if(dotted)
    {
        for(int i = 1; i < n + 1; i += 2)
        {
            float x  = r * sin(6.28318531f * i / n);
            float y  = r * cos(6.28318531f * i / n);
            float px = r * sin(6.28318531f * (i - 1) / n);
            float py = r * cos(6.28318531f * (i - 1) / n);
            vo->AddLine(px, py, x, y);
            //            vo->lineTo(x, y);
        }
    }
    else
    {
        for(int i = 1; i < n + 1; i++)
        {
            float x = r * sin(6.28318531f * i / n);
            float y = r * cos(6.28318531f * i / n);
            vo->lineTo(x, y);
        }
    }
    vo->Center();
    return vo;
}

VectorObject* VectorObject::Ellipse(float r1, float r2, int n)
{
    auto vo = new VectorObject();
    vo->moveTo(0, r1);
    for(int i = 1; i < n + 1; i++)
    {
        // i = 0 to n
        // need 0 to 0; with 1 in the middle
        float rr = 1.0f - fabsf((2.0f * i / n) - 1.0f);
        float r  = r1 + (r2 - r1) * rr;
        float x  = r * sin(6.28318531f * i / n);
        float y  = r * cos(6.28318531f * i / n);
        vo->lineTo(x, y);
    }
    vo->Center();
    return vo;
}

VectorObject* VectorObject::Circle(float r)
{
    return Regular(r, 32, 0);
}

VectorObject* VectorObject::DottedCircle(float r)
{
    return Regular(r, 64, 1);
}

VectorBatch::VectorBatch() : _thick(0.02f) { }

void VectorBatch::Begin()
{
    //rlSetBlendMode(BLEND_ADDITIVE);
    rlDisableBackfaceCulling();
    rlDisableDepthTest();
    rlSetBlendFactors(0, 0, 0x8008); // GL_MAX
    rlSetBlendMode(BLEND_CUSTOM);
    rlBegin(RL_TRIANGLES);
}

void VectorBatch::DrawLine(float sx, float sy, float ex, float ey, Color color)
{
    // ad-hoc line
    VectorObject lo;
    lo.AddLine(sx, sy, ex, ey);
    lo.Render(_thick);
    lo.Draw(color);
}

void VectorBatch::DrawObject(VectorObject* obj, Vector2 pos, float rot, Color color)
{
    rlPushMatrix();
    rlTranslatef(pos.x, pos.y, 0);
    rlRotatef(rot, 0, 0, 1);

    if(obj->_thick != _thick)
        obj->Render(_thick);

    obj->Draw(color);
    rlPopMatrix();
}

void VectorBatch::Thick(float thick)
{
    _thick = thick;
}

void VectorBatch::End()
{
    rlEnd();
    rlSetBlendMode(BLEND_ALPHA);
}
