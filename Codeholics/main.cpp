#include "raylib.h"  // Raylib library for graphics and input handling
#include <string>    // For using std::string to handle text
#include <vector>    // To use std::vector for dynamic arrays
#include <ctime>     // For random number generation based on the system time
#include <cstdlib>   // For srand() and rand() for randomization

// Game Constants
const int SCREEN_WIDTH = 800;   // Width of the game window in pixels
const int SCREEN_HEIGHT = 600;  // Height of the game window in pixels
const int MAX_GUESSES = 6;      // Maximum number of guesses allowed
const int WORD_LENGTH = 5;      // Length of the secret word

// Colors for feedback
Color CORRECT_COLOR = GREEN;   // Color for correct letters in the correct positions
Color PRESENT_COLOR = YELLOW;  // Color for correct letters in wrong positions
Color ABSENT_COLOR = GRAY;     // Color for absent letters

// List of potential words for the secret word
std::vector<std::string> wordList = { "CRANE", "PLANE", "BRAVE", "GRAPE", "SHINE", "BLEND", "DRIVE", "CHIME" };

// Function to pick a random word from the list
std::string GetRandomWord() {
    srand(static_cast<unsigned int>(time(0)));  // Seed random number generator with the current time
    return wordList[rand() % wordList.size()];  // Select and return a random word
}

// Convert a character to uppercase
char ToUpper(char c) {
    if (c >= 'a' && c <= 'z') return c - 32;  // Convert lowercase letters to uppercase
    return c;  // Return the character as it is if it's not a lowercase letter
}

// Function to compare a guess with the answer and return feedback
std::vector<int> CheckGuess(const std::string& guess, const std::string& word) {
    std::vector<int> result(WORD_LENGTH, 0);  // Initialize the result vector with zeros (0 = absent, 1 = present, 2 = correct)
    std::string tempWord = word;  // Make a copy of the word to mark used letters

    // First pass: Check for correct letters in the correct positions
    for (int i = 0; i < WORD_LENGTH; i++) {
        if (guess[i] == tempWord[i]) {
            result[i] = 2;         // Mark as correct
            tempWord[i] = '*';     // Mark the letter as used in the word
        }
    }

    // Second pass: Check for correct letters in the wrong positions
    for (int i = 0; i < WORD_LENGTH; i++) {
        if (result[i] != 2) {  // Skip already matched letters
            for (int j = 0; j < WORD_LENGTH; j++) {
                if (guess[i] == tempWord[j]) {
                    result[i] = 1;  // Mark as present but misplaced
                    tempWord[j] = '*';  // Mark this letter as used
                    break;
                }
            }
        }
    }

    return result;  // Return the feedback array
}

// Display the main menu of the game
void DisplayMainMenu(int selectedOption) {
    const char* menuOptions[] = { "PLAY", "HOW TO PLAY", "ABOUT US", "EXIT" };  // Menu options
    int totalOptions = 4;  // Total number of menu options

    ClearBackground(RAYWHITE);  // Clear screen and set background to white
    DrawText("WORDLE GAME MENU", SCREEN_WIDTH / 2 - MeasureText("WORDLE GAME MENU", 30) / 2, 50, 30, BLACK);  // Draw title

    // Draw each menu option with highlighting for the selected one
    for (int i = 0; i < totalOptions; i++) {
        Color textColor = (i == selectedOption) ? GREEN : DARKGRAY;  // Highlight selected option
        DrawText(menuOptions[i], SCREEN_WIDTH / 2 - MeasureText(menuOptions[i], 20) / 2, 150 + i * 50, 20, textColor);  // Draw the option
    }
}

