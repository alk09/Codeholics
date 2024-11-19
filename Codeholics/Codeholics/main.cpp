#include "raylib.h" // Raylib library for graphics and input handling
#include <iostream> // For standard I/O operations
#include <vector>   // For using vectors (dynamic arrays)
#include <string>   // For handling strings
#include <ctime>    // For random seed generation
#include <algorithm> // For std::transform (case conversion)

// Constants for screen size
const int screenWidth = 800; // Width of the window
const int screenHeight = 600; // Height of the window

// Colors for UI
Color LightGray = { 200, 200, 200, 255 }; // Light gray for background
Color DarkGray = { 50, 50, 50, 255 }; // Dark gray for main background
Color Highlight = { 0, 150, 200, 255 }; // Blue highlight for selected options
Color TextColor = WHITE; // Default text color

// Game states for menu navigation
enum GameState { MENU, PLAY, OPTIONS, INSTRUCTIONS, CREDITS, EXIT };
GameState currentState = MENU; // Starting state is the main menu

// Difficulty levels
enum Difficulty { EASY, MEDIUM, HARD };
Difficulty currentDifficulty = EASY; // Default difficulty is easy

// Word lists for each difficulty level
std::vector<std::string> easyWords = { "apple", "book", "cat", "dog", "fish" }; // Easy word list
std::vector<std::string> mediumWords = { "planet", "window", "guitar", "rocket" }; // Medium word list
std::vector<std::string> hardWords = { "algorithm", "philosophy", "encyclopedia" }; // Hard word list

// Gameplay variables
std::string secretWord; // The word to be guessed
std::string guessedWord; // The current state of the guessed word (e.g., "a__le")
std::vector<char> wrongGuesses; // Stores incorrect guesses
int maxAttempts; // Max number of allowed wrong guesses
bool gameWon = false; // Tracks if the player has won
bool gameLost = false; // Tracks if the player has lost

// Function prototypes
void DrawMenu(); // Function to render the main menu
void DrawGameplay(); // Function to render the gameplay screen
void DrawOptions(); // Function to render the options menu
void DrawInstructions(); // Function to render instructions screen
void DrawCredits(); // Function to render credits screen
void UpdateGameplay(); // Function to update gameplay logic
void InitializeGame(); // Function to initialize game variables
void ResetGame(Difficulty difficulty); // Resets the game state based on difficulty
void DrawHangman(int attemptsLeft); // Draws the hangman based on remaining attempts
void HandleWinCondition(); // Handles what happens when the player wins
void HandleLossCondition(); // Handles what happens when the player loses

// Function to reset the game for a new round
void ResetGame(Difficulty difficulty) {
    wrongGuesses.clear(); // Clear previous wrong guesses
    gameWon = false; // Reset win condition
    gameLost = false; // Reset loss condition

    // Set the max attempts based on difficulty
    switch (difficulty) {
    case EASY:
        maxAttempts = 8; // Easier games allow more attempts
        secretWord = easyWords[rand() % easyWords.size()]; // Random word from the easy list
        break;
    case MEDIUM:
        maxAttempts = 6; // Medium games have fewer attempts
        secretWord = mediumWords[rand() % mediumWords.size()]; // Random word from the medium list
        break;
    case HARD:
        maxAttempts = 4; // Hard games have the least attempts
        secretWord = hardWords[rand() % hardWords.size()]; // Random word from the hard list
        break;
    }

    guessedWord = std::string(secretWord.size(), '_'); // Initialize guessedWord with underscores
}

// Function to initialize the game (called once when starting gameplay)
void InitializeGame() {
    ResetGame(currentDifficulty); // Reset the game with the selected difficulty
}

