#define RAYGUI_IMPLEMENTATION
#define _CRT_SECURE_NO_WARNINGS
#include "raylib.h"
#include "raygui.h"
#include<string>
#include<iostream>
#include <vector>

struct TRANSACTION_STRUCT {
    std::string note;
    float amount;
};

struct TAB_STRUCT{
    std::string tabName;
    std::vector<TRANSACTION_STRUCT> transaction;
};

enum WindowState {
    ACCESS,
    APP
};

// Program main entry point

int main(void)
{
    // Initialization

    const int screenWidth = 1280;
    const int screenHeight = 720;

    //ALL BOXES WILL HAVE THEIR BOUNDS SET HERE BEFORE INITIALISATION (i didnt bother doing this for half of them) 
    const Rectangle usernameInputBounds = { screenWidth / 2 - 60, screenHeight / 2 - 100, 120, 24 };
    const Rectangle usernamePasswordBounds = { screenWidth / 2 - 60, screenHeight / 2 - 50, 120, 24 };
    const Rectangle saveButtonBounds = { 0, 0, 60, 24 };
    const Rectangle tabButtonBounds = { 30, 100, 60, 24 };

    //will not keep the name giggidy for the window state i just couldn't think of a word yet (i am 90% sure this will not be changed)
    WindowState Window = ACCESS;
    Vector2 mousePosition;
    bool loginButtonPressed = false;
    bool signupButtonPressed = false;
    bool saveButtonPressed = false;
    char usernameInputText[32] = "Username";
    char passwordInputText[32] = "Password";
    char newTabInputText[32] = "";

    std::vector<TAB_STRUCT> account;
    int selectedTab = 0;

    InitWindow(screenWidth, screenHeight, "Solvex");

    SetTargetFPS(60);

    // Main game loop
    while (!WindowShouldClose())
    {
        // Update your variables here BEFORE DRAWING unless necessary
        mousePosition = GetMousePosition();
        //Window state specific logic goes here (so it doesnt run needlessly (beep boop totally made by chatgpt))
        switch (Window)
        {
        case ACCESS:
            if (loginButtonPressed)
            {
                loginButtonPressed = false;

                Window = APP;
                std::cout << usernameInputText << std::endl;
            }
            if (signupButtonPressed)
            {
                account.push_back({
                    usernameInputText, // tabName for the new subaccount
                    {
                      {
                        "Transaction 1",
                        300.0f
                      }, // First transaction for the second account
                      {
                        "Transaction 2",
                        400.0f
                      } // Second transaction for the second account
                    }
                    });
                account[0].transaction.push_back({ "Transaction 3", 300.0f });
            }
            break;

        case APP:
            if (saveButtonPressed)
            {
                saveButtonPressed = false;

                std::cout << "no saving implemented yet lolololol" << std::endl;
            }
            break;
        }
        // Draw
        BeginDrawing();

        ClearBackground(RAYWHITE);
        
        switch (Window)
        {
        case ACCESS:

            GuiTextBox(usernameInputBounds, usernameInputText, 32, CheckCollisionPointRec(mousePosition, usernameInputBounds));
            GuiTextBox(usernamePasswordBounds, passwordInputText, 32, CheckCollisionPointRec(mousePosition, usernamePasswordBounds));

            loginButtonPressed = GuiButton({ screenWidth/2 - 60, screenHeight/2, 60, 24 }, "Login");
            signupButtonPressed = GuiButton({ screenWidth/2, screenHeight/2, 60, 24 }, "Signup");
            break;

        case APP:
            saveButtonPressed = GuiButton(saveButtonBounds, "Save");
            for (size_t i = 0; i <= account.size(); i++)
            {
                Rectangle iTabButtonBounds = tabButtonBounds;
                iTabButtonBounds.x += tabButtonBounds.width*i;
                if (i < account.size())
                {
                    if (GuiButton(iTabButtonBounds, account[i].tabName.c_str())) {
                        selectedTab = i;
                    }
                }
                else
                {
                    if (GuiButton(iTabButtonBounds, "Add")) {
                        account.push_back({newTabInputText});
                        newTabInputText[0] = '\0';
                    }

                    Rectangle newTabInputBounds = iTabButtonBounds;
                    newTabInputBounds.y += tabButtonBounds.height;
                    GuiTextBox(newTabInputBounds, newTabInputText, 32, CheckCollisionPointRec(mousePosition, newTabInputBounds));
                }
            }
            break;

        }

        EndDrawing();
    }
    // De-Initialization
    CloseWindow();// Close window and OpenGL context

    return 0;
}