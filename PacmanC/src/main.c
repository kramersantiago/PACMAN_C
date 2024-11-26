#include <stdio.h>
#include <stdlib.h>
#include <conio.h> // Para _kbhit() y _getch()
#include <windows.h> // Para Sleep()
#include <time.h> // Para generación de números aleatorios

// Dimensiones del tablero
#define WIDTH 30
#define HEIGHT 15

// Caracteres para los elementos del juego
#define PLAYER 'P'
#define ENEMY 'E'
#define FOOD '.'
#define WALL '#'
#define EMPTY ' '

// Tablero del juego
char board[HEIGHT][WIDTH];

// Posición del jugador
int playerX = 1;
int playerY = 1;

// Posiciones de los enemigos
int enemyX[3];
int enemyY[3];

// Dirección del jugador (inicialmente quieto)
char playerDirection = ' ';

// Puntuación
int score = 0;

// Función para dibujar el tablero
void drawBoard() {
    system("cls"); // Limpiar pantalla
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            printf("%c", board[i][j]);
        }
        printf("\n");
    }
    printf("Score: %d\n", score);
}

// Verifica la distancia mínima entre dos puntos
int isFarEnough(int x1, int y1, int x2, int y2, int minDistance) {
    int dx = x1 - x2;
    int dy = y1 - y2;
    return (dx * dx + dy * dy >= minDistance * minDistance); // Distancia euclidiana al cuadrado
}

// Inicializa el tablero con paredes, comida y posiciones iniciales
void initializeBoard() {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (i == 0 || i == HEIGHT - 1 || j == 0 || j == WIDTH - 1) {
                board[i][j] = WALL; // Bordes
            } else if ((i % 3 == 0 && j % 5 != 0) || (j % 4 == 0 && i % 2 != 0)) {
                board[i][j] = WALL; // Paredes internas abiertas
            } else {
                board[i][j] = FOOD; // Espacios con comida
            }
        }
    }

    // Colocar al jugador
    board[playerY][playerX] = PLAYER;

    // Generar posiciones de enemigos
    for (int i = 0; i < 3; i++) {
        do {
            enemyX[i] = rand() % (WIDTH - 2) + 1; // Dentro de los límites del tablero
            enemyY[i] = rand() % (HEIGHT - 2) + 1;
        } while (board[enemyY[i]][enemyX[i]] == WALL || 
                 !isFarEnough(playerX, playerY, enemyX[i], enemyY[i], 5));
        board[enemyY[i]][enemyX[i]] = ENEMY; // Colocar al enemigo
    }
}

// Mueve un enemigo en una dirección aleatoria
void moveEnemy(int index) {
    int direction = rand() % 4; // Genera un número aleatorio entre 0 y 3
    int newX = enemyX[index];
    int newY = enemyY[index];

    switch (direction) {
        case 0: newY--; break; // Arriba
        case 1: newY++; break; // Abajo
        case 2: newX--; break; // Izquierda
        case 3: newX++; break; // Derecha
    }

    // Verifica que el movimiento sea válido
    if (board[newY][newX] != WALL) {
        board[enemyY[index]][enemyX[index]] = EMPTY; // Limpia la posición anterior
        enemyX[index] = newX;
        enemyY[index] = newY;
        board[enemyY[index]][enemyX[index]] = ENEMY; // Actualiza la nueva posición
    }
}

// Maneja la entrada del jugador y su dirección
void setPlayerDirection(char input) {
    if (input == 'w' || input == 's' || input == 'a' || input == 'd') {
        playerDirection = input;
    }
}

// Mueve al jugador en la dirección actual
void movePlayer() {
    int newX = playerX;
    int newY = playerY;

    switch (playerDirection) {
        case 'w': newY--; break; // Arriba
        case 's': newY++; break; // Abajo
        case 'a': newX--; break; // Izquierda
        case 'd': newX++; break; // Derecha
    }

    // Verifica que el movimiento sea válido
    if (board[newY][newX] != WALL) {
        board[playerY][playerX] = EMPTY; // Limpia la posición anterior
        playerX = newX;
        playerY = newY;

        if (board[playerY][playerX] == FOOD) {
            score++;
        }
        board[playerY][playerX] = PLAYER;
    }
}

// Comprueba si el jugador ha perdido
int checkGameOver() {
    for (int i = 0; i < 3; i++) {
        if (playerX == enemyX[i] && playerY == enemyY[i]) {
            return 1; // El jugador colisionó con un enemigo
        }
    }
    return 0;
}

// Juego principal
int main() {
    char input;
    srand(time(NULL)); // Inicializa el generador de números aleatorios
    initializeBoard();

    while (1) {
        drawBoard();
        if (_kbhit()) { // Detecta si hay entrada del teclado
            input = _getch();
            setPlayerDirection(input);
        }
        movePlayer();
        for (int i = 0; i < 3; i++) {
            moveEnemy(i);
        }
        if (checkGameOver()) {
            printf("Game Over! Final Score: %d\n", score);
            break;
        }
        Sleep(200); // Controla la velocidad del juego
    }
    return 0;
}
