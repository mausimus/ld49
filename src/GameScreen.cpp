#include "GameScreen.h"
#include "VectorBatch.h"
#include "rlgl.h"

#define PANEL_COLOR C_BLUE1

constexpr float TIME_PANEL_X   = 6.5f;
constexpr float TIME_PANEL_X2  = 11.5f;
constexpr float TIME_PANEL_Y   = -8.5f;
constexpr float MESSAGES_X     = TIME_PANEL_X;
constexpr float MESSAGES_Y     = 0.0f;
constexpr float HINT_PANEL_X   = TIME_PANEL_X;
constexpr float HINT_PANEL_Y   = -1.5f;
constexpr float PLANE_PANEL_X  = TIME_PANEL_X;
constexpr float PLANE_PANEL_X2 = 11.5f;
constexpr float PLANE_PANEL_Y  = -6.0f;
constexpr float TEXT_HEIGHT    = CHAR_HEIGHT * 3.0f;
constexpr float BOUND_X        = 12.0f + 2.0f;
constexpr float BOUND_Y        = 9.0f + 2.0f;
const int       GAME_TIME      = 720;

GameScreen::GameScreen(int no, AirGame* game) : Screen(no, game) { }

void GameScreen::Load()
{
    alertMusic      = LoadMusicStream("resources/alert.ogg");
    landedSound     = LoadSound("resources/landed.wav");
    ackSound        = LoadSound("resources/ack.ogg");
    alertSound      = LoadSound("resources/alert.ogg");
    tickSound       = LoadSound("resources/tick.ogg");
    startSound      = LoadSound("resources/start.ogg");
    endSound        = LoadSound("resources/end.ogg");
    winSound        = LoadSound("resources/win.ogg");
    rect            = VectorObject::Rectangle(0.5f, 0.6f);
    tri             = VectorObject::Triangle(0.4f, 0.3f);
    circ            = VectorObject::Circle(0.1f);
    tri->_solid     = true;
    rect->_solid    = false;
    panelBG         = VectorObject::Rectangle(10.0f, 18.0f);
    panelBG->_solid = true;
    panelGO         = VectorObject::Rectangle(10.0f, 5.0f);
    panelGO->_solid = true;
    panelGOutline   = VectorObject::Rectangle(10.0f, 5.0f);
    restartOutline  = VectorObject::Rectangle(4.0f, 0.7f);
    speedOutline    = VectorObject::Rectangle(0.5f, 0.7f);
    focusOutline    = VectorObject::Rectangle(1.2f, 0.7f);
    exitObj         = VectorObject::Circle(0.2f);
    airportObj      = VectorObject::Circle(0.1f);
    planeObj        = m_game->m_assets->Plane();
    planeSelected   = VectorObject::Rectangle(0.5f, 0.5f);
    planeTarget     = VectorObject::Rectangle(0.25f, 0.25f);
    airportCircle   = VectorObject::DottedCircle(2.0f);
    outline         = m_game->m_assets->Outline();
    inlay           = m_game->m_assets->Inlay();
}

void GameScreen::Enter()
{
    prevGameTime = 0;
    switch(m_game->m_map)
    {
    default:
    case Map::Tokyo:
        map = m_game->m_assets->Tokyo();
        break;

    case Map::London:
        map = m_game->m_assets->London();
        break;

    case Map::SF:
        map = m_game->m_assets->SF();
        break;
    }

    messages.clear();
    exitObjs.clear();
    areaObjs.clear();

    // init
    x_shift            = -5.0f;
    y_shift            = 0.0f;
    zoom               = 1.0f;
    mouse_release_time = 0;
    selectedPlane      = nullptr;

    // prerender exits
    for(auto e : m_game->board->exits)
    {
        auto vo = m_game->m_assets->Airport(e->width, e->length);
        //vo->_solid = true;
        exitObjs.insert(std::make_pair(e->id, vo));
    }

    for(auto p : m_game->board->planes)
    {
        if(p->callSign == "M")
        {
            mousePlane = p;
        }
    }

    finishTime = NAN;

    Message("THEN CLICK ON MAP TO REROUTE");
    Message("CLICK ON A PLANE TO SELECT");
    Message("  ");
    Message("AVOID PLANE COLLISIONS!");
    Message("  ");
    Message("GOOD MORNING & GOOD LUCK");
    Message("  ");
    m_delay    = 0;
    totalDelay = 0;
    m_alerts   = 0;

    PlaySoundMulti(startSound);
}

