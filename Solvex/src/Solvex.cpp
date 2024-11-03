#define RAYGUI_IMPLEMENTATION
//had to add this cause raygui.h has warnings
#define _CRT_SECURE_NO_WARNINGS
#include "raylib.h"
#include "raygui.h"
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
    const Rectangle newTransactionAmountInputBounds = { 150, 200, 120, 24 };
    const Rectangle newTransactionButtonBounds = { 370, 200, 120, 24 };
    const Rectangle saveButtonBounds = { 0, 0, 60, 24 };
    const Rectangle tabButtonBounds = { 30, 100, 60, 24 };
    const Rectangle scrollPanelBounds = { 30, 250, 1220, 400 };

    //stuff for the scrollbox elements
    const int transactionElementHeight = 30;
    const int transactionElementWidth = 1220 - 94;

    WindowState Window = ACCESS;
    Vector2 mousePosition;
    Vector2 scrollOffset = { 0, 0 };
    bool loginButtonPressed = false;
    bool signupButtonPressed = false;
    bool acknowledge_tampering = false;
    bool saveButtonPressed = false;
    bool newTransactionButtonPressed = false;
    char usernameInputText[32] = "Username";
    char passwordInputText[32] = "Password";
    std::string loginErrorText = "";
    char newTabInputText[32] = "";
    char newTransactionNoteInputText[128] = "Note";
    char newTransactionAmountInputText[32] = "0";
    std::string tabSum = "SUM: 0";
    float newTransactionAmountInputFloat = 0;
    float sum = 0;

    std::vector<TAB_STRUCT> account;
    int selectedTab = 0;

    InitWindow(screenWidth, screenHeight, "Solvex");

    SetTargetFPS(60);

    // Main game loop
    while (!WindowShouldClose())
    {
        // Update your variables here BEFORE DRAWING unless necessary
        mousePosition = GetMousePosition();

        //Window state for logic
        switch (Window)
        {
        case ACCESS:
            //runs when the login button is pressed 
            if (loginButtonPressed)
            {
                loginButtonPressed = false;

                if (strlen(passwordInputText) <= 4)
                {
                    loginErrorText = "password must be longer than 4 characters";
                    break;
                }

                //loads the account file and returns a status enum directly into a switch case
                switch (accountLoad(&account, usernameInputText, passwordInputText, acknowledge_tampering))
                {
                case FINE:
                    Window = APP;
                    selectedTab = 0;


                    for (int i = 0; i < account[selectedTab].transaction.size(); i++)
                    {
                        sum += account[selectedTab].transaction[i].amount;
                    }
                    tabSum = "SUM: " + std::to_string(sum);

                    break;
                case NOT_EXIST:
                    loginErrorText = "Account does not exist";
                    break;
                case INTEGRITY:
                    loginErrorText = "Account may have been tampered with, click login again to acknowledge";
                    acknowledge_tampering = true;
                    break;
                case CORRUPTED:
                    loginErrorText = "Account file is corrupted";
                    break;
                case WRONG_PASS:
                    loginErrorText = "Account password is wrong";
                    break;
                }
            }

            //when the sign up button is pressed a single tab is created with the username as the name
            if (signupButtonPressed)
            {
                signupButtonPressed = false;

                account.push_back({ usernameInputText });

                Window = APP;
            }
            break;

            //logic for the actual app
        case APP:

            //runs when the save button is pressed also only saves the account on user request
            if (saveButtonPressed)
            {
                saveButtonPressed = false;

                accountSave(account, usernameInputText, passwordInputText);
            }

            //runs when the new transaction button is pressed and creates a new transaction and updates the sum
            if (newTransactionButtonPressed)
            {
                newTransactionButtonPressed = false;
                if (!account.size()) break;

                account[selectedTab].transaction.push_back({ newTransactionNoteInputText, std::stof(newTransactionAmountInputText) });

                sum = 0;
                for (int i = 0; i < account[selectedTab].transaction.size(); i++)
                {
                    sum += account[selectedTab].transaction[i].amount;
                }
                tabSum = "SUM: " + std::to_string(sum);
            }

            break;
        }
        // Drawing starts now
        BeginDrawing();

        ClearBackground(RAYWHITE);

        //switch case for rendering
        switch (Window)
        {
            //runs for the login screen
        case ACCESS:
            //username input box
            GuiTextBox(usernameInputBounds, usernameInputText, 32, CheckCollisionPointRec(mousePosition, usernameInputBounds));
            //password input box
            GuiTextBox(passwordInputBounds, passwordInputText, 32, CheckCollisionPointRec(mousePosition, passwordInputBounds));

            //login button
            loginButtonPressed = GuiButton({ screenWidth / 2 - 60, screenHeight / 2, 60, 24 }, "Login");
            //sign up button
            signupButtonPressed = GuiButton({ screenWidth / 2, screenHeight / 2, 60, 24 }, "Signup");

            //draws the error message if there is one
            DrawText(loginErrorText.c_str(), screenWidth / 2 - MeasureText(loginErrorText.c_str(), 10), 450, 20, RED);
            break;


            //rendering for the actual app
        case APP:

            //draws the save button
            saveButtonPressed = GuiButton(saveButtonBounds, "Save");

            //draws the tabs
            for (size_t i = 0; i <= account.size(); i++)
            {
                Rectangle iTabButtonBounds = tabButtonBounds;
                iTabButtonBounds.x += tabButtonBounds.width * i;
                if (i < account.size())
                {
                    //when you click a tab you select it
                    if (GuiButton(iTabButtonBounds, account[i].tabName.c_str())) {
                        selectedTab = i;
                    }
                    //draws remove button (if you remove a tab you are viewing you view the one before it unless it was the last one)
                    if (GuiButton({ iTabButtonBounds.x, iTabButtonBounds.y - 24, iTabButtonBounds.width, iTabButtonBounds.height }, "REMOVE")) {
                        account.erase(account.begin() + i);
                        selectedTab = (selectedTab != 0) ? selectedTab - 1 : 0; //if we dont backtrack the selected tab it will try to read out of range memory and crash
                    }
                }
                else
                {
                    //draws the add button
                    if (GuiButton(iTabButtonBounds, "ADD")) {
                        account.push_back({ newTabInputText });
                        newTabInputText[0] = '\0';
                    }

                    //draws the name input box below it
                    Rectangle newTabInputBounds = iTabButtonBounds;
                    newTabInputBounds.y += tabButtonBounds.height;
                    GuiTextBox(newTabInputBounds, newTabInputText, 32, CheckCollisionPointRec(mousePosition, newTabInputBounds));
                }
            }

            //draws the transaction note
            GuiTextBox(newTransactionNoteInputBounds, newTransactionNoteInputText, 32, CheckCollisionPointRec(mousePosition, newTransactionNoteInputBounds));
            //draws the transaction amount
            GuiValueBoxFloat(newTransactionAmountInputBounds, "", newTransactionAmountInputText, &newTransactionAmountInputFloat, CheckCollisionPointRec(mousePosition, newTransactionAmountInputBounds));
            //draws the button that adds a new transaction
            newTransactionButtonPressed = GuiButton(newTransactionButtonBounds, "ADD");

            //draws the sum text
            DrawText(tabSum.c_str(), 1260 - MeasureText(tabSum.c_str(), 20) - 15, 200 + 6, 20, (std::stof(tabSum.substr(4)) < 0) ? RED : DARKGREEN);

            //stops rendering if there are no tabs because it will crash
            if (!account.size())
            {
                tabSum = "SUM: 0";
                break;
            }

            //this is all for the transaction list
            GuiScrollPanel(scrollPanelBounds, nullptr, { 0, 0, 380, float((account[selectedTab].transaction.size() * transactionElementHeight)) }, &scrollOffset, nullptr);

            //renders stff inside a box and everything outside that box gets cut
            BeginScissorMode(scrollPanelBounds.x, scrollPanelBounds.y, scrollPanelBounds.width, scrollPanelBounds.height);

            for (int i = 0; i < account[selectedTab].transaction.size(); i++) {
                int index = account[selectedTab].transaction.size() - i - 1; //index is used for displaying the transactions from the last upward so the latest one is at the top
                float transactionYPosition = scrollPanelBounds.y + scrollOffset.y + (i * transactionElementHeight);
                Color backgroundColor = (i % 2 == 0) ? Fade(LIGHTGRAY, 0.5f) : Fade(LIGHTGRAY, 0.3f); //GRAY and LIGHTGRAY look bad together so i used fade which applies transparency
                Color textColor = (account[selectedTab].transaction[index].amount < 0) ? RED : DARKGREEN; //normal green was hard to read on the background

                //draws the checkered (idk if thats the right word) background
                DrawRectangle(scrollPanelBounds.x, transactionYPosition, transactionElementWidth, transactionElementHeight, backgroundColor);

                // draws the note and amount for each transaction
                DrawText(account[selectedTab].transaction[index].note.c_str(), scrollPanelBounds.x + 15, transactionYPosition + 6, 20, DARKGRAY);
                std::string numberText = std::to_string(account[selectedTab].transaction[index].amount);
                DrawText(numberText.c_str(), scrollPanelBounds.x + transactionElementWidth - MeasureText(numberText.c_str(), 20) - 15, transactionYPosition + 6, 20, textColor);

                // Generates the remove button
                Rectangle removeButtonBounds = { scrollPanelBounds.x + transactionElementWidth + 5, transactionYPosition + 5, 70, 20 };
                if (GuiButton(removeButtonBounds, "REMOVE")) {
                    //removes the transaction on the index
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
