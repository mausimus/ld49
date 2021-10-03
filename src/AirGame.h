#pragma once

#include "Game.h"

#define C_AMBER1                                                                                                                           \
    CLITERAL(Color)                                                                                                                        \
    {                                                                                                                                      \
        255, 204, 0, 255                                                                                                                   \
    }
#define C_AMBER2                                                                                                                           \
    CLITERAL(Color)                                                                                                                        \
    {                                                                                                                                      \
        255, 176, 0, 255                                                                                                                   \
    }
#define C_AMBER3                                                                                                                           \
    CLITERAL(Color)                                                                                                                        \
    {                                                                                                                                      \
        127, 78, 0, 255                                                                                                                    \
    }

#define C_GREEN1                                                                                                                           \
    CLITERAL(Color)                                                                                                                        \
    {                                                                                                                                      \
        0, 255, 51, 255                                                                                                                    \
    }
#define C_GREEN3                                                                                                                           \
    CLITERAL(Color)                                                                                                                        \
    {                                                                                                                                      \
        0, 127, 25, 255                                                                                                                    \
    }

#define C_BLUE0                                                                                                                            \
    CLITERAL(Color)                                                                                                                        \
    {                                                                                                                                      \
        200, 240, 255, 255                                                                                                                 \
    }
#define C_BLUE1                                                                                                                            \
    CLITERAL(Color)                                                                                                                        \
    {                                                                                                                                      \
        82, 198, 255, 255                                                                                                                  \
    }
#define C_BLUE2                                                                                                                            \
    CLITERAL(Color)                                                                                                                        \
    {                                                                                                                                      \
        63, 150, 191, 255                                                                                                                  \
    }
#define C_BLUE3                                                                                                                            \
    CLITERAL(Color)                                                                                                                        \
    {                                                                                                                                      \
        42, 100, 127, 255                                                                                                                  \
    }
#define C_BLUE4                                                                                                                            \
    CLITERAL(Color)                                                                                                                        \
    {                                                                                                                                      \
        21, 50, 63, 255                                                                                                                    \
    }
#define C_BLUE45                                                                                                                           \
    CLITERAL(Color)                                                                                                                        \
    {                                                                                                                                      \
        14, 35, 40, 255                                                                                                                    \
    }
#define C_BLUE5                                                                                                                            \
    CLITERAL(Color)                                                                                                                        \
    {                                                                                                                                      \
        8, 20, 25, 255                                                                                                                     \
    }

#define C_WHITE1                                                                                                                           \
    CLITERAL(Color)                                                                                                                        \
    {                                                                                                                                      \
        0xff, 0xff, 0xff, 255                                                                                                              \
    }
#define C_WHITE2                                                                                                                           \
    CLITERAL(Color)                                                                                                                        \
    {                                                                                                                                      \
        0xaa, 0xaa, 0xaa, 255                                                                                                              \
    }
#define C_WHITE3                                                                                                                           \
    CLITERAL(Color)                                                                                                                        \
    {                                                                                                                                      \
        0x55, 0x55, 0x55, 255                                                                                                              \
    }

#define C_RED                                                                                                                              \
    CLITERAL(Color)                                                                                                                        \
    {                                                                                                                                      \
        252, 84, 116, 255                                                                                                                  \
    }
#define C_VIP                                                                                                                              \
    CLITERAL(Color)                                                                                                                        \
    {                                                                                                                                      \
        245, 166, 0, 255                                                                                                                   \
    }
#define C_MIL                                                                                                                              \
    CLITERAL(Color)                                                                                                                        \
    {                                                                                                                                      \
        84, 190, 98, 255                                                                                                                   \
    }

constexpr float  AIRPORT_WIDTH    = 0.3f;
constexpr float  AIRPORT_LENGTH   = 0.75f;
constexpr float  CORRIDOR_WIDTH   = 2.5f;
constexpr float  CORRIDOR_LENGTH  = 0.2f;
constexpr float  EXIT_RADIUS      = 0.2f;
constexpr float  ANGLE_SPEED      = 0.2f;
constexpr float  VERT_SPEED       = 0.1f;
constexpr float  ACCELERATION     = 0.1f;
constexpr float  DEFAULT_SPEED    = 0.1f;
constexpr double SPAWN_TIME       = 20.0; // time a new plane is immune
constexpr int    AREA_PAUSE       = 50;
constexpr float  AREA_MIN_TIME    = 100;
constexpr float  AREA_MAX_TIME    = 300;
constexpr float  TARGET_THRESH    = 0.2f;
constexpr float  PLANE_ALERT_DIST = 0.5f;
constexpr float  PLANE_CRASH_DIST = 0.1f;