void GameScreen::AddDelay(Plane* p)
{
    if(!p->delay_done)
    {
        auto nd = p->ExpDelay(totalTime);
        auto ed = p->ExpDelay(totalTime);
        if(!std::isnan(nd) && p->type != PlaneType::Military)
        {
            m_delay += nd;
        }
        p->delay_done = true;
    }
}

void GameScreen::Leave() { }

void GameScreen::Unload() { }

void GameScreen::Tick(double deltaTime, double totalTime)
{
    UpdateMusicStream(alertMusic);
    if(!m_game->m_gameOver)
    {
        this->totalTime = totalTime;
        this->deltaTime = deltaTime;
    }
    else
    {
        if(IsMusicStreamPlaying(alertMusic))
        {
            StopMusicStream(alertMusic);
        }
        if(m_game->timeScale != 1)
            m_game->timeScale = 1;
        if(std::isnan(finishTime))
            finishTime = totalTime;
    }

    if(m_game->m_gameTime != prevGameTime && m_game->m_gameTime % 7 == 0)
    {
        PlaySoundMulti(tickSound);
        prevGameTime = m_game->m_gameTime;
    }

    if(m_game->m_gameTime >= GAME_TIME) // 12hr shift 06:00 - 18:00
    {
        if(!m_game->m_gameOver)
        {
            PlaySoundMulti(winSound);
        }
        m_game->m_gameOver = true;
        m_game->m_winner   = true;
        if(std::isnan(finishTime))
            finishTime = totalTime;

        //return;
    }

    mx = m_game->m_mouseX / zoom - x_shift;
    my = m_game->m_mouseY / zoom - y_shift;

    // DEBUG!
    /*
    if(mousePlane != nullptr)
    {
        mousePlane->position = (Vector2) {mx, my};

        if(IsKeyPressed(KEY_M))
        {
            mousePlane->mode = PlaneMode::Static;
        }
    }

    if(IsKeyPressed(KEY_W))
    {
        m_game->m_gameOver = true;
        m_game->m_winner   = true;
        PlaySoundMulti(winSound);
    }
    if(IsKeyPressed(KEY_L))
    {
        m_game->m_gameOver = true;
        m_game->m_winner   = false;
        PlaySoundMulti(endSound);
    }
*/

    if(!m_game->m_gameOver && (IsKeyPressed(KEY_P) || IsKeyPressed(KEY_SPACE)))
    {
        messages.push_back("NO PAUSING!!!");
    }

    // iterate selectable objects
    if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        if(IsHoveringRestartButton() || IsHoveringRestartButton2())
        {
            Restart();
            return;
        }
        int h = IsHoveringSpeedButton();
        if(h)
            m_game->timeScale = h;

        int f = IsHoveringFocusButton();
        if(f == 1)
            m_game->m_noise = true;
        else if(f == -1)
            m_game->m_noise = false;

        if(m_game->m_gameOver)
            return;

        if(mx < 16 - 5) // only on the plane view
        {
            // find selected object
            bool foundSelection = false;
            for(auto p : m_game->board->planes)
            {
                if(p->mode == PlaneMode::Exited)
                    continue;

                if(foundSelection)
                {
                    p->selected = false;
                }
                else if(fabsf(mx - p->position.x) < planeObj->_width && fabsf(my - p->position.y) < planeObj->_height)
                {
                    if(p->selected)
                    {
                        p->selected = false;
                        continue; // deselect and look for next selection
                    }
                    else
                    {
                        p->selected    = true;
                        foundSelection = true;
                        selectedPlane  = p;
                    }
                }
                else
                {
                    p->selected = false;
                }
            }

            if(foundSelection)
            {
                mouse_state = MouseState::SetTarget;
            }
            else if(selectedPlane != nullptr && selectedPlane->mode != PlaneMode::Exited && selectedPlane->type != PlaneType::Military)
            {
                selectedPlane->Touch(totalTime); // mark start of delay

                selectedPlane->target   = (Vector2) {mx, my};
                selectedPlane->mode     = PlaneMode::Target;
                selectedPlane->selected = true; // keep selection
                PlaySoundMulti(ackSound);
            }
        }
    }

    mouse_click = (Vector2) {mx, my};

    if(IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
    {
        mouse_release_time = totalTime;
    }

    // DEBUG only? hover exits
    /*
    mc = WHITE;
    for(auto e : m_game->board->exits)
    {
        if(e->IsAbove((Vector2) {mx, my}))
        {
            mc = YELLOW;
        }
    }*/

    CheckPlanes();
    CheckCollisions();
}

bool GoodPass(Vector2 entry, Vector2 exit, Exit* area)
{
    return false;
}

