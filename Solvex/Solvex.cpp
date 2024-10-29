#define RAYGUI_IMPLEMENTATION
#define _CRT_SECURE_NO_WARNINGS
#include "raylib.h"
#include "raygui.h"
#include<string>
#include<iostream>
#include <vector>

struct subAccount{
    double balance = 0;
    struct transactions {
        std::vector<int> transactionAmount;
        std::vector<std::string> transactionName;
    };
};

struct account {
    std::string username;
    std::string password;
    std::vector<subAccount> subAccounts;
};

enum WindowState {
    LOGIN,
    APP
};

// Program main entry point

int main(void)
{
    // Initialization

    const int screenWidth = 1000;
    const int screenHeight = 500;

    //ALL BOXES WILL HAVE THEIR BOUNDS SET HERE BEFORE INITIALISATION UNLESS THEY'RE DYNAMIC (i didnt bother doing this for half of them) 
    const Rectangle UsernameInputBounds = { screenWidth / 2 - 60, screenHeight / 2 - 100, 120, 24 };
    const Rectangle UsernamePasswordBounds = { screenWidth / 2 - 60, screenHeight / 2 - 50, 120, 24 };

    //will not keep the name giggidy for the window state i just couldn't think of a word yet (i am 90% sure this will not be changed)
    WindowState giggidy = LOGIN;
    Vector2 MousePosition;
    bool LoginButtonPressed = false;
    bool SignupButtonPressed = false;
    char UsernameinputText[32] = "Username";
    char PasswordinputText[32] = "Password";

    InitWindow(screenWidth, screenHeight, "Solvex");

    SetTargetFPS(60);

    // Main game loop
    while (!WindowShouldClose())
    {
        // Update your variables here BEFORE DRAWING unless necessary
        MousePosition = GetMousePosition();
        if (LoginButtonPressed)
        {
            LoginButtonPressed = false;
            giggidy = APP;
            std::cout << UsernameinputText << std::endl;
        }
        if (SignupButtonPressed)
        {
            SignupButtonPressed = false;
            std::cout << "idk how to make accounts yet lolzers" << std::endl;
        }


        // Draw
        BeginDrawing();

        ClearBackground(RAYWHITE);

        switch (giggidy)
        {
        case LOGIN:
            GuiTextBox(UsernameInputBounds, UsernameinputText, 32, CheckCollisionPointRec(MousePosition, UsernameInputBounds));
            GuiTextBox(UsernamePasswordBounds, PasswordinputText, 32, CheckCollisionPointRec(MousePosition, UsernamePasswordBounds));

            LoginButtonPressed = GuiButton({ screenWidth/2 - 60, screenHeight/2, 60, 24 }, "Login");
            SignupButtonPressed = GuiButton({ screenWidth/2, screenHeight/2, 60, 24 }, "Signup");
            break;
        case APP:

            //THIS IS ONLY FOR TESTING PURPOSES
            for (int i = 0; i < 5; i++)
            {
                if (i % 2 == 0) DrawRectangle(screenWidth / 2, (30 + 20 * i), 200, 20, Fade(LIGHTGRAY, 0.5f));
                else DrawRectangle(screenWidth / 2, (30 + 20 * i), 200, 20, Fade(LIGHTGRAY, 0.3f));

                if (i < 5 - 1)
                {
                    DrawText("word", screenWidth / 2, (36 + 20 * i), 10, DARKGRAY);
                    DrawText("num", screenWidth / 2 + 30, (36 + 20 * i), 10, DARKGRAY);
                }
                else
                {
                    DrawText("rizz", screenWidth / 2, (36 + 20 * i), 10, MAROON);
                    DrawText("13", screenWidth / 2 + 30, (36 + 20 * i), 10, MAROON);
                }
            }
            break;
        default: break;
        }

        EndDrawing();
    }
    // De-Initialization
    CloseWindow();// Close window and OpenGL context

    return 0;
}