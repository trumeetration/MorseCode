#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <locale.h>
#include <string.h>
#include <malloc.h>

#define ENTER 13

struct glossary
{
    char mrz_to_rus[45];
    char *mrz_from_rus[45];
    char mrz_to_en[38];
    char *mrz_from_en[38];
};

struct glossary gloss = {{'а', 'б', 'в', 'г', 'д', 'е', 'ё', 'ж', 'з', 'и', 'й', 'к', 'л', 'м', 'н', 'о', 'п', 'р', 'с', 'т', 'у', 'ф', 'х', 'ц', 'ч', 'ш', 'щ', 'ъ', 'ы', 'ь', 'э', 'ю', 'я', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '.', ' '},
                         {".-", "-...", ".--", "--.", "-..", ".", ".", "...-", "--..", "..", ".---", "-.-", ".-..", "--", "-.", "---", ".--.", ".-.", "...", "-", "..-", "..-.", "....", "-.-.", "---.", "----", "--.-", "--.--", "-.--", "-..-", "..-..", "..--", ".-.-", ".----", "..---", "...--", "....-", ".....", "-....", "--...", "---..", "----.", "-----", "......", "------"},
                         {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '.', ' '},
                         {".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..", ".---", "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.", "...", "-", "..-", "...-", ".--", "-..-", "-.--", "--..", ".----", "..---", "...--", "....-", ".....", "-....", "--...", "---..", "----.", "-----", "------", "......"}};

char decode(char *mrzcombo, char lang);
char *encode(char let, char lang);
void StartEncoding(char *input, char lang);
void StartDecoding(char *input, char lang);
void Start(char mode, char inputmode, char lang);
void SaveData(char *data);

void SaveData(char *data)
{
    FILE *file;
    char path[100];
    printf("\nИмя файла: ");
    fflush(stdin);
    scanf("%s", &path);
    if ((file = fopen(path, "w")) == NULL)
    {
        printf("\n\tНеверное имя файла");
        return;
    }
    fprintf(file, "%s", data);
    fclose(file);
    printf("\n\tДанные сохранены.");
}

char decode(char *mrzcombo, char lang)
{
    int i;
    char *letter = lang == '1' ? gloss.mrz_to_rus : gloss.mrz_to_en;
    char **mrzvalue = lang == '1' ? gloss.mrz_from_rus : gloss.mrz_from_en;
    int length = lang == '1' ? 45 : 38;
    for (i = 0; i < length; i++)
    {
        if (strcmp(mrzcombo, mrzvalue[i]) == 0)
        {
            return letter[i];
        }
    }
    return 0;
}

char *encode(char let, char lang)
{
    int i;
    int length = lang == '1' ? 45 : 38;
    char **mrz = lang == '1' ? gloss.mrz_from_rus : gloss.mrz_from_en;
    char *value = lang == '1' ? gloss.mrz_to_rus : gloss.mrz_to_en;
    for (i = 0; i < length; i++)
    {
        if (let == value[i])
        {
            return mrz[i];
        }
    }
    return 0;
}

void StartEncoding(char *input, char lang)
{
    int fullLength = 0, i;
    char ch;
    char *answer = (char *)malloc(sizeof(char));
    for (i = 0; input[i]; i++)
    {
        if (input[i] == '\n')
        {
            fullLength++;
            answer = (char *)realloc(answer, fullLength * sizeof(char));
            answer[fullLength - 1] = input[i];
            continue;
        }
        char *z = encode(input[i], lang);
        if (z == 0)
        {
            printf("\nНевозможно закодировать символ '%c':\n\tНе определено значение в словаре\n\tлибо язык перевода выбран неправильно", input[i]);
            free(input);
            free(answer);
            return;
        }
        int tmp = fullLength;
        fullLength = fullLength + strlen(z) + 1;
        answer = (char *)realloc(answer, fullLength * sizeof(char));
        answer[tmp] = 0;
        strcat(answer, " ");
        strcat(answer, z);
    }
    printf("\n\tКодирование завершено: %s\n", answer);
    printf("\nСохранить результат в отдельном файле?");
    printf("\n\t1 - Да  Любая кнопка - Нет\n");
    fflush(stdin);
    scanf("%c", &ch);
    if (ch == '1')
    {
        SaveData(answer);
    }
    free(answer);
    free(input);
}

