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

// Function to display the main menu
void DisplayMainMenu(int selectedOption) {
    const char* menuOptions[] = { "PLAY", "HOW TO PLAY", "ABOUT US", "EXIT" }; // Menu options
    int totalOptions = 4; // Total number of menu items

    ClearBackground(RAYWHITE); // Set the background color to white
    DrawText("WORDLE GAME MENU", screenWidth / 2 - MeasureText("WORDLE GAME MENU", 30) / 2, 50, 30, BLACK); // Title

    // Loop to draw each menu option
    for (int i = 0; i < totalOptions; i++) {
        Color textColor = (i == selectedOption) ? GREEN : DARKGRAY; // Highlight the selected option
        DrawText(menuOptions[i], screenWidth / 2 - MeasureText(menuOptions[i], 20) / 2, 150 + i * 50, 20, textColor); // Draw option
    }
}

// Function to display "How to Play"
void DisplayHowToPlay() {
    ClearBackground(RAYWHITE); // Set the background color to white
    DrawText("HOW TO PLAY", screenWidth / 2 - MeasureText("HOW TO PLAY", 30) / 2, 50, 30, BLACK); // Title
    // Instructions
    DrawText("1. Guess the secret 5-letter word in 6 attempts.", 50, 150, 20, DARKGRAY);
    DrawText("2. Each guess must be a valid word.", 50, 180, 20, DARKGRAY);
    DrawText("3. Feedback will indicate correct, misplaced, or absent letters.", 50, 210, 20, DARKGRAY);
    DrawText("Press ESC to return to the menu.", 50, 270, 20, DARKGRAY); // Back navigation hint
}

// Function to display "About Us"
void DisplayAboutUs() {
    ClearBackground(RAYWHITE); // Set the background color to white
    DrawText("ABOUT US", screenWidth / 2 - MeasureText("ABOUT US", 30) / 2, 50, 30, BLACK); // Title
    // Description
    DrawText("This Wordle-like game was created using raylib.", 50, 150, 20, DARKGRAY);
    DrawText("Author: Your Name", 50, 180, 20, DARKGRAY);
    DrawText("Year: 2024", 50, 210, 20, DARKGRAY);
    DrawText("Press ESC to return to the menu.", 50, 270, 20, DARKGRAY); // Back navigation hint
}

// Function to run the game loop
void RunGame() {
    std::string answer = GetRandomWord();         // Secret word chosen randomly
    std::vector<std::string> guesses;           // List of guesses entered by the player
    std::vector<std::vector<int>> feedbacks;    // Feedback for each guess
    std::string currentGuess = "";              // Current guess being typed by the player
    int score = 0;                              // Player's score
    bool gameOver = false;                      // Flag indicating if the game is over
    bool gameWon = false;                       // Flag indicating if the player has won

    while (!WindowShouldClose()) { // Game loop runs until the window is closed
        if (IsKeyPressed(KEY_ESCAPE)) break; // Exit the game to the menu if ESC is pressed

        if (!gameOver) {
            // Input handling
            if (IsKeyPressed(KEY_BACKSPACE) && !currentGuess.empty()) {
                currentGuess.pop_back(); // Remove the last character from the guess
            }
            else if (IsKeyPressed(KEY_ENTER) && currentGuess.length() == wordLength) {
                // Submit the guess
                std::vector<int> feedback = CheckGuess(currentGuess, answer); // Get feedback
                feedbacks.push_back(feedback); // Store feedback
                guesses.push_back(currentGuess); // Store the guess

                if (currentGuess == answer) {
                    gameOver = true; // Set game over flag
                    gameWon = true; // Set game won flag
                    score += 100;   // Add points for winning
                }
                else if (guesses.size() >= maxGuesses) {
                    gameOver = true; // End game if maximum guesses are reached
                }
                currentGuess = ""; // Reset current guess
            }
            else {
                // Add letters to the current guess
                for (int key = KEY_A; key <= KEY_Z; key++) {
                    if (IsKeyPressed(key) && currentGuess.length() < wordLength) {
                        currentGuess += ToUpper((char)(key - KEY_A + 'A')); // Convert and add the letter
                    }
                }
            }
        }
        else {
            // Restart the game if game is over
            if (IsKeyPressed(KEY_R)) {
                answer = GetRandomWord(); // Select a new word
                guesses.clear();          // Clear guesses
                feedbacks.clear();        // Clear feedbacks
                currentGuess = "";        // Reset current guess
                gameOver = false;         // Reset game over flag
                gameWon = false;          // Reset game won flag
            }
        }

        // Drawing
        BeginDrawing(); // Begin the rendering process
        ClearBackground(RAYWHITE); // Set background color

        // Title
        DrawText("WORDLE", screenWidth / 2 - MeasureText("WORDLE", 40) / 2, 20, 40, BLACK);

        // Instructions or game over message
        if (!gameOver) {
            DrawText("Type a 5-letter word and press ENTER to guess.", 20, 80, 20, DARKGRAY);
        }
        else {
            DrawText(gameWon ? "YOU WIN! Press R to restart." : "GAME OVER! Press R to restart.", 20, 80, 20, gameWon ? GREEN : RED);
            DrawText(TextFormat("The word was: %s", answer.c_str()), 20, 110, 20, DARKGRAY); // Display the correct word after the game ends
            DrawText(TextFormat("Score: %d", score), screenWidth - MeasureText(TextFormat("Score: %d", score), 20) - 10, 10, 20, BLUE); //draw player's score
        }

        // Draw guesses and feedback
        for (int i = 0; i < guesses.size(); i++) {
            for (int j = 0; j < wordLength; j++) {
                int feedback = feedbacks[i][j]; // Feedback for the current letter
                Color cellColor = (feedback == 2) ? correctColor : (feedback == 1) ? presentColor : absentColor; // Determine color

                // Draw a rectangle for each letter, colored based on feedback
                DrawRectangle(50 + j * 60, 150 + i * 60, 50, 50, cellColor);
                // Draw the letter inside the rectangle
                DrawText(std::string(1, guesses[i][j]).c_str(), 65 + j * 60, 165 + i * 60, 20, WHITE);
            }
        }

        // Draw the current guess being typed
        for (int j = 0; j < currentGuess.size(); j++) {
            DrawRectangle(50 + j * 60, 150 + guesses.size() * 60, 50, 50, LIGHTGRAY); // Draw light gray box
            DrawText(std::string(1, currentGuess[j]).c_str(), 65 + j * 60, 165 + guesses.size() * 60, 20, BLACK); // Draw the letter
        }

        EndDrawing(); // End the rendering process
    }
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

