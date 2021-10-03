#include "Screen.h"
#include "AirGame.h"

enum class MouseState
{
    Default   = 0,
    SetTarget = 1
};

class GameScreen : public Screen
{
public:
    GameScreen(int no, AirGame* game);
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
    void DrawBoard();
    void DrawPanels();
    void Restart();
    void DrawPlane(Plane* plane);
    void DrawExit(Exit* exit);
    void DrawArea(Area* area);
    void CheckPlanes();
    void CheckCollisions();
    void RemovePlane(Plane* p, PlaneMode mode);
    void PlaneLanded(Plane* p);
    void PlaneMissed(Plane* p);
    void AddDelay(Plane* p);
    void Message(const std::string& msg);
    bool IsHoveringRestartButton();
    bool IsHoveringRestartButton2();
    int  IsHoveringSpeedButton();
    int  IsHoveringFocusButton();

    Vector2 ScreenPos(Vector2 modelPos);

    float                        x_shift, y_shift, zoom;
    double                       mouse_release_time;
    Vector2                      mouse_click;
    MouseState                   mouse_state;
    double                       totalTime;
    double                       finishTime;
    double                       deltaTime;
    VectorObject*                exitObj;
    VectorObject*                airportObj;
    VectorObject*                planeObj;
    VectorObject*                planeSelected;
    VectorObject*                planeTarget;
    std::vector<std::string>     messages;
    std::map<int, VectorObject*> exitObjs;
    std::map<int, VectorObject*> areaObjs;
    Plane*                       selectedPlane;
    float                        m_delay;
    int                          m_alerts;
    int                          totalDelay;
    Sound                        landedSound;
    Sound                        ackSound;
    Sound                        alertSound;
    Sound                        tickSound;
    Sound                        startSound;
    Sound                        endSound;
    Sound                        winSound;
    float                        mx, my;
    int                          prevGameTime = 0;
    bool                         haveAlerts;
    VectorObject*                rect;
    VectorObject*                tri;
    VectorObject*                circ;
    VectorObject*                map;
    VectorObject*                panelBG;
    VectorObject*                panelGO;
    VectorObject*                panelGOutline;
    VectorObject*                restartOutline;
    VectorObject*                speedOutline;
    VectorObject*                focusOutline;
    VectorObject*                airportCircle;
    VectorObject*                outline;
    VectorObject*                inlay;
    Plane*                       mousePlane = nullptr;
    Color                        mc         = WHITE;
    Vector2                      restart2Loc {13.5f, 8.2f};
    Vector2                      speed1Loc {9.0f, 8.2f};
    Vector2                      speed2Loc {9.5f, 8.2f};
    Vector2                      speed3Loc {10.0f, 8.2f};
    Vector2                      speed4Loc {10.5f, 8.2f};
    Vector2                      focusOnLoc {9.5f, 7.2f};
    Vector2                      focusOffLoc {11.0f, 7.2f};
    Music                        alertMusic;
};
