#include <stdio.h>

// Игровое поле
#define WIDTH 80
#define HEIGHT 25
// Переменные для мяча, ракеток и счета
int ballX, ballY, ballDirX, ballDirY;
int racket1X, racket1Y, racket2X, racket2Y;
int score1, score2;
int whoIsFirst;

// Приветствие и выбор игрока
void greeting() {
    char symbol;
    printf("Введите номер игрока (1 или 2), который будет подавать первым: ");
    while (scanf("%d%c", &whoIsFirst, &symbol) != 2 || symbol != '\n') {
        while (getchar() != '\n')
            ;  // очистка ввода в случае некорректного ввода
        printf("Некорректный ввод. Введите номер игрока (1 или 2): ");
    }
    if (whoIsFirst == 1) {
        ballDirX = 1;
        ballDirY = 0;
    } else if (whoIsFirst == 2) {
        ballDirX = -1;
        ballDirY = 0;
    } else {
        greeting();
    }
}

// Начальные координаты
void setup() {
    ballX = (WIDTH / 2 - 1);
    ballY = (HEIGHT / 2 - 1);
    racket1X = 0;
    racket1Y = HEIGHT / 2 - 2;
    racket2X = WIDTH - 2;
    racket2Y = HEIGHT / 2 - 2;
}

// Отрисовка игрового поля
void draw() {
    printf("\033[2J");  // Очищаем экран
    printf("\033[H");   // Перемещаем курсор в верхний левый угол
    // Отрисовка верхнего поля
    for (int i = 0; i < WIDTH; i++) printf("-");
    printf("\n");
    // i - столбец, j - строка
    for (int i = 0; i < HEIGHT - 2; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (j == WIDTH / 2 - 1 || j == WIDTH / 2)  // Если мы находимся в середине поля по оси X
                printf("|");                           // Рисуем символ сетки
            else {
                // Твой текущий код оставляем без изменений
                if (j == 0)  // Рисуем символ левого поля
                    printf("|");
                if (i == ballY && j == ballX)  // Рисуем символ мячика
                    printf("о");
                else if (i >= racket1Y && i < racket1Y + 3 && j == racket1X)  // Рисуем символ левой ракетки
                    printf("|");
                else if (i >= racket2Y && i < racket2Y + 3 && j == racket2X)  // Рисуем символ правой ракетки
                    printf("|");
                else if (j == WIDTH - 1)  // Рисуем символ правого поля
                    printf("|");
                else
                    printf(" ");  // Рисуем символ пробела самого поля
            }
        }
        printf("\n");
    }
    // Отрисовка нижнего поля
    for (int i = 0; i < WIDTH; i++) printf("-");
    printf("\n");

    printf("Счет Игрока 1: %d     Счет Игрока 2: %d \n", score1, score2);
}

// Обработка ввода пользователя
void input() {
    char input[10];
    if (fgets(input, sizeof(input), stdin) != NULL) {
        if (input[0] == 'a' && racket1Y > 0) racket1Y--;
        if (input[0] == 'z' && racket1Y < HEIGHT - 3) racket1Y++;
        if (input[0] == 'k' && racket2Y > 0) racket2Y--;
        if (input[0] == 'm' && racket2Y < HEIGHT - 3) racket2Y++;
    }
}

// Обновляет положение мяча, обрабатывает столкновения и подсчитывает очки.
void update() {
    // Эти строки кода обновляют положение мяча, изменяя его координаты на величину, заданную переменными
    // ballDirX и ballDirY.
    ballX += ballDirX;  // Изначальное направление мяча налево
    ballY += ballDirY;

    // Если мяч достигает верхней или нижней границы игрового поля, направление его движения по оси Y
    // инвертируется для отражения от границы.
    if (ballY <= 0 || ballY >= HEIGHT - 3) ballDirY = -ballDirY;
    // Если мяч достигает левой или правой границы игрового поля, соответственно, то к очкам одного из игроков
    // прибавляется 1, и вызывается функция setup(), которая сбрасывает положение мяча и направление его
    // движения.
    if (ballX <= 0) {
        score2++;
        ballDirX = -1;
        ballDirY = 0;
        setup();  // Сброс положения мяча и направления движения
    }

    if (ballX >= WIDTH - 1) {
        score1++;
        ballDirX = 1;
        ballDirY = 0;
        setup();
    }

    // Физика мяча если удар ракеткой 1 (левая) по одному из сегментов

    // // Нижний сегмент
    if ((ballX == 1) && (ballY >= racket1Y) && (ballY < racket1Y + 3)) {
        ballDirX = 1;
        if (ballDirY == -1) {
            ballDirY = 1;
        } else {
            ballDirY = 1;  // Инвертируем направление по оси Y для столкновения с ракеткой 2
        }
    }
    // Центральный сегмент
    if ((ballX == 1) && (ballY >= racket1Y) && (ballY < racket1Y + 2)) {
        ballDirX = 1;
        ballDirY = 0;  // Задаем направление, если 0 - прямо, если 1 - вниз, если -1 - вверх
    }
    // Верхний сегмент
    if ((ballX == 1) && (ballY >= racket1Y) && (ballY < racket1Y + 1)) {
        ballDirX = 1;
        if (ballDirY == 1) {
            ballDirY = 1;
        } else {
            ballDirY = -1;  // Инвертируем направление по оси Y для столкновения с ракеткой 2
        }
    }

    // Физика мяча если удар ракеткой 2 (правая) по одному из сегментов
    // Нижний сегмент
    if (ballX == WIDTH - 3 && ballY >= racket2Y && ballY < racket2Y + 3) {
        ballDirX = -1;
        if (ballDirY == -1) {
            ballDirY = 1;
        } else {
            ballDirY = 1;  // Инвертируем направление по оси Y для столкновения с ракеткой 2
        }
    }
    // Физика мяча если удар ракеткой 2 по одному из сегментов

    // Центральный сегмент
    if (ballX == WIDTH - 3 && ballY >= racket2Y && ballY < racket2Y + 2) {
        ballDirX = -1;
        ballDirY = 0;  // Инвертируем направление по оси Y для столкновения с ракеткой 2
    }

    // Верхний сегмент
    if (ballX == WIDTH - 3 && ballY >= racket2Y && ballY < racket2Y + 1) {
        ballDirX = -1;
        if (ballDirY == -1) {
            ballDirY = 1;
        } else {
            ballDirY = -1;  // Инвертируем направление по оси Y для столкновения с ракеткой 2
        }
    }
}

int main() {
    greeting();
    setup();
    while (1) {
        draw();
        input();
        update();
        if (score1 == 21) {
            printf("\033[2J");  // Очищаем экран
            printf("Игрок 1: %d     Игрок 2: %d", score1, score2);
            printf("\n$$$ Игрок 1 победил! $$$");
            return 0;
        }
        if (score2 == 21) {
            printf("\033[2J");  // Очищаем экран
            printf("Игрок 1: %d     Игрок 2: %d", score1, score2);
            printf("\n$$$ Игрок 2 победил! $$$");
            return 0;
        }
    }
    return 0;
}
