#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "functions.h"
#include "proverki_operacii.h"


struct kmp {
    int re;
    int im;
};

//клетка
struct kle {
    char color;                    //цвет ячейки
    char object;                   //объект в ячейке
};

//игровое поле
struct field {
    int width;                      //координата икс
    int height;                     //координата игрек
    int dino_width;                 //х коор дино
    int dino_height;                //у коор дино
    struct kle obj[100][100];       //игровое поле из клеток
};

struct field move(const struct field Field, char line[][51], int line_number)
{
    struct kmp N = napr(line[1]);  //ищет направление
    struct field res = Field; //мне стрем менять изначальное поле

    int a = ost(res.dino_width + N.re, res.width);   //свой модуль потому что в Си он может быть отр
    int b = ost(res.dino_height + N.im, res.height);

    if (N.re + N.im == 0 || line[2][0] != '\0')  //если направление не определено или третье слово не нулевое - бан
    {
        err(line_number);
    }
    if (res.obj[a][b].object == '%')  //если яма - ггвп
    {
        fprintf(stderr, "Проигрыш. Ты свалился в яму, гений.\n");
        exit(EXIT_FAILURE);
    }
    if (res.obj[a][b].object != '_')  //если не плоскость - ничего не меняем и предупреждаем
    {
        pred(1);
        return res;
    }
    res.obj[res.dino_width][res.dino_height].object = '_';  //удобно на случай если не сдвинулся почему-то
    res.dino_width = a;
    res.dino_height = b;
    res.obj[res.dino_width][res.dino_height].object = '#';
    return res;
}

struct field paint(const struct field Field, char line[][51], int line_number)
{
    struct field res = Field;
    unsigned char c = line[1][0]; //нужен в проверке
    if (line[1][1] != '\0' || line[2][0] != '\0')
    {
        err(line_number);
    }
    if (islower(c) != 0)  //строчная ли буква
    {
        res.obj[res.dino_width][res.dino_height].color = line[1][0];
        return res;
    }
    err(line_number);
    return res;
}

struct field dig(const struct field Field, char line[][51], int line_number)
{
    struct kmp N = napr(line[1]);
    struct field res = Field;
    int a = ost(res.dino_width + N.re, res.width);
    int b = ost(res.dino_height + N.im, res.height);
    if (N.re + N.im == 0 || line[2][0] != '\0')
    {
        err(line_number);
    }
    if (res.obj[a][b].object == '&' || res.obj[a][b].object == '@' || res.obj[a][b].object == '%')   //копать под этим нельзя
    {
        pred(3);
        return res;
    }
    if (res.obj[a][b].object == '^')  //особый случай взаимодействия объектов
    {
        res.obj[a][b].object = '_';
        return res;
    }
    res.obj[a][b].object = '%';
    return res;
}

struct field mound(const struct field Field, char line[][51], int line_number)
{
    struct kmp N = napr(line[1]);
    struct field res = Field;
    int a = ost(res.dino_width + N.re, res.width);
    int b = ost(res.dino_height + N.im, res.height);
    if (N.re + N.im == 0 || line[2][0] != '\0') {
        err(line_number);
    }
    if (res.obj[a][b].object != '%' && res.obj[a][b].object != '_')
    {
        pred(4);
        return res;
    }
    if (res.obj[a][b].object == '%')  //мне надо че то говорить?
    {
        res.obj[a][b].object = '_';
        return res;
    }
    res.obj[a][b].object = '^';
    return res;
}

struct field jump(const struct field Field, char line[][51], int line_number)
{
    struct kmp N = napr(line[1]);
    struct field res = Field;
    int num = get_number(line[2], line_number);

    if (N.re + N.im == 0 || line[3][0] != '\0') {
        err(line_number);
    }

    int current_x = res.dino_width;
    int current_y = res.dino_height;
    int hit = 0;

    for (int step = 1; step <= num; step++) {
        int next_x = ost(current_x + N.re, res.width);
        int next_y = ost(current_y + N.im, res.height);
        if (res.obj[next_x][next_y].object != '_' && res.obj[next_x][next_y].object != '%' && res.obj[next_x][next_y].object != '=' && res.obj[next_x][next_y].object != '#') {
            hit = 1;
            break;
        }
        current_x = next_x;
        current_y = next_y;
    }