// Display instructions for how to play the game
void DisplayHowToPlay() {
    ClearBackground(RAYWHITE);  // Clear screen and set background to white
    DrawText("HOW TO PLAY", SCREEN_WIDTH / 2 - MeasureText("HOW TO PLAY", 30) / 2, 50, 30, BLACK);  // Title
    // Instructions for the player
    DrawText("1. Guess the secret 5-letter word in 6 attempts.", 50, 150, 20, DARKGRAY);
    DrawText("2. Each guess must be a valid word.", 50, 180, 20, DARKGRAY);
    DrawText("3. Green light is a correct letter", 50, 210, 20, DARKGRAY);
    DrawText("4. Yellow light is a correct letter, but not in the correct place", 50, 210, 20, DARKGRAY);
    DrawText("Press ESC to return to the menu.", 50, 270, 20, DARKGRAY);  // Navigation hint
}

// Display information about the creators of the game
void DisplayAboutUs() {
    ClearBackground(RAYWHITE);  // Clear screen and set background to white
    DrawText("ABOUT US", SCREEN_WIDTH / 2 - MeasureText("ABOUT US", 30) / 2, 50, 30, BLACK);  // Title
    // Description of the game and creator
    DrawText("This Wordle-like game was created using raylib.", 50, 150, 20, DARKGRAY);
    DrawText("We are still beginners, but we hope you like our mini game ", 50, 180, 20, DARKGRAY);
    DrawText("Creators of the game: ", 50, 210, 20, DARKGRAY);
    DrawText("1. Aleksandar Georgiev (Scrum Trainer)", 50, 240, 20, DARKGRAY);
    DrawText("2. Mila Lazarova (Designer)", 50, 270, 20, DARKGRAY);
    DrawText("3. Borimir Kirov (Back-End Developer)", 50, 300, 20, DARKGRAY);
    DrawText("4. Dimitar Dimitrov (Back-End Developer)", 50, 330, 20, DARKGRAY);
    DrawText("Press ESC to return to the menu.", 50, 360, 20, DARKGRAY);
}

// Function to handle the gameplay logic
void RunGame() {
    std::string answer = GetRandomWord();  // Select a random word as the answer
    std::vector<std::string> guesses;  // Store player guesses
    std::vector<std::vector<int>> feedbacks;  // Store feedback for each guess
    std::string currentGuess = "";  // The current guess being typed by the player
    int score = 0;  // Player's score
    bool gameOver = false;  // Flag indicating if the game is over
    bool gameWon = false;  // Flag indicating if the player has won

    while (!WindowShouldClose()) {  // Game loop runs until the window is closed
        if (IsKeyPressed(KEY_ESCAPE)) break;  // Exit the game to the menu if ESC is pressed

        if (!gameOver) {
            // Handle user input
            if (IsKeyPressed(KEY_BACKSPACE) && !currentGuess.empty()) {
                currentGuess.pop_back();  // Remove the last character if BACKSPACE is pressed
            }
            else if (IsKeyPressed(KEY_ENTER) && currentGuess.length() == WORD_LENGTH) {
                // Submit the guess if ENTER is pressed and the guess is valid
                std::vector<int> feedback = CheckGuess(currentGuess, answer);  // Get feedback for the guess
                feedbacks.push_back(feedback);  // Store feedback
                guesses.push_back(currentGuess);  // Store the guess

                // Check if the guess is correct
                if (currentGuess == answer) {
                    gameOver = true;  // End the game
                    gameWon = true;  // Player wins
                    score += 100;  // Add points for winning
                }
                else if (guesses.size() >= MAX_GUESSES) {
                    gameOver = true;  // End the game if the maximum guesses are reached
                }
                currentGuess = "";  // Reset current guess
            }
            else {
                // Add letters to the current guess
                for (int key = KEY_A; key <= KEY_Z; key++) {
                    if (IsKeyPressed(key) && currentGuess.length() < WORD_LENGTH) {
                        currentGuess += ToUpper((char)(key - KEY_A + 'A'));  // Convert and add the letter
                    }
                }
            }
        }
        else {
            // Restart the game if it's over and 'R' is pressed
            if (IsKeyPressed(KEY_R)) {
                answer = GetRandomWord();  // Select a new word
                guesses.clear();  // Clear guesses
                feedbacks.clear();  // Clear feedbacks
                currentGuess = "";  // Reset current guess
                gameOver = false;  // Reset game over flag
                gameWon = false;  // Reset game won flag
            }
        }

        // Drawing the game
        BeginDrawing();
        ClearBackground(RAYWHITE);  // Set background color

        // Draw the title of the game
        DrawText("WORDLE", SCREEN_WIDTH / 2 - MeasureText("WORDLE", 40) / 2, 20, 40, BLACK);

        // Draw the message based on the game state
        if (!gameOver) {
            DrawText("Type a 5-letter word and press ENTER to guess.", 20, 80, 20, DARKGRAY);
        }
        else {
            DrawText(gameWon ? "YOU WIN! Press R to restart." : "GAME OVER! Press R to restart.", 20, 80, 20, gameWon ? GREEN : RED);
            DrawText(TextFormat("The word was: %s", answer.c_str()), 20, 110, 20, DARKGRAY); // Show the correct word
            DrawText(TextFormat("Score: %d", score), SCREEN_WIDTH - MeasureText(TextFormat("Score: %d", score), 20) - 10, 10, 20, BLUE); // Display score
        }

        // Draw each guess with feedback colors
        for (int i = 0; i < guesses.size(); i++) {
            for (int j = 0; j < WORD_LENGTH; j++) {
                int feedback = feedbacks[i][j];  // Feedback for the current letter
                Color cellColor = (feedback == 2) ? CORRECT_COLOR : (feedback == 1) ? PRESENT_COLOR : ABSENT_COLOR; // Determine color

                // Draw a rectangle for each letter, colored based on feedback
                DrawRectangle(50 + j * 60, 150 + i * 60, 50, 50, cellColor);
                // Draw the letter inside the rectangle
                DrawText(std::string(1, guesses[i][j]).c_str(), 65 + j * 60, 165 + i * 60, 20, WHITE);
            }
        }

        // Draw the current guess being typed by the player
        for (int j = 0; j < currentGuess.size(); j++) {
            DrawRectangle(50 + j * 60, 150 + guesses.size() * 60, 50, 50, LIGHTGRAY);  // Draw light gray box
            DrawText(std::string(1, currentGuess[j]).c_str(), 65 + j * 60, 165 + guesses.size() * 60, 20, BLACK); // Draw the letter
        }

        EndDrawing();  // End drawing frame
    }
}

