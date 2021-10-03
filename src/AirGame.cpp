#include "AirGame.h"
#include "SelectScreen.h"
#include "GameScreen.h"

float NormalizeAngle(float a)
{
    while(a < 0)
    {
        a += twopi;
    }
    while(a >= twopi)
    {
        a -= twopi;
    }
    return a;
}

float AngleDist(float a1, float a2)
{
    return std::min(twopi - fabs(a1 - a2), fabs(a1 - a2));
}

AirGame::AirGame() : Game()
{
    timeScale = 1;
    m_noise = false;
}

void AirGame::CreateScreens()
{
    //int sn = 0;
    m_screens.push_back(new SelectScreen(0, this));
    m_screens.push_back(new GameScreen(1, this));
}

void AirGame::OnStart(double totalTime)
{
    noiseMusic = LoadMusicStream("resources/noise.ogg");
    //newSound = LoadSound("resources/new.wav");
}

void AirGame::OnTick(double deltaTime, double totalTime)
{
    UpdateMusicStream(noiseMusic);
    if(m_noise && !IsMusicStreamPlaying(noiseMusic))
    {
        PlayMusicStream(noiseMusic);
    } else if(!m_noise && IsMusicStreamPlaying(noiseMusic))
    {
        StopMusicStream(noiseMusic);
    }

    _gt = totalTime;

    m_flashOn = ((int)totalTime) % 2 == 0;

    if(m_activeScreen->m_no == 0)
        return;

    if(m_gameOver)
        return;

    //deltaTime *= TIME_SCALE;
    //totalTime *= TIME_SCALE;
    int prevGameTime = m_gameTime;
    m_gameTime       = static_cast<int>(totalTime - m_startTime);

    if(m_gameTime / SPAWN_PAUSE != prevGameTime / SPAWN_PAUSE)
    {
        board->SpawnPlane(totalTime, m_gameTime);
    }

    if(m_gameTime / AREA_PAUSE != prevGameTime / AREA_PAUSE)
    {
        board->SpawnArea(totalTime);
    }

    for(auto a : board->areas)
    {
        a->Tick(totalTime);
    }

    for(auto p1 : board->planes)
    {
        p1->Tick(deltaTime);
        if(p1->incoming && p1->mode != PlaneMode::Exited && totalTime - p1->started > SPAWN_TIME)
        {
            // only if we'd not be under alert
            bool alert = false;
            for(auto p2 : board->planes)
            {
                if(p1->id != p2->id && p2->mode != PlaneMode::Exited)
                {
                    if(p1->type != PlaneType::Military || p2->type != PlaneType::Military) // Military can't alert each other
                    {
                        float dx = p2->position.x - p1->position.x;
                        float dy = p2->position.y - p1->position.y;
                        if(fabsf(dx) < PLANE_ALERT_DIST * 1.5f && fabsf(dy) < PLANE_ALERT_DIST * 1.5f)
                        {
                            //float d = sqrt(dx * dx + dy * dy);
                            /*if(d < PLANE_CRASH_DIST)
                            {
                                // crash!
                            }
                            else*/
                            //if(d < PLANE_ALERT_DIST * 1.5f) // safe distance
                            {
                                alert = true;
                                break;
                            }
                        }
                    }
                }
            }
            if(!alert)
                p1->incoming = false; // launch up
        }
    }

    // cleanup exited planes
    board->planes.remove_if([](const Plane* p) { return p->mode == PlaneMode::Exited; });
    // TODO: delete

    // cleanup areas
    board->areas.remove_if([totalTime](const Area* a) { return a->endTime < totalTime; });
}

void AirGame::OnSwitchScreen(int prevScreen, int nextScreen) { }

Exit* Board::FindExit(const std::string& code)
{
    for(auto e : exits)
    {
        if(e->code == code)
            return e;
    }
    return nullptr;
}

int Area::_id = 0;

