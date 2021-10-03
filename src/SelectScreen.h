#include "Screen.h"
#include "AirGame.h"

class SelectScreen : public Screen
{
public:
    SelectScreen(int no, AirGame* game);
    virtual void Load();
    virtual void Enter();
    virtual void Tick(double deltaTime, double totalTime);
    virtual void DrawBackground();
    virtual void DrawPixels(Color* framebuffer, Rectangle* rect, bool* fullFrame);
    virtual void DrawShapes();
    virtual void DrawVectors();
    virtual void DrawGameOver();

    virtual void Leave();
    virtual void Unload();

private:
    VectorObject*        tokyo;
    VectorObject*        sf;
    VectorObject*        london;
    VectorObject*        mapOutline;
    VectorObject*        airport;
    Map                  hoverMap;
    std::vector<Vector2> airports;
    Music                gameMusic;
};