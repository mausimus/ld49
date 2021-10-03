#include "stdafx.h"
#include "AirGame.h"
#include "utils.h"

#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

#if defined(PLATFORM_DESKTOP)
#define GLSL_VERSION 330
#else // PLATFORM_RPI, PLATFORM_ANDROID, PLATFORM_WEB
#define GLSL_VERSION 100
#endif

#ifdef GLOW
#define SHADER_PATH "resources/shaders/glsl%i/glow.fs"
#else
#define SHADER_PATH "resources/shaders/glsl%i/base.fs"
#endif

bool            firstFrame = true;
Shader          glowShader;
RenderTexture2D vectorTarget;
RenderTexture2D processingTarget;
Rectangle       textureRect {0, 0, 0, 0};
Rectangle       renderRect {0, 0, 0, 0};
AirGame         game;
Vector2         topLeft {0, 0};
double          totalTime;
Camera2D        camera;
Shader          vectorShader;
float           thick;
int             axisUniform;
int             texture1Location;
int             xsUniform;
int             ysUniform;
int             orgW, orgH;
const int       zero = 0;
const int       one  = 1;
int             pw, ph;
const float     TIME_SCALE = 2.0f;
double          previousTime;

#ifdef GLOW
bool glow = true;
#else
bool glow = false;
#endif

#ifdef DRAW_PIXELS
Color     framebuffer[screenWidth * screenHeight];
Rectangle bufferRect;
bool      fullFrame;
#endif

void UpdateDrawFrame(void);

void UpdateRenderSize(Rectangle& renderRect)
{
    auto viewportWidth  = GetScreenWidth();
    auto viewportHeight = GetScreenHeight();

    if(pw == viewportWidth && ph == viewportHeight)
        return;

    TraceLog(LOG_WARNING, "UpdateRenderSize %d %d", viewportWidth, viewportHeight);

    pw = viewportWidth;
    ph = viewportHeight;

    renderRect.width  = viewportWidth;
    renderRect.height = viewportHeight;
    if(renderRect.width * aspectV != renderRect.height * aspectH)
    {
        if(renderRect.width * aspectV > renderRect.height * aspectH)
        {
            renderRect.width = (renderRect.height * aspectH) / aspectV;
        }
        else
        {
            renderRect.height = (renderRect.width * aspectV) / aspectH;
        }
    }
    renderRect.x = (viewportWidth - renderRect.width) / 2;
    renderRect.y = (viewportHeight - renderRect.height) / 2;
    SetMouseOffset(-renderRect.x, -renderRect.y);

    if(!firstFrame)
    {
        UnloadRenderTexture(vectorTarget);
        UnloadRenderTexture(processingTarget);
    }

    vectorTarget = LoadRenderTexture(renderRect.width, renderRect.height); // 16x9 render size
    SetTextureFilter(vectorTarget.texture, TEXTURE_FILTER_BILINEAR);
    SetTextureWrap(vectorTarget.texture, TEXTURE_WRAP_CLAMP);

    processingTarget = LoadRenderTexture(renderRect.width, renderRect.height); // 16x9 render size
    SetTextureFilter(processingTarget.texture, TEXTURE_FILTER_BILINEAR);
    SetTextureWrap(processingTarget.texture, TEXTURE_WRAP_CLAMP);

    textureRect.width  = renderRect.width;
    textureRect.height = -renderRect.height;

    camera.zoom     = (renderRect.height / 2) / 9.0f;
    camera.offset   = (Vector2) {renderRect.width / 2, renderRect.height / 2};
    camera.rotation = 0;

    thick = thickFactor / (renderRect.height / 1000.0f);

    float oneOverWidth  = 1 / renderRect.width;
    float oneOverHeight = 1 / renderRect.height;

    SetShaderValue(glowShader, xsUniform, &oneOverWidth, SHADER_UNIFORM_FLOAT);
    SetShaderValue(glowShader, ysUniform, &oneOverHeight, SHADER_UNIFORM_FLOAT);

    game.Render(thick);
}