// Function to draw the main menu
void DrawMenu() {
    // Center the text on the screen
    const char* title = "HANGMAN GAME";
    DrawText(title, screenWidth / 2 - MeasureText(title, 40) / 2, 100, 40, TextColor); // Title

    const char* play = "1. Play";
    const char* options = "2. Options";
    const char* instructions = "3. Instructions";
    const char* credits = "4. Credits";
    const char* exit = "5. Exit";

    // Render menu options
    DrawText(play, screenWidth / 2 - MeasureText(play, 20) / 2, 200, 20, Highlight);
    DrawText(options, screenWidth / 2 - MeasureText(options, 20) / 2, 250, 20, TextColor);
    DrawText(instructions, screenWidth / 2 - MeasureText(instructions, 20) / 2, 300, 20, TextColor);
    DrawText(credits, screenWidth / 2 - MeasureText(credits, 20) / 2, 350, 20, TextColor);
    DrawText(exit, screenWidth / 2 - MeasureText(exit, 20) / 2, 400, 20, TextColor);

    // Handle input for menu navigation
    if (IsKeyPressed(KEY_ONE)) currentState = PLAY; // Start gameplay
    if (IsKeyPressed(KEY_TWO)) currentState = OPTIONS; // Go to options
    if (IsKeyPressed(KEY_THREE)) currentState = INSTRUCTIONS; // Show instructions
    if (IsKeyPressed(KEY_FOUR)) currentState = CREDITS; // Show credits
    if (IsKeyPressed(KEY_FIVE)) currentState = EXIT; // Exit game
}

// Function to update the gameplay (called every frame during gameplay)
void UpdateGameplay() {
    if (gameWon || gameLost) return; // If the game is already won or lost, no need to update.

    // Check for key press input (for guessing letters)
    for (char letter = 'A'; letter <= 'Z'; ++letter) {
        if (IsKeyPressed(letter)) {
            char guessedLetter = tolower(letter); // Convert to lowercase for consistency

            // Check if the letter is already guessed
            if (std::find(wrongGuesses.begin(), wrongGuesses.end(), guessedLetter) != wrongGuesses.end() ||
                guessedWord.find(guessedLetter) != std::string::npos) {
                continue; // Skip if the letter is already guessed
            }

            // Check if the guessed letter is in the secret word
            bool correctGuess = false;
            for (size_t i = 0; i < secretWord.size(); ++i) {
                if (secretWord[i] == guessedLetter) {
                    guessedWord[i] = guessedLetter; // Reveal the letter in the guessedWord
                    correctGuess = true;
                }
            }

            if (!correctGuess) {
                wrongGuesses.push_back(guessedLetter); // Add to wrong guesses if incorrect
            }

            // Check for win condition
            if (guessedWord == secretWord) {
                gameWon = true; // Player won
            }

            // Check for loss condition (if the max attempts are exceeded)
            if (wrongGuesses.size() >= maxAttempts) {
                gameLost = true; // Player lost
            }
        }
    }
}

// Function to render the gameplay screen (draws the game state)
void DrawGameplay() {
    // Draw the guessed word (with underscores for unguessed letters)
    DrawText(("Word: " + guessedWord).c_str(), screenWidth / 2 - MeasureText(guessedWord.c_str(), 30) / 2, 150, 30, TextColor);

    // Draw the wrong guesses
    DrawText(("Wrong guesses: " + std::string(wrongGuesses.begin(), wrongGuesses.end())).c_str(),
        screenWidth / 2 - MeasureText("Wrong guesses: ", 20) / 2, 250, 20, TextColor);

    // Draw the hangman (based on the number of wrong attempts)
    DrawHangman(wrongGuesses.size());

    // Check if the player has won or lost and display the appropriate message
    if (gameWon) {
        DrawText("You Win!", screenWidth / 2 - MeasureText("You Win!", 40) / 2, 350, 40, Highlight);
    }
    else if (gameLost) {
        DrawText("You Lose!", screenWidth / 2 - MeasureText("You Lose!", 40) / 2, 350, 40, Highlight);
        DrawText(("The word was: " + secretWord).c_str(), screenWidth / 2 - MeasureText("The word was: ", 20) / 2, 400, 20, TextColor);
    }
}

// Function to draw the hangman based on the number of wrong attempts
void DrawHangman(int attemptsLeft) {
    // Draw hangman body parts depending on how many attempts left
    switch (attemptsLeft) {
    case 0: // No wrong attempts, no hangman drawn
        break;
    case 1: // Head
        DrawCircle(screenWidth / 2, 100, 30, WHITE);
        break;
    case 2: // Body
        DrawLine(screenWidth / 2, 130, screenWidth / 2, 200, WHITE);
        break;
    case 3: // Left arm
        DrawLine(screenWidth / 2, 150, screenWidth / 2 - 50, 170, WHITE);
        break;
    case 4: // Right arm
        DrawLine(screenWidth / 2, 150, screenWidth / 2 + 50, 170, WHITE);
        break;
    case 5: // Left leg
        DrawLine(screenWidth / 2, 200, screenWidth / 2 - 50, 250, WHITE);
        break;
    case 6: // Right leg
        DrawLine(screenWidth / 2, 200, screenWidth / 2 + 50, 250, WHITE);
        break;
    case 7: // Noose (full hangman)
        DrawLine(screenWidth / 2, 100, screenWidth / 2, 50, WHITE); // The rope
        break;
    }
}