Area::Area(AreaType type, Vector2 start, float w, float h, float dx, float dy) :
    id(_id++), type(type), center(start), start(start), w(w), h(h), dx(dx), dy(dy)
{ }

void Area::Tick(double totalTime)
{
    // move
    center.x = start.x + (totalTime - startTime) / (endTime - startTime) * dx;
    center.y = start.y + (totalTime - startTime) / (endTime - startTime) * dy;
}

int Exit::_id = 0;

Exit::Exit(const std::string& code,
           const std::string& name,
           Vector2            location,
           ExitType           type,
           float              heading,
           float              width,
           float              length,
           float              tx,
           float              ty) :
    id(_id++),
    code {code}, name {name}, location {location}, type {type}, heading {heading}, width {width}, length {length}, tx {tx}, ty {ty}
{
    this->tx = -0.5f * AIRPORT_LENGTH * sin(heading);
    this->ty = -0.5f * AIRPORT_LENGTH * -cos(heading);
}

int Plane::_id = 0;

Plane::Plane(const std::string& callSign, Vector2 position, Exit* exit, PlaneMode mode, PlaneType type) :
    id(_id++), callSign {callSign}, origin {"UNK"}, position {position}, extraDelay {0}, speedScale {1.0f}, type {type}, exit {exit}, selected {false},
    entry_heading {NAN}, delay_done{false}
{
    this->heading       = 0;
    this->height        = 10;
    this->target_height = 10;
    this->speed         = DEFAULT_SPEED;
    this->target_speed  = DEFAULT_SPEED;
    this->incoming      = true;
    if(type == PlaneType::Cargo)
    {
        this->speedScale = 0.6f;
    }
    else if(type == PlaneType::Military)
    {
        this->speedScale = 1.5f;
    }
    if(exit != nullptr)
    {
        this->target          = exit->location;
        this->directionString = std::string("r") + exit->code;
    }
    else
    {
        this->directionString = "";
    }
    this->mode             = mode;
    this->entry            = (Vector2) {NAN, NAN};
    this->previousPosition = position;
    this->expTime          = NAN;
    this->areaSpeed        = 1.0f;
}

void Plane::TurnTowards(float target, float dt)
{
    // change heading towards a
    float rtd = target - heading;
    if(rtd < 0)
        rtd += twopi;
    if(rtd > twopi)
        rtd -= twopi;
    if(rtd < pi)
    {
        // turn right
        heading += ANGLE_SPEED * dt * speedScale;
    }
    else
    {
        // turn left
        heading -= ANGLE_SPEED * dt * speedScale;
    }
    heading = fmodf(heading, twopi);
}

void Plane::Tick(float dt)
{
    previousPosition = position;

    if(mode == PlaneMode::Static)
        return;

    if(mode == PlaneMode::Target && exit != nullptr)
    {
        // check if we're within target, if yes switch to destination
        float dx = target.x - position.x;
        float dy = target.y - position.y;
        if(fabsf(dx) < TARGET_THRESH && fabsf(dy) < TARGET_THRESH)
        {
            mode   = PlaneMode::Destination;
            target = exit->location;
        }
    }
    // update heading towards destination
    if(mode == PlaneMode::Target || mode == PlaneMode::Destination)
    {
        // do we need to adjust our heading?
        float dx = target.x - position.x;
        float dy = target.y - position.y;
        float a  = atan2f(-dy, -dx) - pi2;
        if(a < 0)
        {
            a += twopi;
        }
        if(target_heading != a)
        {
            target_heading = a;
        }
    }

    if(fabsf(AngleDist(NormalizeAngle(heading), NormalizeAngle(target_heading))) > twopi / 180.0f)
    {
        TurnTowards(target_heading, dt);
    }

    if(speed > target_speed)
    {
        speed -= ACCELERATION * dt;
        speed = std::max(speed, target_speed);
    }
    else if(speed < target_speed)
    {
        speed += ACCELERATION * dt;
        speed = std::min(speed, target_speed);
    }

    if(height > target_height)
    {
        height -= VERT_SPEED * dt;
        height = std::max(height, target_height);
    }
    else if(height < target_height)
    {
        height += VERT_SPEED * dt;
        height = std::min(height, target_height);
    }

    position.x += speed * sin(heading) * dt * speedScale * areaSpeed;
    position.y += speed * -cos(heading) * dt * speedScale * areaSpeed;
}

