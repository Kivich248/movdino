void parse_line(const char line[], char words[][51], int max_words)
{
    int word_count = 0;
    int word_len = 0;
    int in_word = 0;

    for (int i = 0; line[i] != '\0' && line[i] != '\n'; i++)
    {
        char c = line[i];

        if (c == ' ' || c == '\t')           //если пробел/табуляция, то если были в слове, закончим строку в массиве вордс, увеличии счетчик строк, длину на ноль, статус в слове на ноль. Если слишком много слов - бан
        {
            if (in_word == 1)
            {
                words[word_count][word_len] = '\0';
                word_count++;
                word_len = 0;
                in_word = 0;
                if (word_count >= max_words)
                {
                    break;
                }
            }
        }
        else  //елсе тут значит если символ НЕ пробел/табуляция, то если не в слове, статус в слове на 1, длину слова на ноль. Если длина слова меньше 50 символов, записываем символ в строку вордс, увеличиваем длину слова(счетчик столбцов)
        {
            if (in_word == 0)
            {
                in_word = 1;
                word_len = 0;
            }
            if (word_len < 50)
            {
                words[word_count][word_len] = c;
                word_len++;
            }
        }
    }

    if (in_word == 1 && word_count < max_words) {  //если мы до сих пор в слове после выхода из цикла - выходим
        words[word_count][word_len] = '\0';
    }
}