// Function to draw the options menu (difficulty selection)
void DrawOptions() {
    const char* optionsTitle = "Options: Difficulty";
    DrawText(optionsTitle, screenWidth / 2 - MeasureText(optionsTitle, 30) / 2, 100, 30, TextColor);

    // Render the difficulty options
    DrawText("1. Easy", screenWidth / 2 - MeasureText("1. Easy", 20) / 2, 200, 20, currentDifficulty == EASY ? Highlight : TextColor);
    DrawText("2. Medium", screenWidth / 2 - MeasureText("2. Medium", 20) / 2, 250, 20, currentDifficulty == MEDIUM ? Highlight : TextColor);
    DrawText("3. Hard", screenWidth / 2 - MeasureText("3. Hard", 20) / 2, 300, 20, currentDifficulty == HARD ? Highlight : TextColor);
    DrawText("4. Back", screenWidth / 2 - MeasureText("4. Back", 20) / 2, 350, 20, TextColor);

    // Handle key presses to change difficulty
    if (IsKeyPressed(KEY_ONE)) {
        currentDifficulty = EASY;
        ResetGame(currentDifficulty); // Reset game with new difficulty
    }
    else if (IsKeyPressed(KEY_TWO)) {
        currentDifficulty = MEDIUM;
        ResetGame(currentDifficulty); // Reset game with new difficulty
    }
    else if (IsKeyPressed(KEY_THREE)) {
        currentDifficulty = HARD;
        ResetGame(currentDifficulty); // Reset game with new difficulty
    }
    else if (IsKeyPressed(KEY_FOUR)) {
        currentState = MENU; // Go back to menu
    }
}

// Function to draw the instructions screen
void DrawInstructions() {
    const char* instructionsText = "Instructions: Guess the word by entering letters.";
    DrawText(instructionsText, screenWidth / 2 - MeasureText(instructionsText, 30) / 2, 100, 20, TextColor);
    DrawText("Press letters A-Z to guess.", screenWidth / 2 - MeasureText("Press letters A-Z to guess.", 20) / 2, 150, 20, TextColor);
    DrawText("You can guess up to 6 wrong letters before losing.", screenWidth / 2 - MeasureText("You can guess up to 6 wrong letters before losing.", 20) / 2, 200, 20, TextColor);
    DrawText("Press 'Back' to return.", screenWidth / 2 - MeasureText("Press 'Back' to return.", 20) / 2, 250, 20, TextColor);

    // Allow user to go back to menu
    if (IsKeyPressed(KEY_BACKSPACE)) {
        currentState = MENU;
    }
}

// Function to draw the credits screen
void DrawCredits() {
    const char* creditsText = "Credits: Hangman Game by YourName.";
    DrawText(creditsText, screenWidth / 2 - MeasureText(creditsText, 30) / 2, 100, 30, TextColor);
    DrawText("Press 'Back' to return.", screenWidth / 2 - MeasureText("Press 'Back' to return.", 20) / 2, 150, 20, TextColor);

    // Allow user to go back to menu
    if (IsKeyPressed(KEY_BACKSPACE)) {
        currentState = MENU;
    }
}

// Function to handle the win condition
void HandleWinCondition() {
    if (gameWon) {
        DrawText("You Win!", screenWidth / 2 - MeasureText("You Win!", 40) / 2, 350, 40, Highlight);
    }
}

