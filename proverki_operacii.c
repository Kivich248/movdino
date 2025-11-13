#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "functions.h"
#include "proverki_operacii.h"
#include "parser.h"

struct kmp {
    int re;
    int im;
};

//клетка
struct kle {
    char color;         //цвет ячейки
    char object;        //объект в ячейке
};

//игровое поле
struct field {
    int width;          //ширина
    int height;         //высота
    int dino_width;
    int dino_height;
    struct kle obj[100][100];
};

#include <stdio.h>
#include <stdlib.h>

void clear_screen()       //мойдодыр экрана
{
    #if defined(_WIN32) || defined(_WIN64)
        system("cls");
    #else
        system("clear");
    #endif
    fflush(stdout);
}

#if defined(_WIN32) || defined(_WIN64)   //инклудим нужную библу
    #include <windows.h>
#else
    #include <unistd.h>
#endif

void delay_seconds(int seconds)          //задержка, сдава богу не месячных
{
    if (seconds <= 0) return;
    #if defined(_WIN32) || defined(_WIN64)
        Sleep(seconds * 1000);
    #else
        sleep(seconds);
    #endif
}

void err(int a)
{
    fprintf(stderr, "Ошибка. Не объебывайся в написании команд в строке %i, гений.\n", a);  //общий код ошибки, говорящий о строке проеба
    exit(EXIT_FAILURE);
}

void pred(int a)  //много разных предов
{
    switch (a)
    {
    case 1:
        fprintf(stderr, "Прекрати долбиться в препятствия.\n");
        break;
    case 2:
        fprintf(stderr, "Нельзя красить в клетки в буквы кроме a-z.\n");
        break;
    case 3:
        fprintf(stderr, "Ты как подкоп под камнем/деревом/ямой сделать планировал.\n");
        break;
    case 4:
        fprintf(stderr, "Создать гору под чем-то... Нереально.\n");
        break;
    case 5:
        fprintf(stderr, "Из-за тебя динозавр впечатался головой в препятствие.\n");
        break;
    case 6:
        fprintf(stderr, "Растить деревья можно только на пустых клетках.\n");
        break;
    case 7:
        fprintf(stderr, "Я даже спрашивать не хочу как ты собрался рубить что-то кроме дерева.\n");
        break;
    case 8:
        fprintf(stderr, "Создать камень можно только на пустых клетках.\n");
        break;
    case 9:
        fprintf(stderr, "Зачем ты толкаешь что-то кроме камня? Я не разрешал.\n");
        break;
    case 10:
        fprintf(stderr, "Некуда толкать.\n");
        break;
    }
}

int ost(const int a, const int b) //я научился делить!!!!
{
    int r = a % b;
    if (r < 0) r += b;
    return r;
}

struct kmp napr(const char *line)  //считай ущемленные комплексные числа
{
    if (strcmp(line, "LEFT") == 0)
    {
        return (struct kmp){-1, 0};
    }
    if (strcmp(line, "RIGHT") == 0)
    {
        return (struct kmp){1, 0};
    }
    if (strcmp(line, "UP") == 0)
    {
        return (struct kmp){0, -1};
    }
    if (strcmp(line, "DOWN") == 0)
    {
        return (struct kmp){0, 1};
    }
    return (struct kmp){0, 0};
}

struct field komanda(struct field Field, char line[][51], int line_number)  //выбор команды и ее вызов
{
    if (strlen(line[0]) < 2)
    {
        err(line_number);
    }
    if (line[0][0] == '/' && line[0][1] == '/')
    {
        return Field;
    }
    if (strcmp(line[0], "MOVE") == 0)
    {
        struct field f = move(Field, line, line_number);
        return f;
    }
    if (strcmp(line[0], "PAINT") == 0)
    {
        struct field f = paint(Field, line, line_number);
        return f;
    }
    if (strcmp(line[0], "DIG") == 0)
    {
        struct field f = dig(Field, line, line_number);
        return f;
    }
    if (strcmp(line[0], "MOUND") == 0)
    {
        struct field f = mound(Field, line, line_number);
        return f;
    }
    if (strcmp(line[0], "JUMP") == 0)
    {
        struct field f = jump(Field, line, line_number);
        return f;
    }
    if (strcmp(line[0], "GROW") == 0)
    {
        struct field f = grow(Field, line, line_number);
        return f;
    }
    if (strcmp(line[0], "CUT") == 0)
    {
        struct field f = cut(Field, line, line_number);
        return f;
    }
    if (strcmp(line[0], "MAKE") == 0)
    {
        struct field f = make(Field, line, line_number);
        return f;
    }
    if (strcmp(line[0], "PUSH") == 0)
    {
        struct field f = push(Field, line, line_number);
        return f;
    }
    if (strcmp(line[0], "IF") == 0)
    {
        char subline[50][51] = {0};
        struct field f = handle_if(Field, line, subline, line_number);
        return f;
    }
    if (strcmp(line[0], "UNDO") == 0)
    {
        struct field f = undo(Field, line, line_number);
        return f;
    }
    err(line_number);
}