void Plane::Touch(double totalTime)
{
    if(!std::isnan(touchTime))
        return;

    // calculate expected time since we touch first time
    touchTime = totalTime;
    float dx  = (target.x - position.x);
    float dy  = (target.y - position.y);
    float d   = sqrt(dx * dx + dy * dy);
    if(target_speed != 0)
        expTime = d / (target_speed * speedScale);
    else
        expTime = NAN;
}

inline float Random(float min, float max)
{
    return min + (max - min) * (rand() % 100) / 100.0f;
}

void Board::SpawnArea(double startTime)
{
    auto type = (AreaType)(rand() % 2);

    float sx = type == AreaType::Weather ? Random(-10.0f, 10.0f) : Random(-8.0f, 8.0f);
    float sy = type == AreaType::Weather ? Random(-7.0f, 7.0f) : Random(-5.0f, 5.0f);

    float dx = type == AreaType::Weather ? Random(-4.0f, 4.0f) : 0;
    float dy = type == AreaType::Weather ? Random(-3.0f, 3.0f) : 0;

    float w = type == AreaType::Weather ? Random(1.0f, 3.0f) : Random(1.0f, 2.0f);
    float h = type == AreaType::Weather ? Random(1.0f, 3.0f) : Random(1.0f, 2.0f);

    auto a       = new Area(type, (Vector2) {sx, sy}, w, h, dx, dy);
    a->startTime = startTime;
    a->endTime   = startTime + AREA_MIN_TIME + (AREA_MAX_TIME - AREA_MIN_TIME) * (rand() % 100) / 100.0f;

    for(auto ea : areas)
    {
        // no on top of each other
        if(ea->IsAbove(a->center) || a->IsAbove(ea->center))
            return;
    }

    if(type == AreaType::NoFly)
    {
        for(auto e : exits)
        {
            if(a->IsAbove(e->location)) // no exits  within nofly
            {
                return;
            }
        }
    }

    areas.push_back(a);
}

bool Area::IsAbove(Vector2 p)
{
    return (p.x <= center.x + (w / 2)) && (p.x >= center.x - (w / 2)) && (p.y <= center.y + (h / 2)) && (p.y >= center.y - (h / 2));
}

void Board::SpawnPlane(double startTime, double gameTime)
{
    //return;

    //PlaySoundMulti(newSound);

    int  rtype = rand() % 12;
    auto type  = rtype == 0 ? PlaneType::Military : (rtype == 1 ? PlaneType::VIP : (rtype < 5 ? PlaneType::Cargo : PlaneType::Passenger));
    //auto type  = (PlaneType)(rand() % 4);

    if(gameTime < 60 && type == PlaneType::VIP)
        type = PlaneType::Passenger;

    if(gameTime < 120 && type == PlaneType::Military)
        type = PlaneType::Passenger;

    int entry = rand() % exits.size();
    int exit  = rand() % exits.size();
    while(exit == entry || (type == PlaneType::Military && exits[exit]->type == ExitType::Airport))
    {
        exit = rand() % exits.size();
    }
    const std::string& prefix   = type == PlaneType::Military ? "MX" : airlines[rand() % airlines.size()];
    const std::string  callSign = prefix + std::to_string(rand() % 100);

    // move forward a bit so we don't spawn so close
    Vector2 location = exits[entry]->location;
    location.x += 0.5f * sin(exits[entry]->heading);
    location.y += 0.5f * -cos(exits[entry]->heading);

    auto p     = new Plane(callSign, location, exits[exit], PlaneMode::Destination, type);
    p->heading = exits[entry]->heading;
    if(p->heading < 0)
        p->heading += twopi;

    p->started = startTime;
    p->origin  = exits[entry]->code;
    if(exits[entry]->type == ExitType::Exit)
    {
        p->heading += pi; // came out of a corridor
        if(p->heading >= twopi)
            p->heading -= twopi;

        // also shake position a bit?
        float shakeHeading = p->heading + pi2; // 90 deg angle
        p->position.x += 2.0f * sin(shakeHeading);
        p->position.y += 2.0f * -cos(shakeHeading);
    }

    p->touchTime = NAN;

    planes.push_back(p);
}