void GameScreen::DrawBackground() { }

void GameScreen::DrawPixels(Color* framebuffer, Rectangle* rect, bool* fullFrame) { }

void GameScreen::DrawShapes() { }

bool GameScreen::IsHoveringRestartButton()
{
    if(m_game->m_gameOver)
    {
        return (mx >= -restartOutline->_width / 2 - x_shift) && (mx <= restartOutline->_width / 2 - x_shift) &&
               (my >= 1.8f - restartOutline->_height / 2) && (my <= 1.8f + restartOutline->_height / 2);
    }
    return false;
}

bool GameScreen::IsHoveringRestartButton2()
{
    if(!m_game->m_gameOver)
    {
        return (mx >= restart2Loc.x + -restartOutline->_width / 2 - x_shift) &&
               (mx <= restart2Loc.x + restartOutline->_width / 2 - x_shift) && (my >= restart2Loc.y - restartOutline->_height / 2) &&
               (my <= restart2Loc.y + restartOutline->_height / 2);
    }
    return false;
}

int GameScreen::IsHoveringSpeedButton()
{
    if(!m_game->m_gameOver)
    {
        if((mx >= speed1Loc.x + -speedOutline->_width / 2 - x_shift) && (mx <= speed1Loc.x + speedOutline->_width / 2 - x_shift) &&
           (my >= speed1Loc.y - speedOutline->_height / 2) && (my <= speed1Loc.y + speedOutline->_height / 2))
            return 1;
        if((mx >= speed2Loc.x + -speedOutline->_width / 2 - x_shift) && (mx <= speed2Loc.x + speedOutline->_width / 2 - x_shift) &&
           (my >= speed2Loc.y - speedOutline->_height / 2) && (my <= speed2Loc.y + speedOutline->_height / 2))
            return 2;
        if((mx >= speed3Loc.x + -speedOutline->_width / 2 - x_shift) && (mx <= speed3Loc.x + speedOutline->_width / 2 - x_shift) &&
           (my >= speed3Loc.y - speedOutline->_height / 2) && (my <= speed3Loc.y + speedOutline->_height / 2))
            return 3;
        if((mx >= speed4Loc.x + -speedOutline->_width / 2 - x_shift) && (mx <= speed4Loc.x + speedOutline->_width / 2 - x_shift) &&
           (my >= speed4Loc.y - speedOutline->_height / 2) && (my <= speed4Loc.y + speedOutline->_height / 2))
            return 4;
    }
    return 0;
}

int GameScreen::IsHoveringFocusButton()
{
    if(!m_game->m_gameOver)
    {
        if((mx >= focusOnLoc.x + -focusOutline->_width / 2 - x_shift) && (mx <= focusOnLoc.x + focusOutline->_width / 2 - x_shift) &&
           (my >= focusOnLoc.y - focusOutline->_height / 2) && (my <= focusOnLoc.y + focusOutline->_height / 2))
            return 1;
        if((mx >= focusOffLoc.x + -focusOutline->_width / 2 - x_shift) && (mx <= focusOffLoc.x + focusOutline->_width / 2 - x_shift) &&
           (my >= focusOffLoc.y - focusOutline->_height / 2) && (my <= focusOffLoc.y + focusOutline->_height / 2))
            return -1;
    }
    return 0;
}

void GameScreen::DrawGameOver()
{
    // BG
    rlEnd();
    rlSetBlendMode(BLEND_ALPHA);
    rlBegin(RL_TRIANGLES);
    m_vectorBatch.DrawObject(panelGO, (Vector2) {0, 0}, 0, C_BLUE5);
    rlEnd();
    rlSetBlendMode(BLEND_CUSTOM);
    rlBegin(RL_TRIANGLES);
    m_vectorBatch.DrawObject(panelGOutline, (Vector2) {0, 0}, 0, C_BLUE1);

    if(!m_game->m_winner)
    {
        DrawText("COULDN'T EVEN PASS", 0, -2.0f, C_BLUE0, 0, 0, 2);
        DrawText("A TRAINING SIMULATION...", 0, -1.2f, C_BLUE0, 0, 0, 2);

        if(m_game->m_flashOn)
        {
            DrawText("YOU'RE FIRED!!!", 0, 0.3f, C_BLUE0, 0, 0, 2);
        }
    }
    else
    {
        DrawText("EXCELLENT JOB!", 0, -1.6f, C_BLUE0, 0, 0, 2);

        if(m_alerts == 0)
        {
            if(m_game->m_flashOn)
            {
                DrawText("ASK FOR A RAISE!!!", 0, 0.1f, C_BLUE0, 0, 0, 2);
            }
        }
        else
        {
            DrawText("BACK TO WORK TOMORROW.", 0, 0.1f, C_BLUE0, 0, 0, 2);
        }
    }

    Color c = C_BLUE2;
    if(IsHoveringRestartButton())
    {
        c = C_BLUE0;
    }

    DrawText(m_game->m_winner ? "AGAIN!" : "REAPPLY", 0, 1.8f, c, 0, 0, 2);
    m_vectorBatch.DrawObject(restartOutline, (Vector2) {0, 1.8f}, 0, c);
}