int main()
{
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(screenWidth, screenHeight, WINDOW_TITLE);
    SetWindowState(FLAG_VSYNC_HINT);
    InitAudioDevice();
    SetWindowMinSize(screenWidth / 4, screenHeight / 4);
    glowShader   = LoadShader(0, TextFormat(SHADER_PATH, GLSL_VERSION));
    vectorShader = LoadShader(TextFormat("resources/shaders/glsl%i/base.vs", GLSL_VERSION),
                              TextFormat("resources/shaders/glsl%i/vector.fs", GLSL_VERSION));

    axisUniform      = GetShaderLocation(glowShader, "axis");
    xsUniform        = GetShaderLocation(glowShader, "xs");
    ysUniform        = GetShaderLocation(glowShader, "ys");
    texture1Location = GetShaderLocation(glowShader, "texture1");

    std::srand((unsigned)std::time(0));

    game.Load();

    UpdateRenderSize(renderRect);
    
    HideCursor();
    previousTime = GetTime();
    totalTime    = previousTime * TIME_SCALE;
    game.Start(totalTime);

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(UpdateDrawFrame, 0, 1);
#else
    SetTargetFPS(TARGET_FPS);
    while(!WindowShouldClose())
    {
        UpdateDrawFrame();
    }
#endif

    game.End();
    UnloadRenderTexture(vectorTarget);
    UnloadRenderTexture(processingTarget);
    CloseAudioDevice();
    CloseWindow();

    return 0;
}

void UpdateDrawFrame()
{
#ifdef FULLSCREEN_KEY
    if(IsKeyPressed(FULLSCREEN_KEY))
    {
#if defined(PLATFORM_WEB)
//        ToggleFullscreen();
#else
        if(IsWindowFullscreen())
        {
            ToggleFullscreen();
            SetWindowSize(orgW, orgH);
        }
        else
        {
            orgW   = GetScreenWidth();
            orgH   = GetScreenHeight();
            auto m = GetCurrentMonitor();
            auto w = GetMonitorWidth(m);
            auto h = GetMonitorHeight(m);
            SetWindowSize(w, h);
            ToggleFullscreen();
        }

        UpdateRenderSize(renderRect);
#endif
    }
#endif

    if(IsKeyPressed(KEY_ONE))
    {
        game.timeScale = 1;
    }
    if(IsKeyPressed(KEY_TWO))
    {
        game.timeScale = 2;
    }
    if(IsKeyPressed(KEY_THREE))
    {
        game.timeScale = 3;
    }
    if(IsKeyPressed(KEY_FOUR))
    {
        game.timeScale = 4;
    }
/*
    if(IsKeyPressed(KEY_G))
    {
        glow = !glow;
    }
*/
    game.m_mouseX = 9 * (GetMouseX() - renderRect.width / 2) / (renderRect.height / 2);
    game.m_mouseY = 9 * (GetMouseY() - renderRect.height / 2) / (renderRect.height / 2);

    auto currentTime = GetTime();
    auto timeDelta   = currentTime - previousTime;
    previousTime     = currentTime;

    auto scaledDelta = timeDelta * TIME_SCALE * game.timeScale;
    totalTime += scaledDelta;
    game.Tick(scaledDelta, totalTime);

    bool windowResized = IsWindowResized();
    if(windowResized)
    {
        UpdateRenderSize(renderRect);
    }

    BeginDrawing();
    {

#ifdef DRAW_BACKGROUND
        BeginTextureMode(vectorTarget);
        game.DrawBackground();
        EndTextureMode();
#endif

        /*
#ifdef DRAW_PIXELS
        game.DrawPixels(framebuffer, &bufferRect, &fullFrame);
        if(fullFrame)
            UpdateTexture(target.texture, framebuffer);
        else
            UpdateTextureRec(target.texture, bufferRect, framebuffer);
#endif
*/

#ifdef DRAW_SHAPES
        BeginTextureMode(vectorTarget);
        ClearBackground(BLACK);
        BeginMode2D(camera);
        BeginShaderMode(vectorShader);
        game.DrawVectors(thick);
        EndMode2D();
        EndShaderMode();
        EndTextureMode();
#endif

        if(glow)
        {
            BeginTextureMode(processingTarget);
            BeginShaderMode(glowShader);
            SetShaderValue(glowShader, axisUniform, &zero, SHADER_UNIFORM_INT);
            //ClearBackground(BLACK);
            DrawTexturePro(
                vectorTarget.texture, textureRect, (Rectangle) {0, 0, textureRect.width, -textureRect.height}, (Vector2) {0, 0}, 0, WHITE);
            EndShaderMode();
            EndTextureMode();

            BeginShaderMode(glowShader);
            SetShaderValue(glowShader, axisUniform, &one, SHADER_UNIFORM_INT);
            SetShaderValueTexture(glowShader, texture1Location, vectorTarget.texture);
            if(windowResized || firstFrame)
            {
                ClearBackground(BLACK);
                firstFrame = false;
            }
            DrawTexturePro(processingTarget.texture, textureRect, renderRect, topLeft, 0, WHITE);
            EndShaderMode();
        }
        else
        {
            if(windowResized || firstFrame)
            {
                ClearBackground(BLACK);
                firstFrame = false;
            }
            DrawTexturePro(vectorTarget.texture, textureRect, renderRect, topLeft, 0, WHITE);
        }
        EndDrawing();
    }
}