enum class Map
{
    None   = 0,
    SF     = 1,
    Tokyo  = 2,
    London = 3
};

enum class PlaneMode
{
    Heading     = 0,
    Destination = 1,
    Target      = 2,
    Airway      = 3,
    Exited      = 4,
    Static      = 5
};

enum class PlaneType
{
    Passenger = 0,
    Cargo     = 1,
    VIP       = 2,
    Military  = 3
};

enum class ExitType
{
    Exit    = 0,
    Airport = 1
};

class Exit
{
private:
    static int _id;

    Vector4 entryLine;
    Vector4 exitLine;

    Vector2 LocalPos(Vector2 modelPos);

public:
    Exit(const std::string& code,
         const std::string& name,
         Vector2            location,
         ExitType           type,
         float              heading,
         float              width,
         float              length,
         float              tx = 0,
         float              ty = 0);
    bool IsAbove(Vector2 p);
    bool GoodPass(Vector2 entry, Vector2 exit, float entry_heading);

    int         id;
    std::string code;
    std::string name;
    Vector2     location;
    ExitType    type;
    float       heading;
    float       width;
    float       length;
    float       tx; // text offset
    float       ty;
};

class Plane
{
private:
    static int _id;

    void TurnTowards(float target, float dt);

public:
    Plane(const std::string& callSign, Vector2 position, Exit* exit, PlaneMode mode, PlaneType type);

    void  Tick(float dt);
    float Delay(double totalTime);
    float ExpDelay(double totalTime);

    void Touch(double totalTime);

    int                     id;
    std::string             callSign;
    std::string             origin;
    Vector2                 position;
    Vector2                 previousPosition;
    float                   speed;
    float                   target_speed;
    float                   height;
    float                   target_height;
    float                   heading;
    float                   target_heading;
    float                   extraDelay;
    float                   speedScale;
    float                   areaSpeed;
    Vector2                 target;
    PlaneMode               mode;
    PlaneType               type;
    Exit*                   exit;
    bool                    selected;
    bool                    incoming;
    double                  started;
    double                  touchTime;
    float                   expTime;
    std::unordered_set<int> alerts;
    std::string             directionString;
    Vector2                 entry;
    float                   entry_heading;
    bool                    delay_done;
};

enum class AreaType
{
    NoFly   = 0,
    Weather = 1
};

class Area
{
private:
    static int _id;

public:
    Area(AreaType type, Vector2 start, float w, float h, float dx, float dy);

    bool IsAbove(Vector2 p);
    void Tick(double totalTime);

    int         id;
    std::string name;
    AreaType    type;
    double      startTime;
    double      endTime;
    Vector2     center;
    Vector2     start;
    float       w;
    float       h;
    float       dx;
    float       dy;
};

class Board
{
public:
    Board(Map m);

    Exit* FindExit(const std::string& code);
    void  SpawnPlane(double totalTime, double gameTime);
    void  SpawnArea(double totalTime);

    std::vector<std::string> airlines;
    std::list<Plane*>        planes;
    std::vector<Exit*>       exits;
    std::list<Area*>         areas;
};

class AirGame : public Game
{
protected:
    virtual void CreateScreens();
    virtual void OnTick(double deltaTime, double totalTime);
    virtual void OnStart(double totalTime);
    virtual void OnSwitchScreen(int prevScreen, int nextScreen);

private:
    double _gt;
    Sound  newSound;
    Music  noiseMusic;
    int    SPAWN_PAUSE = 10;

public:
    AirGame();
    void StartMap(Map m);
    void SelectMap();

    double m_startTime;
    int    m_gameTime;
    bool   m_gameOver;
    bool   m_winner;
    bool   m_flashOn;
    bool   m_selection;
    int    timeScale;
    Map    m_map;
    bool   m_noise;
    Board* board;
};