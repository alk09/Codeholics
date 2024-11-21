/*******************************************************************
* Wordle-Like Game Using Raylib
* A graphical version of the popular word guessing game "Wordle."
* Enhanced with a main menu, dynamic word selection, scoring, and more.
* Written in C++
********************************************************************/

#include "raylib.h" // Includes the raylib library for graphics and input handling
#include <string>   // Provides the std::string class for text manipulation
#include <vector>   // Provides the std::vector container for dynamic arrays
#include <ctime>    // Enables time-based functions, used for randomization
#include <cstdlib>  // Includes the rand() and srand() functions for random number generation

// Game Constants
const int screenWidth = 800;       // Width of the game window in pixels
const int screenHeight = 600;      // Height of the game window in pixels
const int maxGuesses = 6;          // Maximum number of guesses allowed
const int wordLength = 5;          // Number of letters in the secret word

// Colors
Color correctColor = GREEN;        // Highlight color for correct letters in correct positions
Color presentColor = YELLOW;       // Highlight color for correct letters in wrong positions
Color absentColor = GRAY;          // Highlight color for absent letters

// List of possible words (dynamic word selection)
std::vector<std::string> wordList = { "CRANE", "PLANE", "BRAVE", "GRAPE", "SHINE", "BLEND", "DRIVE", "CHIME" };

// Function to choose a random word from the list
std::string GetRandomWord() {
    srand(time(0)); // Seeds the random number generator with the current time
    return wordList[rand() % wordList.size()]; // Selects a random word from wordList
}

// Helper Function: Convert a character to uppercase
char ToUpper(char c) {
    if (c >= 'a' && c <= 'z') return c - 32; // Converts lowercase letters ('a'-'z') to uppercase
    return c; // Returns the character as-is if it's not lowercase
}

// Helper Function: Compare two strings character by character
std::vector<int> CheckGuess(const std::string& guess, const std::string& word) {
    std::vector<int> result(wordLength, 0); // Initialize feedback array (0 = absent, 1 = present, 2 = correct)
    std::string tempWord = word;            // Copy of the secret word for tracking matched letters

    // First pass: Check for exact matches (correct letters in correct positions)
    for (int i = 0; i < wordLength; i++) {
        if (guess[i] == tempWord[i]) {
            result[i] = 2;     // Mark the position as correct
            tempWord[i] = '*'; // Mark the letter as used
        }
    }

    // Second pass: Check for correct letters in wrong positions
    for (int i = 0; i < wordLength; i++) {
        if (result[i] != 2) { // Skip already matched letters
            for (int j = 0; j < wordLength; j++) {
                if (guess[i] == tempWord[j]) {
                    result[i] = 1;     // Mark as present but misplaced
                    tempWord[j] = '*'; // Mark the letter as used
                    break;
                }
            }
        }
    }
    return result; // Return feedback array
}

// Entry Point: main function
int main() {
    // Initialize the Raylib window
    InitWindow(screenWidth, screenHeight, "Wordle with Menu and Scoring");
    SetTargetFPS(60); // Set the frame rate to 60 frames per second

    int menuOption = 0; // Tracks the currently selected menu option

    while (!WindowShouldClose()) { // Main menu loop runs until the window is closed
        // Navigate the menu
        if (IsKeyPressed(KEY_UP)) menuOption = (menuOption > 0) ? menuOption - 1 : 3; // Move up, wrap to last option
        if (IsKeyPressed(KEY_DOWN)) menuOption = (menuOption < 3) ? menuOption + 1 : 0; // Move down, wrap to first option
        if (IsKeyPressed(KEY_ENTER)) { // Handle menu selection
            if (menuOption == 0) { // PLAY
                RunGame(); // Start the game loop
            }
            else if (menuOption == 1) { // HOW TO PLAY
                while (!WindowShouldClose()) {
                    if (IsKeyPressed(KEY_ESCAPE)) break; // Exit "How to Play" screen to return to the menu
                    BeginDrawing();
                    DisplayHowToPlay(); // Render the "How to Play" screen
                    EndDrawing();
                }
            }
            else if (menuOption == 2) { // ABOUT US
                while (!WindowShouldClose()) {
                    if (IsKeyPressed(KEY_ESCAPE)) break; // Exit "About Us" screen to return to the menu
                    BeginDrawing();
                    DisplayAboutUs(); // Render the "About Us" screen
                    EndDrawing();
                }
            }
            else if (menuOption == 3) { // EXIT
                CloseWindow(); // Close the application window
                return 0; // Exit the program
            }
        }

        // Drawing the main menu
        BeginDrawing();
        DisplayMainMenu(menuOption); // Display the menu with the currently selected option highlighted
        EndDrawing();
    }

    // Cleanup: Close the window before exiting
    CloseWindow();
    return 0; // Exit the program
}