    if (hit == 1)
    {
        pred(5);
    }

    if (res.obj[current_x][current_y].object == '%')
    {
        fprintf(stderr, "Проигрыш. Ты свалился в яму, гений.\n");
        exit(EXIT_FAILURE);
    }

    res.obj[res.dino_width][res.dino_height].object = '_';
    res.dino_width = current_x;
    res.dino_height = current_y;
    res.obj[res.dino_width][res.dino_height].object = '#';

    return res;
}

struct field grow(const struct field Field, char line[][51], int line_number)
{
    struct kmp N = napr(line[1]);
    struct field res = Field;
    int a = ost(res.dino_width + N.re, res.width);
    int b = ost(res.dino_height + N.im, res.height);
    if (N.re + N.im == 0 || line[2][0] != '\0')
    {
        err(line_number);
    }
    if (res.obj[a][b].object != '_')
    {
        pred(6);
        return res;
    }
    res.obj[a][b].object = '&';
    return res;
}

struct field cut(const struct field Field, char line[][51], int line_number)
{
    struct kmp N = napr(line[1]);
    struct field res = Field;
    int a = ost(res.dino_width + N.re, res.width);
    int b = ost(res.dino_height + N.im, res.height);
    if (N.re + N.im == 0 || line[2][0] != '\0')
    {
        err(line_number);
    }
    if (res.obj[a][b].object != '&')
    {
        pred(7);
        return res;
    }
    res.obj[a][b].object = '_';
    return res;
}

struct field make(const struct field Field, char line[][51], int line_number)
{
    struct kmp N = napr(line[1]);
    struct field res = Field;
    int a = ost(res.dino_width + N.re, res.width);
    int b = ost(res.dino_height + N.im, res.height);
    if (N.re + N.im == 0 || line[2][0] != '\0')
    {
        err(line_number);
    }
    if (res.obj[a][b].object != '_')
    {
        pred(8);
        return res;
    }
    res.obj[a][b].object = '@';
    return res;
}

struct field push(const struct field Field, char line[][51], int line_number)
{
    struct kmp N = napr(line[1]);
    struct field res = Field;
    int a = ost(res.dino_width + N.re, res.width);
    int b = ost(res.dino_height + N.im, res.height);
    int c = ost(res.dino_width + 2*N.re, res.width);
    int d = ost(res.dino_height + 2*N.im, res.height);
    if (N.re + N.im == 0 || line[2][0] != '\0')
    {
        err(line_number);
    }
    if (res.obj[a][b].object != '@')
    {
        fflush(stdout);
        pred(9);
        return res;
    }
    if (res.obj[c][d].object == '_')
    {
        res.obj[a][b].object = '_';
        res.obj[c][d].object = '@';
        return res;
    }
    if (res.obj[c][d].object == '%')
    {
        res.obj[a][b].object = '_';
        res.obj[c][d].object = '_';
        return res;
    }
    pred(10);
    return res;
}

struct field handle_if(const struct field Field, char line[][51], char subline[50][51], int line_number) //честно, он отказывается со мной сотрудничать если числа больше 10, а еще я проглядел анализ отрицательных
{
    if (strcmp(line[1], "CELL") != 0) err(line_number);
    if (strcmp(line[4], "IS") != 0) err(line_number);
    if (strcmp(line[6], "THEN") != 0) err(line_number);
    if (strlen(line[5]) != 1) err(line_number);

    char target_char = line[5][0];
    int x = get_number(line[2], line_number);
    int y = get_number(line[3], line_number);

    if (x < 0 || x >= Field.width || y < 0 || y >= Field.height)
    {
        fprintf(stderr, "Ошибка: координаты (%d, %d) вне пределов поля в строке %d\n", x, y, line_number);
        exit(EXIT_FAILURE);
    }

    x = ost(x, Field.width);
    y = ost(y, Field.height);

    if (Field.obj[x][y].object == target_char || Field.obj[x][y].color == target_char)
    {
        int after_then = 0;
        for (int i = 7; i < 50; i++)
        {
            strcpy(subline[after_then], line[i]);
            after_then++;
        }
        return komanda(Field, subline, line_number);
    }
    return Field;
}

struct field undo(const struct field Field, char line[][51], int line_number)
{
    struct field res = Field;
    res.width = -100;  //схема имба я считаю
    return res;
}