void GameScreen::DrawVectors()
{
    m_vectorBatch.DrawObject(map, (Vector2) {x_shift, 0}, 0, C_BLUE45);

    DrawBoard();
    DrawPanels();

    if(m_game->m_gameOver)
        DrawGameOver();

    m_vectorBatch.DrawObject(&m_mouseCursor, (Vector2) {m_game->m_mouseX, m_game->m_mouseY}, 0, mc);
}

Vector2 GameScreen::ScreenPos(Vector2 modelPos)
{
    return (Vector2) {(modelPos.x + x_shift) * zoom, (modelPos.y + y_shift) * zoom};
}

void GameScreen::DrawPlane(Plane* plane)
{
    Vector2 screenPos = ScreenPos(plane->position);

    Color c = C_BLUE0;
    if(plane->type == PlaneType::VIP)
        c = C_VIP;

    c = (plane->incoming || plane->mode == PlaneMode::Heading) ? C_BLUE3 : (plane->alerts.size() ? C_RED : c);

    if(plane->type == PlaneType::Military)
        c = C_MIL; // always visible

    m_vectorBatch.DrawObject(planeObj, screenPos, plane->heading * RAD2DEG, c);
    DrawText(plane->callSign, screenPos.x, screenPos.y + 0.35f, c, 0, -1);
    if(plane->exit != nullptr)
        DrawText(plane->exit->code, screenPos.x, screenPos.y + 0.35f + CHAR_HEIGHT + 0.1f, c, 0, -1);

    if(plane->selected)
    {
        m_vectorBatch.DrawObject(planeSelected, screenPos, 0, c);

        // show its target
        Vector2 targetPos = ScreenPos(plane->target);
        m_vectorBatch.DrawObject(planeTarget, targetPos, 0, c);

        // line to target
        m_vectorBatch.DrawLine(screenPos.x, screenPos.y, targetPos.x, targetPos.y, c);

        // if target not destination, also draw destination
        if(plane->mode == PlaneMode::Target && plane->exit != nullptr)
        {
            Vector2 exitPos = ScreenPos(plane->exit->location);
            // line to exit
            m_vectorBatch.DrawLine(targetPos.x, targetPos.y, exitPos.x, exitPos.y, c);
            m_vectorBatch.DrawObject(planeTarget, exitPos, 0, c);
        }
    }
}

void GameScreen::DrawExit(Exit* exit)
{
    Vector2 screenPos = ScreenPos(exit->location);

    auto vectorObj = exitObjs.at(exit->id);

    switch(exit->type)
    {
    case ExitType::Exit:
        m_vectorBatch.DrawObject(vectorObj, screenPos, exit->heading * RAD2DEG, C_BLUE3);
        if(exit->code == "EAST" || exit->code == "WEST")
            DrawText(exit->name, (screenPos.x - x_shift) * 0.97f + x_shift, screenPos.y, C_BLUE3, 0, 0, 1, 1);
        else
            DrawText(exit->name, screenPos.x, screenPos.y * 0.97f, C_BLUE3, 0, 0, 1, 0);
        //DrawText(exit->code, screenPos.x, screenPos.y + 0.5f, C_BLUE3, 0, -1);
        break;

    case ExitType::Airport:
        m_vectorBatch.DrawObject(vectorObj, screenPos, exit->heading * RAD2DEG, C_BLUE1);
        m_vectorBatch.DrawObject(airportCircle, screenPos, 0, C_BLUE45);
        DrawText(exit->code, screenPos.x + exit->tx, screenPos.y + exit->ty, C_BLUE2, 0, 0);
        break;

    default:
        break;
    }
}