float Plane::Delay(double totalTime)
{
    if(delay_done)
        return 0;

    if(type == PlaneType::Military)
        return 0;

    if(std::isnan(touchTime) || std::isnan(expTime))
        return extraDelay;

    auto d = (float)((totalTime - touchTime) - expTime) * (type == PlaneType::VIP ? 3 : 1);
    return d + extraDelay;
}

float Plane::ExpDelay(double totalTime)
{
    if(delay_done)
        return 0;

    if(type == PlaneType::Military)
        return 0;

    if(mode == PlaneMode::Exited || mode == PlaneMode::Static)
        return 0;

    if(std::isnan(touchTime) || exit == nullptr || std::isnan(expTime))
        return extraDelay;

    // calculate expected time
    float remTime = 0;
    if(target_speed != 0 && speedScale != 0)
    {
        float dx      = exit->location.x - position.x;
        float dy      = exit->location.y - position.y;
        float d       = sqrt(dx * dx + dy * dy);
        remTime = d / (target_speed * speedScale);
    }

    return extraDelay + ((totalTime + remTime) - (touchTime + expTime)) * (type == PlaneType::VIP ? 3 : 1);
}

Board::Board(Map m)
{
    if(m != Map::None)
    {
        airlines.push_back("AB");
        airlines.push_back("CD");
        airlines.push_back("EF");
        airlines.push_back("GH");
        airlines.push_back("IJ");
        airlines.push_back("KL");
        airlines.push_back("MN");
        airlines.push_back("OP");
        airlines.push_back("QR");
        airlines.push_back("ST");

        exits.push_back(new Exit {"NORTH", "NORTH EXIT", (Vector2) {0, -8.5f}, ExitType::Exit, 0, CORRIDOR_WIDTH, CORRIDOR_LENGTH});
        exits.push_back(new Exit {"SOUTH", "SOUTH EXIT", (Vector2) {0, 8.5}, ExitType::Exit, -pi, CORRIDOR_WIDTH, CORRIDOR_LENGTH});
        exits.push_back(new Exit {"EAST", "EAST EXIT", (Vector2) {10.5f, 0}, ExitType::Exit, pi2, CORRIDOR_WIDTH, CORRIDOR_LENGTH});
        exits.push_back(new Exit {"WEST", "WEST EXIT", (Vector2) {-10.5f, 0}, ExitType::Exit, 3 * pi2, CORRIDOR_WIDTH, CORRIDOR_LENGTH});
    }
    if(m == Map::Tokyo || m == Map::None)
    {
        exits.push_back(new Exit {
            "NRT", "Narita", (Vector2) {9.0f - 4.0f, -2.0f + 2.0f}, ExitType::Airport, -0.4f + twopi, AIRPORT_WIDTH, AIRPORT_LENGTH});
        exits.push_back(new Exit {
            "HND", "Haneda", (Vector2) {-1.0f - 2.0f, 2.0f + 2.0f}, ExitType::Airport, 0.4f + twopi, AIRPORT_WIDTH, AIRPORT_LENGTH});
        exits.push_back(new Exit {"IBR",
                                  "Ibaraki",
                                  (Vector2) {9.5f - 4.0f, -7.0f + 1.0f},
                                  ExitType::Airport,
                                  0.2f,
                                  AIRPORT_WIDTH / 2.0f,
                                  AIRPORT_LENGTH}); // Ibaraki
        exits.push_back(new Exit {"OKO",
                                  "Yokota",
                                  (Vector2) {-4.0f - 2.0f, -2.0f},
                                  ExitType::Airport,
                                  -0.2f + twopi,
                                  AIRPORT_WIDTH / 2.0f,
                                  AIRPORT_LENGTH}); // Yokota
    }
    if(m == Map::SF || m == Map::None)
    {
        exits.push_back(
            new Exit {"SFO", "San Francisco", (Vector2) {-2.5f - 2.0f, -1.25f}, ExitType::Airport, 1.75f, AIRPORT_WIDTH, AIRPORT_LENGTH});
        exits.push_back(new Exit {"OAK", "Oakland", (Vector2) {1.5f - 2.4f, -4.75f}, ExitType::Airport, 2.9f, AIRPORT_WIDTH, AIRPORT_LENGTH});
        exits.push_back(
            new Exit {"SJC", "San Jose", (Vector2) {7.5f - 2.0f, 5.7f}, ExitType::Airport, -0.2f + twopi, AIRPORT_WIDTH / 2.0f, AIRPORT_LENGTH});
    }
    if(m == Map::London || m == Map::None)
    {
        exits.push_back(new Exit {"LHR", "Heathrow", (Vector2) {-5.2f, 1.6f}, ExitType::Airport, 1.75f, AIRPORT_WIDTH, AIRPORT_LENGTH});

        exits.push_back(new Exit {"LCY", "City", (Vector2) {0.75f, 0.5f}, ExitType::Airport, 2.9f, AIRPORT_WIDTH, AIRPORT_LENGTH});

        exits.push_back(
            new Exit {"LGW", "Gatwick", (Vector2) {-1.5f, 6.7f}, ExitType::Airport, -0.2f + twopi, AIRPORT_WIDTH / 2.0f, AIRPORT_LENGTH});

        exits.push_back(new Exit {"LTN", "Luton", (Vector2) {-4.0f, -6.7f}, ExitType::Airport, 2.7f, AIRPORT_WIDTH / 2.0f, AIRPORT_LENGTH});

        exits.push_back(
            new Exit {"STN", "Stansted", (Vector2) {3.75f, -6.7f}, ExitType::Airport, 4.0f, AIRPORT_WIDTH / 2.0f, AIRPORT_LENGTH});
    }
}

