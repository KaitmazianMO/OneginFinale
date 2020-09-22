#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

//{----------------------------------------------------------------------------
//! \brief Считает количество вхождений символа в массиве.
//!
//! \param [in] Array- массив, в котором производится поиск;
//! \param [in] c    - искомый символ;
//! \param [in] size - размер массива;
//!
//! \return Количество вхождений символа в массив.
//}----------------------------------------------------------------------------

size_t count      (const char *Array, size_t size, char sym);

//{----------------------------------------------------------------------------
//! \brief Возвращает минимальный элемент из a и b.
//}----------------------------------------------------------------------------

int    min        (int a, int b);

//{----------------------------------------------------------------------------
//! \brief Считает количество символов файле.
//!
//! \param [in] file - файл, в котором считается количество символов;
//!
//! \return Количество символов файле.
//}----------------------------------------------------------------------------

size_t SizeOfFile (FILE *file);

//{----------------------------------------------------------------------------
//! \brief Записывает в файл текст, обрамляя его полосками.
//!
//! \param [in] file - файл, в который производится запись;
//! \param [in] str  - записываемый текст;
//}----------------------------------------------------------------------------

void   PrintTitle (FILE* file, const char *str);

//{----------------------------------------------------------------------------
//! Структура строки.
//}----------------------------------------------------------------------------

typedef struct string
    {
    char *begin = NULL;    ///< указатель на начало строки;
    size_t size = 0;       ///< размер строки;
    };

//{----------------------------------------------------------------------------
//! \brief Заполняет массив строк, используя массив символов.
//!
//! \param [in] str    - указаетль на начало массива строк,
//!                      в который ведется запись;
//! \param [in] nLines - размер массива строк;
//! \param [in] ch     - указаетль на начало массива символов,
//!                      из которого ведется запись;
//! \param [in] nChars - размер масива символов;
//!
//! \return Количество записанных в массив строк.
//}----------------------------------------------------------------------------

int    GetStrings          (string *str, size_t nLines, const char *ch, size_t nChars);

//{----------------------------------------------------------------------------
//! \brief Записывает сожержимое массива строк в файл.
//!
//! \param [in] str    - указаетль на начало массива строк,
//!                      из которого ведется запись;
//! \param [in] nLines - размер массива строк;
//! \param [in] file   - файл, в который ведется запись;
//}----------------------------------------------------------------------------

void   WriteToFile         (const string* str, size_t nLines, FILE *file);

//{----------------------------------------------------------------------------
//! Относительно случайно записывает рифмованные строки в файл.
//}----------------------------------------------------------------------------

void   RandomWriteToFile   (const string *text, size_t nLines, FILE *file);

//{----------------------------------------------------------------------------
//! Сравнивает строки с конца, игнорируя знаки пунктуации.
//}----------------------------------------------------------------------------

int    BackStringComp      (const string *str1, const string *str2);

//{----------------------------------------------------------------------------
//! Компаратор для структуры string.
//}----------------------------------------------------------------------------

int    StringComp          (const string *str1, const string *str2);

//{----------------------------------------------------------------------------
//! \brief Проверяет является ли строка римcкой
//!        цифрой или набором из римских цифр.
//!
//! \param [in] - проверяемая строка;
//}----------------------------------------------------------------------------

bool   isChapterTitle      (const char* str);

//{----------------------------------------------------------------------------
//! \brief Записывает все символы файла в буффер и
//!        преобразует их в массив отсортированных строк.
//!
//! \param [in] file_name - имя читаемого файла;
//! \param [in] text      - указатель на массив строк, в который будут
//!                         записываться содержательные строки файла
//!                        (все кроме пустых строк, названий глав и тд);
//! \param [in] buffer    - массив указателей, в который записываются
//!                         символы файла file_name;
//!
//! \return Количество записанных "содержательных" строк в *text.
//}----------------------------------------------------------------------------

size_t GetSortedStrings    (const char *file_name,  string **text,
                                                    char   **buffer);

//{----------------------------------------------------------------------------
//! \brief Копирует текст в массив и возврашает указатель на его начало.
//!
//! \param [in] Text   - указатель на массив строк;
//! \param [in] nLines - размер массива строк;
//!
//! \return Возвращает указатель на начало скопированного массива строк.
//}----------------------------------------------------------------------------

string *CopyText           (string *Text, size_t nLines);