void GameScreen::DrawArea(Area* area)
{
    Vector2 screenPos = ScreenPos(area->center);

    if(areaObjs.find(area->id) == areaObjs.end())
    {
        // precreate object
        auto ao = VectorObject::Rectangle(area->w, area->h);
        areaObjs.insert(std::make_pair(area->id, ao));
    }

    auto vectorObj = areaObjs.at(area->id);

    switch(area->type)
    {
    case AreaType::NoFly:
        m_vectorBatch.DrawObject(vectorObj, screenPos, 0, C_BLUE3);
        DrawText("NO FLY", screenPos.x, screenPos.y, C_BLUE3, 0, 0);
        break;

    case AreaType::Weather:
        m_vectorBatch.DrawObject(vectorObj, screenPos, 0, C_BLUE3);
        DrawText("STORM", screenPos.x, screenPos.y, C_BLUE3, 0, 0);
        break;

    default:
        break;
    }
}

void GameScreen::DrawBoard()
{
    // outline
    m_vectorBatch.DrawObject(outline, (Vector2) {x_shift, 0}, 0, haveAlerts && m_game->m_flashOn ? C_RED : C_BLUE1);
    m_vectorBatch.DrawObject(inlay, (Vector2) {x_shift, 0}, 0, C_BLUE3);

    // draw exits
    for(auto exit : m_game->board->exits)
    {
        DrawExit(exit);
    }

    // draw areas
    for(auto area : m_game->board->areas)
    {
        DrawArea(area);
    }

    // draw planes
    for(auto plane : m_game->board->planes)
    {
        if(plane->mode != PlaneMode::Exited)
        {
            DrawPlane(plane);
        }
    }
}

