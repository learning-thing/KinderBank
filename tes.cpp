#include <iostream>
#include "raylib.h"
#include <cmath> 
#include "raymath.h"

#define GLSL_VERSION  330

class Environment {
public:
    float gravity = 0.5f;
};

class Ground {
public:
    Rectangle ground;

    Ground(float x, float y, float w, float h) {
        ground = {x, y, w, h};
    }

    void updateSize(float w, float h) {
        ground.width = w;
        ground.y = h - 150; 
    }

    void draw() {
        DrawRectangleRounded(ground, 1, 1, Color({238, 235, 211, 55}));
    }
};

class Player {
public:
    float speed = 2.0f;
    float velocityY = 0.0f;      
    float jumpForce = -10.0f;  
    float coefficientOfElasticity = 0.2f; 
    float jumpBufferTime = 0.0f; 
    float maxJumpHeight = 150.0f; 

    bool onGround = false;       
    Rectangle body;          

    float dashSpeed = 400.0f;
    float dashDuration = 0.1f;
    float dashCooldown = 1.0f;
    float dashTimer = 0.0f;
    float lastDashTime = 0.0f;
    Vector2 dashStart;
    Vector2 dashEnd;
    bool isDashing=false;


    Player(float x, float y, float w, float h) {
        body = {x, y, w, h};
    }

    void draw() {
        DrawRectangleRounded(body, 1, 1, Color({238, 235, 211, 155}));
    }

    void handleControl() {
        if(!isDashing){
            if (IsKeyDown(KEY_A)) { body.x -= speed; }
            if (IsKeyDown(KEY_D)) { body.x += speed; }
        }
        if (IsKeyPressed(KEY_SPACE) && (onGround || jumpBufferTime > 0) && body.y >= maxJumpHeight) {
            velocityY = jumpForce; 
            onGround = false;   
        }
        if (IsKeyPressed(KEY_E)) {
            std::cout<<"E pressed"<<std::endl;
            if (!isDashing && (GetTime() - lastDashTime > dashCooldown)) {
                startDash();
            }
        }
    }

    void applyPhysics(Rectangle groundRect, float gravity) {
        if(!isDashing){
            velocityY += gravity; 
            body.y += velocityY*GetFrameTime()*GetMonitorRefreshRate(0);  
        }
        if (CheckCollisionRecs(body, groundRect)) {
            body.y = groundRect.y - body.height; 
            velocityY = -velocityY * coefficientOfElasticity;  
            if (fabs(velocityY) < 1.0f) {  
                velocityY = 0;
                onGround = true;
            }
        } else {
            onGround = false; 
        }
        if (isDashing) {
            updateDash();
        }
    }

    void startDash() {
        Vector2 direction = {0, 0};
        if (IsKeyDown(KEY_A)) direction.x -= 1;
        if (IsKeyDown(KEY_D)) direction.x += 1;
        if (IsKeyDown(KEY_W)) direction.y -= 1;
        if (IsKeyDown(KEY_S)) direction.y += 1;

        float length = sqrt(direction.x * direction.x + direction.y * direction.y);
        if (length > 0) {
            direction.x /= length;
            direction.y /= length;
        }else{
            return;
        }

        dashStart = {body.x, body.y};
        dashEnd = {body.x + direction.x * dashSpeed, body.y + direction.y * dashSpeed};
        dashTimer = dashDuration;
        isDashing = true;
        lastDashTime = GetTime();
    }

    void updateDash() {
        if (dashTimer > 0) {
            body.x = Lerp(dashStart.x, dashEnd.x, 1.0f - (dashTimer / dashDuration));
            body.y = Lerp(dashStart.y, dashEnd.y, 1.0f - (dashTimer / dashDuration));
            dashTimer -= GetFrameTime();
        } else {
            isDashing = false;
        }
    }
};

int main(void) {
    const int screenWidth = 1280;
    const int screenHeight = 720;
    
    InitWindow(screenWidth, screenHeight, "raylib - Physics Simulation");

    Shader BackgroundShader = LoadShader(0, TextFormat("shaders/background.fs", GLSL_VERSION));
    Font FunnelDisplay = LoadFont(TextFormat("/home/lamao/workdir/MechinicaRaylib/Fonts/Funnel_Display/static/FunnelDisplay-Regular.ttf"));

    SetTargetFPS(100);

    int monitor = GetCurrentMonitor();
    int monitor_width = GetMonitorWidth(monitor);
    int monitor_height = GetMonitorHeight(monitor);
    bool is_full_screen = false;

    Environment world;
    Ground platform(0, screenHeight - 150, screenWidth , 50);
    Player player(100, 100, 50, 100);

    float ResolutionVector[2] = {(float)screenWidth, (float)screenHeight};
    SetShaderValue(BackgroundShader, GetShaderLocation(BackgroundShader, "iResolution"), ResolutionVector, SHADER_UNIFORM_VEC2);

    while (!WindowShouldClose()) {
        player.handleControl();
        player.applyPhysics(platform.ground, world.gravity);  

        if (IsKeyPressed(KEY_F)) {
            is_full_screen = !is_full_screen;

            if (is_full_screen) {
                SetWindowSize(monitor_width, monitor_height);
                ToggleFullscreen();
            } else {
                ToggleFullscreen();
                SetWindowSize(screenWidth, screenHeight);
            }

            float newWidth = (float)GetScreenWidth();
            float newHeight = (float)GetScreenHeight();
            float ResolutionVector[2] = {newWidth, newHeight};
            SetShaderValue(BackgroundShader, GetShaderLocation(BackgroundShader, "iResolution"), ResolutionVector, SHADER_UNIFORM_VEC2);

            platform.updateSize(newWidth, newHeight);
        }

        

        BeginDrawing();
            ClearBackground(BLACK);

            BeginShaderMode(BackgroundShader);
                DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), BLACK);
            EndShaderMode();

            DrawFPS(10, 10);

            DrawTextEx(FunnelDisplay, TextFormat("<---%dx%d--->", (int)platform.ground.width, (int)platform.ground.height),
                Vector2({10, platform.ground.y + platform.ground.height + 10}), 16, 4, Color({238, 235, 211, 155}));

            platform.draw();
            player.draw();
        EndDrawing();
    }

    CloseWindow();
    return 0;
}