int main ()
    {
    printf ("Text sorting program\n\n");

    string *Onegin_sorted = NULL;
    char   *Onegin_text   = NULL;

    size_t nStrings = GetSortedStrings ("Evgen1.txt", &Onegin_sorted, &Onegin_text);
    assert (Onegin_sorted != NULL);
    assert (Onegin_text   != NULL);

    FILE *Answer_file = fopen ("Onegin.txt", "w");
    assert (Answer_file != NULL);

    printf ("Start writing to file...\n");

    PrintTitle  (Answer_file, "Евгений Онегин, отсортированный по возрастанию");
    WriteToFile (Onegin_sorted, nStrings, Answer_file);

    PrintTitle        (Answer_file,  "           Евгений Онегин by Netflix...");
    RandomWriteToFile (Onegin_sorted, nStrings, Answer_file);

    PrintTitle  (Answer_file, "                   Оригинальный Евгений Онегин");
    fprintf     (Answer_file, "%s", Onegin_text);

    printf ("Writing finished\n");

    fclose (Answer_file  );
    free   (Onegin_text  );
    free   (Onegin_sorted);

    Answer_file   = NULL;
    Onegin_text   = NULL;
    Onegin_sorted = NULL;

    return 0;
    }

//-----------------------------------------------------------------------------

string *CopyText        (const string *Text, size_t nLines)
    {
    assert (Text != NULL);

    string *cpyText = (string *)calloc (nLines, sizeof(string));

    for (size_t i = 0; i < nLines; ++i)
        {
        (cpyText + i)->begin = (Text + i)->begin;
        (cpyText + i)->size  = (Text + i)->size;
        }

    return cpyText;
    }

//-----------------------------------------------------------------------------

size_t GetSortedStrings (const char *file_name,  string **text,
                                                 char   **buffer)
    {
    assert (text   != NULL);
    assert (buffer != NULL);

    FILE *file_ptr = fopen (file_name, "r");
    assert (file_ptr != NULL);

    *buffer = (char *)calloc (SizeOfFile (file_ptr), sizeof (char));

    size_t nSym = fread (*buffer, sizeof(char), SizeOfFile (file_ptr),
                         file_ptr);

    size_t nStrings = count (*buffer, nSym, '\n');
    *text           = (string *)calloc (nStrings, sizeof (string));

    nStrings = GetStrings (*text, nStrings, *buffer, nSym);

    printf ("Start sorting...\n");

    qsort (*text, nStrings, sizeof (string),
           (int (*)(const void*, const void*))StringComp);

    printf ("Sorting finishied\n\n");

    fclose (file_ptr);
    file_ptr = NULL;

    return nStrings;
    }

//-----------------------------------------------------------------------------

size_t count            (const char *Array, size_t size, char sym)
    {
    assert (Array != NULL);

    size_t count = 0;

    for (size_t i = 0; i < size; ++i)
        if (*(Array + i) == sym)
            ++count;

    return count;
    }

//-----------------------------------------------------------------------------

int min                 (int a, int b)
    {
    return (a < b) ? a : b;
    }

//-----------------------------------------------------------------------------

int StringComp          (const string *str1, const string *str2)
    {
    assert (str1->begin != NULL);
    assert (str2->begin != NULL);

    size_t i = 0;
    size_t j = 0;
    for (i = 0, j = 0; i < min (str1->size, str2->size) &&
                       j < min (str1->size, str2->size); ++i, ++j)
        {
        if (isgraph (*(str1->begin + i)))
            {
            i++;
            if (isspace (*(str1->begin + i)))
                i++;
            }
        if (isgraph (*(str2->begin + j)))
            {
            j++;
            if (isspace (*(str2->begin + j)))
                j++;
            }

        if (*(str1->begin + i) != *(str2->begin + j))
            return *(str1->begin + i) - *(str2->begin + j);
        }

    return *(str1->begin + i) - *(str2->begin + j);
    }

//-----------------------------------------------------------------------------

int BackStringComp      (const string *str1, const string *str2)
    {
    assert (str1->begin != NULL);
    assert (str2->begin != NULL);

    size_t i = str1->size;
    size_t j = str2->size;
    for(; i > 0 && j > 0; --i, --j)
        {
        while (isprint (*(str1->begin + i)) || isdigit (*(str1->begin + i))) --i;
        while (isprint (*(str2->begin + j)) || isdigit (*(str2->begin + j))) --j;

        if (*(str1->begin + i) != *(str2->begin + j))
            return *(str1->begin + i) - *(str2->begin + j);
        }

    return *(str1->begin + i) - *(str2->begin + j);
    }

//-----------------------------------------------------------------------------

void WriteToFile        (const string* str, size_t nLines, FILE *file)
    {
    assert (file != NULL);
    assert (str  != NULL);

    for (size_t i = 0; i < nLines; ++i)
        fprintf (file, "%.*s", (str + i)->size, (str + i)->begin);

    }

//-----------------------------------------------------------------------------