Vector2 Exit::LocalPos(Vector2 modelPos)
{
    float s  = sin(-heading);
    float c  = cos(-heading);
    float dx = modelPos.x - location.x;
    float dy = modelPos.y - location.y;
    float rx = dx * c - dy * s;
    float ry = dx * s + dy * c;
    return (Vector2) {rx, ry};
}

bool Exit::IsAbove(Vector2 p)
{
    auto localPos = LocalPos(p);
    return fabsf(localPos.x) <= width / 2 && localPos.y <= 0 && localPos.y >= -length / 2;
}

bool Exit::GoodPass(Vector2 entry, Vector2 exit, float entry_heading)
{
    float a1 = NormalizeAngle(heading);
    float a2 = NormalizeAngle(entry_heading);

    //  OK if we entered within 90 degs
    auto adiff = AngleDist(a1, a2);
    return adiff < pi2;
}

void AirGame::StartMap(Map m)
{
    m_map     = m;
    board     = new Board(m);
    timeScale = 1;

    switch (m)
    {
    case Map::SF:
        SPAWN_PAUSE = 11;
        break;
    
    case Map::Tokyo:
        SPAWN_PAUSE = 9;
        break;

    case Map::London:
        SPAWN_PAUSE = 6;
        break;

    default:
        break;
    }

    SwitchScreen(1);

    m_startTime = _gt;
    m_gameOver  = false;
    m_winner    = false;
    m_selection = true;
    board->SpawnPlane(_gt, 0);
    //board->SpawnArea(_gt);
}

void AirGame::SelectMap()
{
    timeScale = 1;
    SwitchScreen(0);
}