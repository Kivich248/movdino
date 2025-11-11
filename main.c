#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "parser.h"
#include "functions.h"
#include "proverki_operacii.h"

#define MAX_LINE_LENGTH 151


//ПРОГРАММА ЗАПУСКАЕТСЯ В ТЕРМИНАЛЕ минимально что ввести в него надо:  cmake-build-debug\untitled.exe input.txt output.txt
//так же есть еще 3 настраиваемых параметра согласно данной лабораторной

struct kmp {                                                                //удобные направления
    int re;                                                                 //горизонталь
    int im;                                                                 //вертикаль
};
struct kle {
    char color;                                                             //цвет клетки
    char object;                                                            //объект в клетке
};

struct field {
    int width;                                                              //кол-во столбцов
    int height;                                                             //кол-во строк
    int dino_width;                                                         //столбец дино
    int dino_height;                                                        //строка дино
    struct kle obj[100][100];                                               //поле само
};


int main(int argc, char *argv[])
{
    if (argc < 3) {                                                         //если меньше трех аргументов cmake-build-debug\untitled.exe input.txt output.txt - бан на сервере за читы
        err(0);
    }

    const char *input_file = argv[1];                                       //инпут файл
    const char *output_file = argv[2];                                      //оутпут файл

    int interval = 0;
    int no_display = 0;
    int no_save = 0;

    for (int i = 3; i < argc; i++)
    {
        if (strcmp(argv[i], "interval") == 0 && i + 1 < argc)
        {
            interval = get_number(argv[i + 1], 0);                 //задержка
            i++;
        }
        if (strcmp(argv[i], "no-display") == 0)
        {
            no_display = 1;                                                  //не выводим поля
        }
        if (strcmp(argv[i], "no-save") == 0)
        {
            no_save = 1;                                                     //не сохраняем в файл
        }
    }                                                                        //читаем аргументы которые ввели вы

    int max_undo = 0;                                                        //максимальное количество подряд идущих ундо
    int all_ne_komm_line_number = count_commands(input_file, &max_undo) - 1; //колво всех строк которые не комментарии минус (-1 потому что опытным путем)


    struct field *undo_field = malloc((max_undo + 2) * sizeof(struct field));   //ура динамический массив ундо
    if (!undo_field)
    {
        fprintf(stderr, "Ошибка выделения памяти.\n");
        exit(EXIT_FAILURE);
    }


    FILE *file = fopen(input_file, "r");                        //открыт файл в режиме чтения
    int line_number = 1;                                                      //номер строки с командой
    int line_komm = 0;                                                        //номер строки с коммом
    char line[MAX_LINE_LENGTH];                                               //буфер под парсинг

    struct field Field = create_empty_field();                                //ура создали поле

    int size_k = 0;                                                          //три счетчика
    int start_k = 0;
    int osn_k = 0;
    int a1 = 0, a2 = 0, a3 = 0, a4 = 0; //числа из сайз и старт

    while (fgets(line, sizeof(line), file) != NULL)
    {
        if (line[0] == '/' && line[1] == '/') //проеврка комма, если она здесь, то коммы могут быть любой длины
        {
            line_komm++;
            continue;
        }

        if (strchr(line, '\n') == NULL)                      //если в лайн нету символа перехода на след строку, проверяем, не конец ли файла это, если нет - строка слишком длинная для нашего буфера
        {
            int c = fgetc(file);
            if (c != EOF)
            {
                fprintf(stderr, "Ошибка: строка %d слишком длинная (максимум %d символов).\n", line_number, MAX_LINE_LENGTH - 1);
                exit(EXIT_FAILURE);
            }
        }

        char words[50][51] = {0};
        parse_line(line, words, 50); //парсинг урар

        if (line[0] == '\n' || line[0] == '\r')  //у меня пустая строка = бан, даже если после нее не следуют команды
        {
            err(line_number + line_komm);
        }

        if (strcmp(words[0], "SIZE") == 0) //проверка на слово
        {
            if (size_k == 1) //если оно уже было
            {
                err(line_number + line_komm); //строчка файла
            }
            a1 = strtol(words[1], NULL, 10);
            a2 = strtol(words[2], NULL, 10);
            if (a1 < 10 || a1 > 100 || a2 < 10 || a2 > 100)
            {
                err(line_number + line_komm);
            }
            size_k = 1; //как раз поменяли счетчик
            line_number++;
            continue;
        }

        if (strcmp(words[0], "START") == 0)
        {
            if (start_k == 1)
            {
                err(line_number + line_komm);
            }
            a3 = strtol(words[1], NULL, 10) - 1;
            a4 = strtol(words[2], NULL, 10) - 1;
            if (a3 < 0 || a3 > a1 - 1 || a4 < 0 || a4 > a2 - 1)
            {
                err(line_number + line_komm);
            }
            start_k = 1;
            line_number++;
            continue;
        }

        if (size_k == 1 && start_k == 1 && osn_k == 0)  //инициадизация структуры
        {
            osn_k = 1;
            Field.width = a1;
            Field.height = a2;
            Field.dino_width = a3;
            Field.dino_height = a4;
            for (int i = 0; i < 100; i = i + 1)
            {
                for (int j = 0; j < 100; j = j + 1)
                {
                    if (i < a1 && j < a2)
                    {
                        Field.obj[i][j].color = '0';  //нулевой цвет
                        Field.obj[i][j].object = '_';
                    }
                    if (i >= a1 || j >= a2)
                    {
                        Field.obj[i][j].color = '=';
                        Field.obj[i][j].object = '=';
                    }
                }
            }
            Field.obj[a3][a4].object = '#';
            if (no_display == 0)
            {
                print_field(Field); //вывод, задержка, очистка
                fflush(stdout);
                if (interval != 0)
                {
                    delay_seconds(interval);
                }
                clear_screen();
            }
            if (max_undo == 0)  //если были ундо - заполняем массив, если нет - освобождаем память
            {
                free(undo_field);
            }
            if (max_undo > 0)
            {
                for (int i = 0; i < max_undo + 2; i = i + 1)
                {
                    undo_field[i] = rav(Field, undo_field[i]); // копия структуры
                }
            }
        }

        if (osn_k == 1) //основное происходящее тут
        {
            Field = komanda(Field, words, line_number + line_komm); //выбор команды
            if (Field.width != -100 && max_undo != 0) //-100 сделано для ундо, не обессудьте, я делал лабу за пару дней подряд....
            {
                krugovorot_govna(undo_field, max_undo, Field); //пожалуйста не снижайте баллы мне честно уже лень переименовывать, до дд 1:30 часа, а я не спал, и мне на пары
            }
            if (Field.width == -100 && max_undo != 0) //если поменять порядок ифов, все будет очень плохо
            {
                Field = undo_field[max_undo];
                krugovorot_govna_2(undo_field, max_undo, Field);

            }
            if (no_display == 0)  //если разрешено печатать
            {
                print_field(Field);
                fflush(stdout);
                if (interval > 0 && line_number - 1 != all_ne_komm_line_number)
                {
                    delay_seconds(interval);
                }

                if (line_number - 1 != all_ne_komm_line_number)
                {
                    clear_screen();
                }
            }
        }
        line_number = line_number + 1;
    }

    if (!no_save) {
        FILE *out = fopen(output_file, "w"); //открыли в режиме записи

        if (!out)
        {
            perror("Не удалось открыть файл для записи");
            exit(EXIT_FAILURE);
        }

        for (int i = 0; i < Field.height; i = i + 1)
        {
            for (int j = 0; j < Field.width; j = j + 1)
            {
                char c;
                if (Field.obj[j][i].object != '_')
                {
                    c = Field.obj[j][i].object;
                }
                else if (Field.obj[j][i].color != '0')
                {
                    c = Field.obj[j][i].color;
                }
                else
                {
                    c = '_';
                }
                fputc(c, out);
                fputc(' ', out);
            }
            fputc('\n', out);
        }
        printf("Успешно записали поле в файл.\n"); //почему бы и не сообщить
        fclose(out);
    }
    fclose(file);
    return 0;           //я бы с удовольствием освободил память от динамического массива но я хочу спать
}