void GameScreen::DrawPanels()
{
    // BG
    rlEnd();
    rlSetBlendMode(BLEND_ALPHA);
    rlBegin(RL_TRIANGLES);
    m_vectorBatch.DrawObject(panelBG, (Vector2) {16 - 5, 0}, 0, C_BLUE5);
    rlEnd();
    rlSetBlendMode(BLEND_CUSTOM);
    rlBegin(RL_TRIANGLES);

    // draw time panel
    {
        float yPos = TIME_PANEL_Y;
        char  timeString[20];
        int   timeLeft = GAME_TIME - m_game->m_gameTime;
        std::snprintf(timeString, 20, "%02d:%02d", /*6 +*/ timeLeft /*m_game->m_gameTime*/ / 60, timeLeft /*m_game->m_gameTime*/ % 60);
        DrawText("TIME LEFT", TIME_PANEL_X, yPos, PANEL_COLOR, -1, -1, 2);
        DrawText(std::string(timeString), TIME_PANEL_X2, yPos, C_BLUE0 /* PANEL_COLOR*/, -1, -1, 2);
        yPos += TEXT_HEIGHT;

        char delayString[20];
        if(!m_game->m_gameOver)
        {
            int delay = (int)m_delay;
            // add provisional delay of planes in flight
            float provisionalDelay = 0;
            for(auto p : m_game->board->planes)
            {
                //if(p->mode != PlaneMode::Exited && p->mode != PlaneMode::Heading && !p->delay_done)
                if(!p->delay_done)
                {
                    auto edelay = p->ExpDelay(m_game->m_gameOver ? finishTime : totalTime);
                    if(edelay < 0)
                    {
                        edelay = 0;
                    }
                    provisionalDelay += edelay;
                }
            }
            delay += (int)provisionalDelay;
            totalDelay = delay;
        }

        if(!m_game->m_gameOver || m_game->m_flashOn || (m_game->m_gameOver && !m_game->m_winner))
        {
            DrawText("ALERTS", PLANE_PANEL_X, yPos, PANEL_COLOR, -1, -1, 2);
            std::snprintf(delayString, 20, "%03d", m_alerts);
            DrawText(std::string(delayString), TIME_PANEL_X2, yPos, C_BLUE0 /* PANEL_COLOR*/, -1, -1, 2);
        }
        yPos += TEXT_HEIGHT;
        //if(!m_game->m_gameOver || m_game->m_flashOn || (m_game->m_gameOver && !m_game->m_winner))
        {
            DrawText("TOTAL DELAYS", PLANE_PANEL_X, yPos, PANEL_COLOR, -1, -1, 2);
            std::snprintf(delayString, 20, "%02d:%02d", totalDelay / 60, totalDelay % 60);
            DrawText(std::string(delayString), TIME_PANEL_X2, yPos, C_BLUE0 /* PANEL_COLOR*/, -1, -1, 2);
        }
    }

    m_vectorBatch.DrawLine(TIME_PANEL_X - 0.5f, PLANE_PANEL_Y - 0.5f, TIME_PANEL_X + 10.0f, PLANE_PANEL_Y - 0.5F, C_BLUE1);
    m_vectorBatch.DrawLine(TIME_PANEL_X - 0.5f, MESSAGES_Y - 0.5f, TIME_PANEL_X + 10.0f, MESSAGES_Y - 0.5F, C_BLUE1);

    // draw plane info
    if(selectedPlane != nullptr && selectedPlane->mode != PlaneMode::Exited)
    {
        if(selectedPlane->type == PlaneType::Military)
        {
            if(m_game->m_flashOn)
                DrawText("CANNOT REROUTE MILITARY", HINT_PANEL_X, HINT_PANEL_Y, PANEL_COLOR, -1, -1, 2);
        }
        else
        {
            DrawText("CLICK TO REROUTE", HINT_PANEL_X, HINT_PANEL_Y, PANEL_COLOR, -1, -1, 2);
        }

        float yPos = PLANE_PANEL_Y;
        DrawText("CALL SIGN", PLANE_PANEL_X, yPos, PANEL_COLOR, -1, -1, 2);
        DrawText(selectedPlane->callSign, PLANE_PANEL_X2, yPos, PANEL_COLOR, -1, -1, 2);
        yPos += TEXT_HEIGHT;

        DrawText("DESTINATION", PLANE_PANEL_X, yPos, PANEL_COLOR, -1, -1, 2);
        if(selectedPlane->exit != nullptr)
        {
            DrawText(selectedPlane->exit->code, PLANE_PANEL_X2, yPos, PANEL_COLOR, -1, -1, 2);
        }
        yPos += TEXT_HEIGHT;

        bool        typeFlash = false;
        std::string typeString, speedString;
        switch(selectedPlane->type)
        {
        case PlaneType::Passenger:
            typeString  = "PASSENGER";
            speedString = "FAST";
            break;
        case PlaneType::Cargo:
            typeString  = "CARGO";
            speedString = "SLOW";
            break;
        case PlaneType::Military:
            typeString  = "MILITARY";
            speedString = "SUPERSONIC";
            typeFlash   = true;
            break;
        case PlaneType::VIP:
            typeString  = "VIP";
            speedString = "FAST";
            typeFlash   = true;
            break;
        default:
            typeString  = "";
            speedString = "";
            break;
        }

        DrawText("TYPE", PLANE_PANEL_X, yPos, PANEL_COLOR, -1, -1, 2);
        if(!typeFlash || m_game->m_flashOn)
            DrawText(typeString, PLANE_PANEL_X2, yPos, PANEL_COLOR, -1, -1, 2);
        yPos += TEXT_HEIGHT;

        DrawText("SPEED", PLANE_PANEL_X, yPos, PANEL_COLOR, -1, -1, 2);
        DrawText(speedString, PLANE_PANEL_X2, yPos, PANEL_COLOR, -1, -1, 2);
        yPos += TEXT_HEIGHT;

        DrawText("ORIGIN", PLANE_PANEL_X, yPos, PANEL_COLOR, -1, -1, 2);
        DrawText(selectedPlane->origin, PLANE_PANEL_X2, yPos, PANEL_COLOR, -1, -1, 2);
        yPos += TEXT_HEIGHT;

        char planeDelayString[20];
        int  planeDelay = (int)selectedPlane->ExpDelay(m_game->m_gameOver ? finishTime : totalTime);
        std::snprintf(planeDelayString, 20, "%02d:%02d", planeDelay / 60, planeDelay % 60);

        DrawText("DELAY", PLANE_PANEL_X, yPos, PANEL_COLOR, -1, -1, 2);
        DrawText(planeDelayString, PLANE_PANEL_X2, yPos, PANEL_COLOR, -1, -1, 2);
        yPos += TEXT_HEIGHT;

        char effectsString[20] = "";
        bool flash             = false;
        if(selectedPlane->alerts.size())
        {
            flash = true;
            std::snprintf(effectsString, 20, "ALERT");
        }
        else if(selectedPlane->incoming || selectedPlane->mode == PlaneMode::Heading)
        {
            std::snprintf(effectsString, 20, "CHANGING ALT");
        }
        else if(selectedPlane->areaSpeed != 1.0f)
        {
            flash = true;
            std::snprintf(effectsString, 20, "SPEED X%.1f", selectedPlane->areaSpeed);
        }
        else if(m_game->board->areas.size() && selectedPlane->type != PlaneType::Military)
        {
            for(auto a : m_game->board->areas)
            {
                if(a->type == AreaType::NoFly && a->IsAbove(selectedPlane->position))
                {
                    flash = true;
                    std::snprintf(effectsString, 20, "NO FLY DELAY");
                    break;
                }
            }
        }
        if(strlen(effectsString) == 0)
        {
            std::snprintf(effectsString, 20, "CRUISING");
        }
        DrawText("STATUS", PLANE_PANEL_X, yPos, PANEL_COLOR, -1, -1, 2);
        if(!flash || m_game->m_flashOn)
            DrawText(effectsString, PLANE_PANEL_X2, yPos, PANEL_COLOR, -1, -1, 2);
        yPos += TEXT_HEIGHT;
    }
    else
    {
        DrawText("NO PLANE SELECTED", PLANE_PANEL_X, PLANE_PANEL_Y, PANEL_COLOR, -1, -1, 2);
    }

    {
        // draw 10 messages
        float yPos = MESSAGES_Y;
        for(int i = messages.size() - 1; i >= 0 && i >= (int)messages.size() - (int)10; i--)
        {
            if(messages[i][0] != 'C' || messages[i][1] != 'R' || m_game->m_flashOn)
                DrawText(messages[i], MESSAGES_X, yPos, PANEL_COLOR, -1, -1, 2);
            yPos += TEXT_HEIGHT;
        }
    }

    Color c = C_BLUE2;
    if(IsHoveringRestartButton2())
    {
        c = C_BLUE0;
    }
    DrawText("RESTART", restart2Loc.x, restart2Loc.y, c, 0, 0, 2);
    m_vectorBatch.DrawObject(restartOutline, restart2Loc, 0, c);

    int hover = IsHoveringSpeedButton();
    DrawText("SPEED", PLANE_PANEL_X, restart2Loc.y, PANEL_COLOR, -1, 0, 2);
    DrawText("1", speed1Loc.x, speed1Loc.y, hover == 1 ? C_BLUE0 : C_BLUE2, 0, 0, 2);
    DrawText("2", speed2Loc.x, speed2Loc.y, hover == 2 ? C_BLUE0 : C_BLUE2, 0, 0, 2);
    DrawText("3", speed3Loc.x, speed3Loc.y, hover == 3 ? C_BLUE0 : C_BLUE2, 0, 0, 2);
    DrawText("4", speed4Loc.x, speed4Loc.y, hover == 4 ? C_BLUE0 : C_BLUE2, 0, 0, 2);
    if(m_game->timeScale == 1)
        m_vectorBatch.DrawObject(speedOutline, speed1Loc, 0, hover == 1 ? C_BLUE0 : C_BLUE2);
    if(m_game->timeScale == 2)
        m_vectorBatch.DrawObject(speedOutline, speed2Loc, 0, hover == 2 ? C_BLUE0 : C_BLUE2);
    if(m_game->timeScale == 3)
        m_vectorBatch.DrawObject(speedOutline, speed3Loc, 0, hover == 3 ? C_BLUE0 : C_BLUE2);
    if(m_game->timeScale == 4)
        m_vectorBatch.DrawObject(speedOutline, speed4Loc, 0, hover == 4 ? C_BLUE0 : C_BLUE2);

    int fhover = IsHoveringFocusButton();
    DrawText("FOCUS", PLANE_PANEL_X, focusOnLoc.y, PANEL_COLOR, -1, 0, 2);
    DrawText("ON", focusOnLoc.x, focusOnLoc.y, fhover == 1 ? C_BLUE0 : C_BLUE2, 0, 0, 2);
    DrawText("OFF", focusOffLoc.x, focusOffLoc.y, fhover == -1 ? C_BLUE0 : C_BLUE2, 0, 0, 2);
    if(m_game->m_noise)
        m_vectorBatch.DrawObject(focusOutline, focusOnLoc, 0, fhover == 1 ? C_BLUE0 : C_BLUE2);
    else
        m_vectorBatch.DrawObject(focusOutline, focusOffLoc, 0, fhover == -1 ? C_BLUE0 : C_BLUE2);
}

