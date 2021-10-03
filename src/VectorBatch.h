#pragma once

class VectorObject
{
public:
    VectorObject();
    void AddLine(float sx, float sy, float ex, float ey);
    void Center(float x, float y); // center specific coord
    void CenterY(float y); // center y only for text
    void Center(); // auto center bounding box
    void Flip(int x, int y);
    void Scale(float s); // scale
    void Render(float thick);
    void Draw(Color color);

    // Inkscape
    void moveTo(double x, double y);
    void lineTo(double x, double y);
    void stroke() { }
    void beginPath() { }

    // multiple cached versions per thickness?
    float _thick;
    float _width;
    float _height;
    bool  _solid;

    static VectorObject* Rectangle(float w, float h);
    static VectorObject* Triangle(float w, float h);
    static VectorObject* Regular(float r, int n, int d);
    static VectorObject* Ellipse(float r1, float r2, int n);
    static VectorObject* Circle(float r);
    static VectorObject* DottedCircle(float r);

    float _s; // scale
    float _cx; // center
    float _cy;

private:
    void RenderLine(float sx, float sy, float ex, float ey, float thick);
    void RenderSolid();

    float _x; // temporary during construction
    float _y;

    std::vector<Vector4> lines; // source lines
    std::vector<Vector2> vertices; // rendered triangles
    std::vector<Vector2> solids; // rendered solids
    std::vector<float>   lengths;
};

class VectorBatch
{
public:
    VectorBatch();
    void DrawObject(VectorObject* obj, Vector2 pos, float rot, Color color);
    void DrawLine(float sx, float sy, float ex, float ey, Color color); // ad-hoc, slow!
    void Begin();
    void End();

    void Thick(float thick);

    float _thick;
};