void RandomWriteToFile  (const string* text, size_t nLines, FILE *file)
    {
    assert(text != NULL);
    assert(file != NULL);

    string *str = CopyText (text, nLines);

    qsort (str, nLines, sizeof(string),
           (int (*)(const void*, const void*))BackStringComp);

    size_t nStr = 0; //< сколько строк было записано

    srand ((unsigned int)time (0));

    size_t i = rand () % nLines;  //< индекс для первой строки
    size_t j = rand () % nLines;  //< индекс для второй строки

    size_t step = 14;

    for (; nStr < nLines; i = (i*j + nLines/step) % nLines,
                          j = (i*j + nLines/step) % nLines)
        {
        if      (nStr % step == 0 )
            {
            fprintf (file, "%.*s", (str + i)->size,     (str + i)->begin    );
            fprintf (file, "%.*s", (str + j)->size,     (str + j)->begin    );

            fprintf (file, "%.*s", (str + i + 2)->size, (str + i + 2)->begin);
            fprintf (file, "%.*s", (str + j + 2)->size, (str + j + 2)->begin);

            nStr += 4;
            }

        else if (nStr % step == 4 )
            {
            fprintf (file, "%.*s", (str + i)->size,     (str + i)->begin    );
            fprintf (file, "%.*s", (str + i + 2)->size, (str + i + 2)->begin);

            fprintf (file, "%.*s", (str + j)->size,     (str + j)->begin    );
            fprintf (file, "%.*s", (str + j + 2)->size, (str + j + 2)->begin);

            nStr += 4;
            }

        else if (nStr % step == 8 )
            {
            fprintf (file, "%.*s", (str + i)->size,     (str + i)->begin    );

            fprintf (file, "%.*s", (str + j)->size,     (str + j)->begin    );
            fprintf (file, "%.*s", (str + j + 2)->size, (str + j + 2)->begin);

            fprintf (file, "%.*s", (str + i + 2)->size, (str + i + 2)->begin);

            nStr += 4;
            }

        else if (nStr % step == 12)
            {
            fprintf (file, "%.*s", (str + i)->size,     (str + i)->begin    );
            fprintf (file, "%.*s", (str + i + 2)->size, (str + i + 2)->begin);

            fprintf (file, "\n\n");

            nStr += 2;
            }
        }
    }

//-----------------------------------------------------------------------------

size_t SizeOfFile       (FILE *file)
    {
    assert (file != NULL);

    size_t pos = 0;

    fseek (file, 0, SEEK_END);
    pos = ftell (file);
    fseek (file, 0, SEEK_SET);

    return pos;
    }

//-----------------------------------------------------------------------------

int GetStrings          (string     *str, size_t nLines,
                         const char *ch,  size_t nChars)
    {
    assert (ch != NULL);

    size_t nCh    = 0;        // индекс массива ch
    size_t nStr   = 0;        // индекс массива str
    int    szStr  = 1;        // размер строки в данный момент
    bool   inside = false;    // внутри строки - true, строка закончилась - false

    for (;nCh < nChars && nStr < nLines; ++nCh, ++szStr)
        {
        if (!inside)
            {
            if (isChapterTitle (ch + nCh))
                {
                while (*(ch + nCh) != '\n' && nCh < nChars) ++nCh;
                --szStr;     //после continue sz увеличится на 1, хотя должен остаться прежним
                continue;
                }

            while ((isspace (*(ch + nCh)) ||
                   *(ch + nCh) == '.'  )  &&  nCh < nChars)
                {
                ++nCh;
                }


            (str + nStr)->begin = (char *)ch + nCh;   //!?
            inside = true;
            }

        else if (*(ch + nCh) == '\n')
            {
            (str + nStr)->size = szStr;
            szStr = 0;

            ++nStr;

            inside = false;
            }

        }

    return nStr;
    }

//-----------------------------------------------------------------------------

void PrintTitle         (FILE* file, const char *str)
    {
    char line    [100] = {0};
    char crosline[100] = {0};

    for (size_t i = 0; i < 100; ++i)
        {
        line[i]     = '-';
        crosline[i] = '|';
        }

    fprintf (file, "\n\n%.100s\n%.100s\n%.100s\n%.100s\n\n",
             line, crosline, crosline, line);
    fprintf (file, "\n\n\n\n\n%60s\n\n\n\n\n", str);
    fprintf (file, "%.100s\n%.100s\n%.100s\n%.100s\n\n\n\n",
             line, crosline, crosline, line);
    }

//-----------------------------------------------------------------------------

bool isChapterTitle     (const char* str)
    {
    for (size_t i = 0; *(str + i) != '\n' && *(str + i) != NULL; ++i)
        {
        if (isprint (*(str + i))) continue;

        if ( !(*(str + i) >= 'A' && *(str + i) <= 'Z' ||
               *(str + i) == 'Х'                      ||
               *(str + i) >= 'А' && *(str + i) <= 'Я')  )
            return false;
        }

    return true;
    }

//-----------------------------------------------------------------------------