void GameScreen::Message(const std::string& msg)
{
    messages.push_back(msg);
}

void GameScreen::PlaneMissed(Plane* p)
{
    // draw time panel
    char msg[100];
    std::snprintf(msg, 100, "%s MISSED", p->callSign.c_str());
    Message(msg);
}

void GameScreen::PlaneLanded(Plane* p)
{
    // draw time panel
    char msg[100];
    int  delay = (int)p->Delay(totalTime);
    if(delay < 1)
        std::snprintf(msg, 100, "%s ON TIME", p->callSign.c_str());
    else
        std::snprintf(msg, 100, "%s DELAYED %02d:%02d", p->callSign.c_str(), delay / 60, delay % 60);

    PlaySoundMulti(landedSound);
    Message(msg);
}

void GameScreen::RemovePlane(Plane* p, PlaneMode mode)
{
    p->mode = mode;

    // clear alerts
    if(p->alerts.size())
    {
        for(auto p2 : m_game->board->planes)
        {
            if(p2->alerts.size())
            {
                p2->alerts.erase(p->id);
            }
        }
        p->alerts.clear();
    }

    // clear selection
    if(p->selected)
    {
        p->selected   = false;
        selectedPlane = nullptr;
    }
}

void GameScreen::CheckPlanes()
{
    for(auto p : m_game->board->planes)
    {
        // if plane is over its exit
        if(p->mode != PlaneMode::Exited)
        {
            if(p->exit->IsAbove(p->position))
            {
                if(p->exit->type == ExitType::Exit)
                {
                    // we hit our target corridor, just go away off screen
                    AddDelay(p);
                    RemovePlane(p, PlaneMode::Heading); // just go away from the screen
                }
                else
                {
                    PlaneLanded(p);
                    AddDelay(p);
                    RemovePlane(p, PlaneMode::Exited);
                }
            }

            // disappear planes off the board
            if((p->position.x < -BOUND_X || p->position.x > BOUND_X) || (p->position.y < -BOUND_Y || p->position.y > BOUND_Y))
            {
                AddDelay(p);
                RemovePlane(p, PlaneMode::Exited);
            }

            if(p->type != PlaneType::Military)
            {
                p->areaSpeed = 1.0f;
                if(m_game->board->areas.size())
                {
                    for(auto a : m_game->board->areas)
                    {
                        if(a->IsAbove(p->position))
                        {
                            if(a->type == AreaType::Weather)
                            {
                                p->areaSpeed = 0.5f; // slow down 2x
                                if(std::isnan(p->touchTime))
                                {
                                    // we need to add as extra delay
                                    p->extraDelay += deltaTime;
                                }
                            }
                            else if(a->type == AreaType::NoFly)
                            {
                                m_delay += deltaTime * 3; // 3x delay
                                p->extraDelay += deltaTime * 3;
                            }
                        }
                    }
                }
            }
        }
    }
}