void StartDecoding(char *input, char lang)
{
    char ch;
    int i, j, k;
    char *answer = (char *)malloc(sizeof(char));
    char *letter = (char *)malloc(sizeof(char));
    letter[0] = 0;
    while (1)
    {
        if (input[j] == 0 && letter[0] == 0)
        {
            break;
        }
        if (input[j] == ' ' && letter[0] == 0)
        {
            j++;
            continue;
        }
        if (input[j] == ' ' || input[j] == 0 || input[j] == '\n')
        {
            if (input[j] == '\n')
                answer[k] = input[j];
            else
            {
                char z = decode(letter, lang);
                if (z == 0)
                {
                    printf("\nНевозможно закодировать символ '%c':\n\tНе определено значение в словаре\n\tлибо язык перевода выбран неправильно", input[i]);
                    free(input);
                    free(answer);
                    free(letter);
                    return;
                }
                answer[k] = z;
            }
            letter = (char *)realloc(letter, sizeof(char));
            letter[0] = 0;
            k++;
            j++;
            answer = (char *)realloc(answer, (k + 1) * sizeof(char));
            i = 0;
            if (input[j - 1] == 0)
                break;
            continue;
        }
        i++;
        letter = (char *)realloc(letter, (i + 1) * sizeof(char));
        letter[i - 1] = input[j];
        letter[i] = 0;
        j++;
    }
    printf("\n\tДекодирование завершено: %s\n", answer);
    printf("\nСохранить результат в отдельном файле?");
    printf("\n\t1 - Да  Любая кнопка - Нет\n");
    fflush(stdin);
    scanf("%c", &ch);
    if (ch == '1')
    {
        SaveData(answer);
    }
    free(answer);
    free(input);
    free(letter);
    return;
}

void Start(char mode, char inputmode, char lang)
{
    char path[50];
    char *input;
    int i = 0, j, k, n = 50;
    input = (char *)malloc(n * sizeof(char));
    FILE *file;
    if (inputmode == '1')
    {
        printf("\nИмя файла: ");
        fflush(stdin);
        scanf("%s", &path);
        if ((file = fopen(path, "r")) == NULL)
        {
            printf("\n\tНеверное имя файла");
            return;
        }
        printf("\n File");
        for (i = 0; fscanf(file, "%c", &input[i]) == 1; i++)
        {
            if (i + 5 > n)
            {
                input = (char *)realloc(input, (i + 5) * sizeof(char));
                n += 5;
            }
        }
        input[i] = 0;
    }
    if (inputmode == '2')
        while (1)
        {
            printf("\nТекст: ");
            fflush(stdin);
            scanf("%c", &input[0]);
            for (i = 1; input[i - 1] != ENTER && input[i - 1] != 10; i++)
            {
                if (i + 5 > n)
                {
                    input = (char *)realloc(input, (i + 5) * sizeof(char));
                    n += 5;
                }
                scanf("%c", &input[i]);
            }
            if (i > 1)
            {
                input[i - 1] = 0;
                break;
            }
        }
    if (mode == '1')
        StartEncoding(input, lang);
    else
        StartDecoding(input, lang);
}

int main()
{
    char choice, lang, mode;
    system("chcp 1251");
    setlocale(LC_ALL, "Russian");
    do
    {
        printf("\n\t\t\tАзбука Морзе");
        printf("\n\t1. Перевод текста в морзе");
        printf("\n\t2. Расшифровка текста");
        printf("\n\t3. Выход");
        printf("\n\n");
        fflush(stdin);
        scanf("%c", &mode);
        if (mode != '1' && mode != '2' && mode != '3')
        {
            printf("\n\tНекорректный ввод\n");
            continue;
        }
        if (mode == '3')
        {
            exit(0);
        }
        printf("\n\t1. Русский");
        printf("\n\t2. Английский\n");
        fflush(stdin);
        scanf("%c", &lang);
        if (lang == '1' && lang == '2')
        {
            printf("\n\tНекорректный ввод\n");
            continue;
        }
        printf("\n\t1. Прочитать файл");
        printf("\n\t2. Ввести в консоли\n");
        fflush(stdin);
        scanf("%c", &choice);
        if (choice == '1' && choice == '2')
        {
            printf("\n\tНекорректный ввод\n");
            continue;
        }
        Start(mode, choice, lang);
    } while (1);
}