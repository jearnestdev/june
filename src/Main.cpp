/*
The entry point for my AI assistant, June.

J.U.N.E
"Just Understands Nearly Everything"

Start Date: 08/19/2026
*/

#include "raylib.h"

#include "june/core/June.h"
#include "june/network/LlamaServer.h"

int main() {
    june::LlamaServer server;
    auto server_init_status = server.init();

    InitWindow(1280, 720, "June");
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLACK);
        DrawText("June", 50, 50, 40, WHITE);
        EndDrawing();
    }
    server.shutdown();
    CloseWindow();
    return 0;
}