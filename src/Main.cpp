/*
The entry point for my AI assistant, June.

J.U.N.E
"Just Understands Nearly Everything"

Start Date: 08/19/2026
*/

#include <format>
#include "raylib.h"

#include "june/core/June.h"
#include "june/network/LlamaBackend.h"

int main() {
    june::LlamaBackend backend;
    backend.start();

    SetConfigFlags(FLAG_BORDERLESS_WINDOWED_MODE);
    InitWindow(3840, 2160, "June");
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLACK);
        DrawText(june::toString(backend.getState()), 50, 50, 40, WHITE);
        EndDrawing();
    }
    backend.stop();
    CloseWindow();
    return 0;
}