// Function to handle the loss condition
void HandleLossCondition() {
    if (gameLost) {
        DrawText("You Lose!", screenWidth / 2 - MeasureText("You Lose!", 40) / 2, 350, 40, Highlight);
        DrawText(("The word was: " + secretWord).c_str(), screenWidth / 2 - MeasureText("The word was: ", 20) / 2, 400, 20, TextColor);
    }
}
// Function to reset the game (called when starting a new game or changing difficulty)
void MainResetGame(Difficulty newDifficulty) {
    gameWon = false; // Reset win condition
    gameLost = false; // Reset loss condition
    wrongGuesses.clear(); // Clear the list of wrong guesses
    guessedWord.clear(); // Clear the guessed word

    // Select a new word based on the chosen difficulty
    guessedWord = std::string(secretWord.size(), '_'); // Initialize guessedWord with underscores

    // Set maximum attempts based on the selected difficulty
    switch (newDifficulty) {
    case EASY:
        maxAttempts = 8; // Easier difficulty has more chances
        break;
    case MEDIUM:
        maxAttempts = 6; // Medium difficulty has a balanced number of attempts
        break;
    case HARD:
        maxAttempts = 4; // Hard difficulty has fewer chances
        break;
    }
}

// Function to select a random word from a predefined list based on difficulty
std::string GetRandomWord(Difficulty difficulty) {
    std::vector<std::string> easyWords = { "apple", "banana", "grape" };
    std::vector<std::string> mediumWords = { "elephant", "guitar", "computer" };
    std::vector<std::string> hardWords = { "astronaut", "inconceivable", "quizzaciously" };

    // Randomly choose a word based on the difficulty
    std::vector<std::string> wordList;
    switch (difficulty) {
    case EASY:
        wordList = easyWords;
        break;
    case MEDIUM:
        wordList = mediumWords;
        break;
    case HARD:
        wordList = hardWords;
        break;
    }

    // Return a random word from the selected list
    int randomIndex = GetRandomValue(0, wordList.size() - 1);
    return wordList[randomIndex];
}

// Main game loop function
void GameLoop() {
    // Initialize Raylib
    InitWindow(screenWidth, screenHeight, "Hangman Game");
    SetTargetFPS(60); // Set frames per second for smoother gameplay

    while (!WindowShouldClose()) {
        // Begin drawing
        BeginDrawing();
        ClearBackground(BLACK);

        // Handle the game states (menu, gameplay, options, etc.)
        switch (currentState) {
        case MENU:
            DrawMenu();
            break;
        case PLAY:
            UpdateGameplay();
            DrawGameplay();
            break;
        case OPTIONS:
            DrawOptions();
            break;
        case INSTRUCTIONS:
            DrawInstructions();
            break;
        case CREDITS:
            DrawCredits();
            break;
        }

        // End drawing
        EndDrawing();
    }

    // Close Raylib window after the game loop ends
    CloseWindow();
}

// Function to draw the main menu (title and options)
void MainDrawMenu() {
    const char* menuTitle = "Hangman Game";
    DrawText(menuTitle, screenWidth / 2 - MeasureText(menuTitle, 30) / 2, 100, 30, TextColor);

    DrawText("1. Start Game", screenWidth / 2 - MeasureText("1. Start Game", 20) / 2, 200, 20, TextColor);
    DrawText("2. Options", screenWidth / 2 - MeasureText("2. Options", 20) / 2, 250, 20, TextColor);
    DrawText("3. Instructions", screenWidth / 2 - MeasureText("3. Instructions", 20) / 2, 300, 20, TextColor);
    DrawText("4. Credits", screenWidth / 2 - MeasureText("4. Credits", 20) / 2, 350, 20, TextColor);

    DrawText("Press 'Esc' to quit", screenWidth / 2 - MeasureText("Press 'Esc' to quit", 20) / 2, 400, 20, TextColor);

    // Menu selection handling
    if (IsKeyPressed(KEY_ONE)) {
        currentState = PLAY; // Start the game
        MainResetGame(currentDifficulty); // Reset the game when starting
    }
    else if (IsKeyPressed(KEY_TWO)) {
        currentState = OPTIONS; // Go to options menu
    }
    else if (IsKeyPressed(KEY_THREE)) {
        currentState = INSTRUCTIONS; // Show instructions
    }
    else if (IsKeyPressed(KEY_FOUR)) {
        currentState = CREDITS; // Show credits
    }
    else if (IsKeyPressed(KEY_ESCAPE)) {
        CloseWindow(); // Quit the game
    }
}

// Function to display the main game logic (setups and transitions)
void DisplayGame() {
    if (currentState == PLAY) {
        UpdateGameplay();
        DrawGameplay();
    }
    else {
        MainDrawMenu();
    }
}

int main() {
    // Initialize game
    GameLoop(); // Start the main game loop
    return 0; // End the game
}

