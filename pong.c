#include <ncurses.h>  // Library for handling text-based user interfaces
#include <stdlib.h>
#include <unistd.h>  // Library for POSIX operating system API

#define WIDTH 80   // Width of the game field
#define HEIGHT 25  // Height of the game field

// Global variables to store game state and scores
int ballX, ballY, directionX, directionY;
int racket1Y, racket2Y, score1, score2;
int gameOver;

// Initialize game state and scores
void setup() {
    ballX = WIDTH / 2;
    ballY = HEIGHT / 2;
    racket1Y = HEIGHT / 2;
    racket2Y = HEIGHT / 2;
    score1 = 0;
    score2 = 0;
    gameOver = 0;
    directionX = (rand() % 2 == 0) ? -1 : 1;  // Randomly choose -1 or 1 for horizontal direction
    directionY = (rand() % 2 == 0) ? -1 : 1;  // Randomly choose -1 or 1 for vertical direction
}

// Draw the boundaries of the game field
void drawBoundaries() {
    for (int i = 1; i < HEIGHT - 2; i++) {
        mvprintw(i, 0, "|");          // Left boundary at column 0
        mvprintw(i, WIDTH - 1, "|");  // Right boundary at the last column
        mvprintw(i, WIDTH / 2, ":");  // Vertical line at the middle
    }
    for (int i = 0; i < WIDTH; i++) {
        mvprintw(0, i, "-");           // Top boundary at row 0
        mvprintw(HEIGHT - 2, i, "-");  // Bottom boundary at HEIGHT - 2
    }

    mvprintw(HEIGHT - 2, WIDTH / 2 - 3, "21Trew");  // Display "School 21" at the bottom middle
}

// Draw the objects such as the ball, rackets, and the score
void drawObjects() {
    mvprintw(ballY, ballX, "o");
    for (int i = -1; i <= 1; i++) {
        mvprintw(racket1Y + i, 2, "#");          // Draw racket 1 on the left
        mvprintw(racket2Y + i, WIDTH - 3, "#");  // Draw racket 2 on the right
    }
    mvprintw(0, WIDTH / 2 - 5 , "Score: %d - %d", score1, score2);  // Display score in the middle
}

// Clear the screen, draw the boundaries and objects, and refresh the display
void draw() {
    clear();
    drawBoundaries();
    drawObjects();
    refresh();
}

// Update the game state based on user input and ball movement
void handleRacketMovement(char key, int *racketY) {
    if (key == 'a' && *racketY > 1) {
        (*racketY)--;
    } else if (key == 'z' && *racketY < HEIGHT - 3) {
        (*racketY)++;
    }
}

// Handle user input for both players
void input() {
    char key = getch();
    handleRacketMovement(key, &racket1Y);  // Player 1 controls with 'a' and 'z'

    if (key == 'k' && racket2Y > 1) {
        racket2Y--;
    } else if (key == 'm' && racket2Y < HEIGHT - 3) {
        racket2Y++;
    }
}

// Update the game state based on ball movement and collisions
void update() {
    ballX += directionX;
    ballY += directionY;

    if ((ballX == 3 && ballY >= racket1Y - 1 && ballY <= racket1Y + 1) ||
        (ballX == WIDTH - 4 && ballY >= racket2Y - 1 && ballY <= racket2Y + 1)) {
        directionX = -directionX;
    }

    if (ballY == 1 || ballY == HEIGHT - 2) {
        directionY = -directionY;
    }

    if (ballX == 0) {
        score2++;
        ballX = WIDTH / 2;
        ballY = HEIGHT / 2;
    } else if (ballX == WIDTH - 1) {
        score1++;
        ballX = WIDTH / 2;
        ballY = HEIGHT / 2;
    }

    if (score1 == 21 || score2 == 21) {
        gameOver = 1;
    }
}

// Main function to run the game loop
int main() {
    initscr();             // Initialize the ncurses library
    noecho();              // Don't echo user input to the screen
    curs_set(FALSE);       // Hide the cursor
    keypad(stdscr, TRUE);  // Enable the keypad for user input

    setup();  // Initialize game state and scores

    while (!gameOver) {  // Main game loop
        draw();          // Draw the game field and objects
        input();         // Handle user input
        update();        // Update the game state
        usleep(100000);  // Delay for smooth animation
    }

    mvprintw(HEIGHT / 2, WIDTH / 2 - 10, "Game Over! Winner: Player %d\n",
             (score1 == 21) ? 1 : 2);  // Display game over message
    refresh();

    getch();   // Wait for user input
    endwin();  // End the ncurses session
    return 0;
}