int get_number(const char *line, int line_number) //вроде рабочий перевод из строки в число с учетом того что ты не знаешь число ли это
{

    int i = 0;
    int num = 0;
    if (line[0] == '\0')
    {
        err(line_number);
    }

    while (line[i] != '\0')
    {
        char c = line[i];
        if (isdigit((unsigned char)c) == 0)
        {
            err(line_number);
        }
        num = num * 10 + (c - '0');
        if (num < 0)
        {
            err(line_number);
        }

        i = i + 1;
    }
    return num;
}

void print_field(const struct field Field)  //ура принт
{
    for (int i = 0; i < Field.height; i++)
        {
        for (int j = 0; j < Field.width; j++)
            {
            char c;
            if (Field.obj[j][i].object != '_')
            {
                c = Field.obj[j][i].object;
            }
            if (Field.obj[j][i].color != '0' && Field.obj[j][i].object == '_')
            {
                c = Field.obj[j][i].color;
            }
            if (Field.obj[j][i].color == '0' && Field.obj[j][i].object == '_')
            {
                c = Field.obj[j][i].object;
            }
            putchar(c);
            putchar(' ');
        }
        putchar('\n');
    }
    putchar('\n');
}

int count_commands(const char *filename, int *max_undo_out)  //развлеха для подсчета, оказалось что с указателями "первой степени" достаточно просто и легко работать
{
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Ошибка. Файл не открылся");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    char line[151];
    int count = 0;
    int current_undo_streak = 0;
    int max_undo = 0;

    while (fgets(line, sizeof(line), file))
    {
        if (line[0] == '/' && line[1] == '/')
        {
            if (current_undo_streak > max_undo)
            {
                max_undo = current_undo_streak;
            }
            current_undo_streak = 0;
            continue;
        }

        count++;

        char words[50][51] = {0};
        parse_line(line, words, 50);

        if (words[0][0] != '\0' && strcmp(words[0], "UNDO") == 0)
        {
            current_undo_streak++;
        }
        else
        {
            if (current_undo_streak > max_undo)
            {
                max_undo = current_undo_streak;
            }
            current_undo_streak = 0;
        }
    }

    if (current_undo_streak > max_undo)
    {
        max_undo = current_undo_streak;
    }
    *max_undo_out = max_undo;
    fclose(file);
    return count;
}

void undo_tuda(struct field *undo_field, int max_undo, const struct field Field)  //в массиве ундо филд сдвигаем условно из 2 в 1, из 3 во 2. В size - 1 - структуру из параметров функции
{
    int size = max_undo + 2;
    for (int i = 0; i < size - 1; i = i + 1)
    {
        undo_field[i] = rav(undo_field[i + 1], undo_field[i]);
    }
    undo_field[size - 1] = rav(Field, undo_field[size - 1]);
}

struct field create_empty_field()  //специально плохо заполненный массив чтобы не принять его за наш
{
    struct field empty;
    empty.width = 0;
    empty.height = 0;
    empty.dino_width = -1;
    empty.dino_height = -1;

    for (int i = 0; i < 100; i = i + 1)
    {
        for (int j = 0; j < 100; j = j + 1)
        {
            empty.obj[i][j].color = '=';
            empty.obj[i][j].object = '=';
        }
    }
    return empty;
}

void undo_obr(struct field *undo_field, int max_undo, const struct field Field)  //ундо список идет в обратную сторону из-за функции ундо
{
    int size = max_undo + 2;
    for (int i = size - 2; i > -1; i = i - 1) {
        undo_field[i + 1] = rav(undo_field[i], undo_field[i + 1]);
    }
    undo_field[0] = create_empty_field();
}

struct field rav(const struct field Field,  struct field Field_new) //приравнивание двух структур(можно из параметров было убрать вторую но я тупой
{
    Field_new.width = Field.width;
    Field_new.height = Field.height;
    Field_new.dino_width = Field.dino_width;
    Field_new.dino_height = Field.dino_height;

    for (int i = 0; i < 100; i++) {
        for (int j = 0; j < 100; j++) {
            Field_new.obj[i][j].color = Field.obj[i][j].color;
            Field_new.obj[i][j].object = Field.obj[i][j].object;
        }
    }
    return Field_new;
}