void GameScreen::CheckCollisions()
{
    if(m_game->m_gameOver)
        return;
    haveAlerts = false;
    for(auto p1 : m_game->board->planes)
    {
        if(p1->mode == PlaneMode::Exited || p1->incoming || p1->mode == PlaneMode::Heading)
            continue;

        for(auto p2 : m_game->board->planes)
        {
            if(p2->mode == PlaneMode::Exited || p2->incoming || p2->mode == PlaneMode::Heading)
                continue;

            if(p1->id < p2->id && (p1->type != PlaneType::Military || p2->type != PlaneType::Military)) // Military can't alert each other
            {
                float dx = p2->position.x - p1->position.x;
                float dy = p2->position.y - p1->position.y;
                if(fabsf(dx) < PLANE_ALERT_DIST && fabsf(dy) < PLANE_ALERT_DIST)
                {
                    float d = sqrt(dx * dx + dy * dy);
                    if(d < PLANE_CRASH_DIST)
                    {
                        // crash!
                        char alertString[40];
                        std::snprintf(alertString, 40, "CRASH: %s AND %s", p1->callSign.c_str(), p2->callSign.c_str());
                        messages.push_back(alertString);

                        m_game->m_gameOver = true;
                        m_game->m_winner   = false;
                        if(std::isnan(finishTime))
                            finishTime = totalTime;

                        PlaySoundMulti(endSound);
                    }
                    else if(d < PLANE_ALERT_DIST)
                    {
                        haveAlerts = true;
                        // if not already alerted
                        if(p1->alerts.find(p2->id) == p1->alerts.end() || p2->alerts.find(p1->id) == p2->alerts.end())
                        {
                            m_alerts++;
                            p1->alerts.insert(p2->id);
                            p2->alerts.insert(p1->id);
                            char alertString[40];
                            std::snprintf(alertString, 40, "ALERT: %s AND %s", p1->callSign.c_str(), p2->callSign.c_str());
                            messages.push_back(alertString);
                        }
                    }
                }
                else if(p1->alerts.size() || p2->alerts.size())
                {
                    // TODO: don't do every time?
                    p1->alerts.erase(p2->id);
                    p2->alerts.erase(p1->id);
                }
            }
        }
    }
    if(haveAlerts && !IsMusicStreamPlaying(alertMusic))
        PlayMusicStream(alertMusic);
    else if(!haveAlerts && IsMusicStreamPlaying(alertMusic))
        StopMusicStream(alertMusic);
}

void GameScreen::Restart()
{
    m_game->SelectMap();
}