// Main function
int main() {
    // Initialize the Raylib window
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Wordle with Menu and Scoring");
    SetTargetFPS(60);  // Set frame rate to 60 FPS

    int menuOption = 0;  // Variable to keep track of the currently selected menu option

    while (!WindowShouldClose()) {  // Main menu loop
        // Handle menu navigation
        if (IsKeyPressed(KEY_UP)) menuOption = (menuOption > 0) ? menuOption - 1 : 3; // Move up, wrapping to last option
        if (IsKeyPressed(KEY_DOWN)) menuOption = (menuOption < 3) ? menuOption + 1 : 0; // Move down, wrapping to first option
        if (IsKeyPressed(KEY_ENTER)) {  // Handle menu option selection
            if (menuOption == 0) {  // PLAY option
                RunGame();  // Start the game
            }
            else if (menuOption == 1) {  // HOW TO PLAY option
                while (!WindowShouldClose()) {
                    if (IsKeyPressed(KEY_ESCAPE)) break;  // Return to menu if ESC is pressed
                    BeginDrawing();
                    DisplayHowToPlay();  // Show "How to Play" instructions
                    EndDrawing();
                }
            }
            else if (menuOption == 2) {  // ABOUT US option
                while (!WindowShouldClose()) {
                    if (IsKeyPressed(KEY_ESCAPE)) break;  // Return to menu if ESC is pressed
                    BeginDrawing();
                    DisplayAboutUs();  // Show "About Us" information
                    EndDrawing();
                }
            }
            else if (menuOption == 3) {  // EXIT option
                return 0;  // Exit the program
            }
        }

        // Draw the main menu screen
        BeginDrawing();
        DisplayMainMenu(menuOption);  // Show the menu with the selected option highlighted
        EndDrawing();
    }

    // Cleanup: Close the Raylib window
    return 0;  // Exit the program
}

