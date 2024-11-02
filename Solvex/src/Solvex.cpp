#define RAYGUI_IMPLEMENTATION
#define _CRT_SECURE_NO_WARNINGS
#include "raylib.h"
#include "raygui.h"
#include<string>
#include "../headers/data_functions.h"

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
    const Rectangle passwordInputBounds = { screenWidth / 2 - 60, screenHeight / 2 - 50, 120, 24 };
    const Rectangle newTransactionNoteInputBounds = { 30, 200, 120, 24 };
    const Rectangle newTransactionPriceInputBounds = { 150, 200, 120, 24 };
    const Rectangle newTransactionButtonBounds = { 370, 200, 120, 24 };
    const Rectangle saveButtonBounds = { 0, 0, 60, 24 };
    const Rectangle tabButtonBounds = { 30, 100, 60, 24 };
    const Rectangle scrollPanelBounds = { 30, 250, 1220, 200 };

    //stuff for the scrollbox elements
    const int transactionElementHeight = 30;
    const int transactionElementWidth = 1220-94;

    //will not keep the name giggidy for the window state i just couldn't think of a word yet (i am 90% sure this will not be changed)
    WindowState Window = ACCESS;
    Vector2 mousePosition;
    Vector2 scrollOffset = { 0, 0 };
    bool loginButtonPressed = false;
    bool signupButtonPressed = false;
    bool saveButtonPressed = false;
    bool newTransactionButtonPressed = false;
    char usernameInputText[32] = "Username";
    char passwordInputText[32] = "Password";
    char newTabInputText[32] = "";
    char newTransactionNoteInputText[128] = "Note";
    char newTransactionPriceInputText[32] = "0";

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

                accountLoad(&account, usernameInputText, passwordInputText);

                Window = APP;
            }
            if (signupButtonPressed)
            {
                signupButtonPressed = false;
                
                account.push_back({ usernameInputText });

                Window = APP;
            }
            break;

        case APP:
            //checks if char array is empty and if it isn't then it checks if the last char is a number or a dot and if it isn't it deletes the last char
            if (strlen(newTransactionPriceInputText) > 0)
            {
                if (!(newTransactionPriceInputText[std::strlen(newTransactionPriceInputText) - 1] >= '0' && newTransactionPriceInputText[std::strlen(newTransactionPriceInputText) - 1] <= '9' || newTransactionPriceInputText[std::strlen(newTransactionPriceInputText) - 1] == '.'))
                {
                    newTransactionPriceInputText[std::strlen(newTransactionPriceInputText) - 1] = '\0';
                }
            }


            if (saveButtonPressed)
            {
                saveButtonPressed = false;

                accountSave(account, usernameInputText, passwordInputText);
            }

            if (newTransactionButtonPressed)
            {
                newTransactionButtonPressed = false;
                if (!account.size()) break;

                account[selectedTab].transaction.push_back({ newTransactionNoteInputText, std::stof(newTransactionPriceInputText)});
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
            GuiTextBox(passwordInputBounds, passwordInputText, 32, CheckCollisionPointRec(mousePosition, passwordInputBounds));

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
                    if (GuiButton({ iTabButtonBounds.x, iTabButtonBounds.y - 24, iTabButtonBounds.width, iTabButtonBounds.height }, "REMOVE")) {
                        account.erase(account.begin() + i);
                        selectedTab = (selectedTab != 0) ? selectedTab - 1 : 0; //if we dont backtrack the selected tab it will try to read out of range memory and crash
                    }
                }
                else
                {
                    if (GuiButton(iTabButtonBounds, "ADD")) {
                        account.push_back({newTabInputText});
                        newTabInputText[0] = '\0';
                    }

                    Rectangle newTabInputBounds = iTabButtonBounds;
                    newTabInputBounds.y += tabButtonBounds.height;
                    GuiTextBox(newTabInputBounds, newTabInputText, 32, CheckCollisionPointRec(mousePosition, newTabInputBounds));
                }
            }

            GuiTextBox(newTransactionNoteInputBounds, newTransactionNoteInputText, 32, CheckCollisionPointRec(mousePosition, newTransactionNoteInputBounds));
            GuiTextBox(newTransactionPriceInputBounds, newTransactionPriceInputText, 32, CheckCollisionPointRec(mousePosition, newTransactionPriceInputBounds));
            newTransactionButtonPressed = GuiButton(newTransactionButtonBounds, "ADD");

            if (!account.size()) break;

            GuiScrollPanel(scrollPanelBounds, nullptr, { 0, 0, 380, float((account[selectedTab].transaction.size() * transactionElementHeight)) }, &scrollOffset, nullptr);

            BeginScissorMode(scrollPanelBounds.x, scrollPanelBounds.y, scrollPanelBounds.width, scrollPanelBounds.height);
            
            for (int i = 0;i < account[selectedTab].transaction.size(); i++) {
                int index = account[selectedTab].transaction.size() - i - 1; //index is used for displaying the transactions from the last upward so the latest one is at the top
                float transactionYPosition = scrollPanelBounds.y + scrollOffset.y + (i * transactionElementHeight);
                Color backgroundColor = (i % 2 == 0) ? Fade(LIGHTGRAY, 0.5f) : Fade(LIGHTGRAY, 0.3f); //GRAY and LIGHTGRAY look bad together so i used fade which applies transparency
                Color textColor = (account[selectedTab].transaction[index].amount < 0) ? RED : DARKGREEN; //normal green was hard to read on the background
                DrawRectangle(scrollPanelBounds.x, transactionYPosition, transactionElementWidth, transactionElementHeight, backgroundColor);

                // Draw text and corresponding number
                DrawText(account[selectedTab].transaction[index].note.c_str(), scrollPanelBounds.x + 15, transactionYPosition + 10, 10, DARKGRAY);
                std::string numberText = std::to_string(account[selectedTab].transaction[index].amount);
                DrawText(numberText.c_str(), scrollPanelBounds.x + transactionElementWidth - MeasureText(numberText.c_str(), 10) - 15, transactionYPosition + 10, 10, textColor);

                // Generates the remove button
                Rectangle removeButtonBounds = { scrollPanelBounds.x + transactionElementWidth + 5, transactionYPosition + 5, 70, 20 };
                if (GuiButton(removeButtonBounds, "REMOVE")) {
                    account[selectedTab].transaction.erase(account[selectedTab].transaction.begin() + index);
                }
            }
            EndScissorMode();

            break;
        }

        EndDrawing();
    }
    // De-Initialization
    CloseWindow();

    return 0;
}