#include "Strings.h"

string *CopyText (const string *Text, size_t nLines)
    {
    assert (Text != NULL);

    string *cpyText = (string *)calloc (nLines, sizeof(string));

    for (size_t i = 0; i < nLines; ++i)
        *(cpyText + i) = *(Text + i);

    return cpyText;
    }

//-----------------------------------------------------------------------------

size_t ReadFile (const char *file_name,  string **text, char **buffer)
    {
    assert (text   != NULL);
    assert (buffer != NULL);

    FILE *file_ptr = fopen (file_name, "r");
    assert (file_ptr != NULL);

    *buffer = (char *)calloc (SizeOfFile (file_ptr) + 1, sizeof (char));

    size_t nSym = fread (*buffer, sizeof(char), SizeOfFile (file_ptr),
                         file_ptr);

    size_t nStrings = count (*buffer, nSym, '\n');
    *text           = (string *)calloc (nStrings, sizeof (string));

    nStrings = GetStrings (*text, nStrings, *buffer, nSym);

    fclose (file_ptr);
    file_ptr = NULL;

    return nStrings;
    }

//-----------------------------------------------------------------------------

size_t count (const char *Array, size_t size, char sym)
    {
    assert (Array != NULL);

    size_t count = 0;

    for (size_t i = 0; i < size; ++i)
        if (*(Array + i) == sym)
            ++count;

    return count;
    }


//-----------------------------------------------------------------------------

int StringComp (const void *void_str1, const void *void_str2)
    {
    const string *str1 = (string *)void_str1;
    const string *str2 = (string *)void_str2;

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

int BackStringComp (const void *void_str1, const void *void_str2)
    {
    const string *str1 = (string *)void_str1;
    const string *str2 = (string *)void_str2;

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

void WriteToFile (const string* str, size_t nLines, FILE *file)
    {
    assert (file != NULL);
    assert (str  != NULL);

    for (size_t i = 0; i < nLines; ++i)
        fprintf (file, "%.*s", (str + i)->size, (str + i)->begin);

    }

//-----------------------------------------------------------------------------

void PrintRhymes (const string *text, size_t nLines, FILE *file)
    {

    assert(text != NULL);
    assert(file != NULL);

    string *str = CopyText (text, nLines);

    qsort (str, nLines, sizeof(string), BackStringComp);

    size_t str_num = 0; //< сколько строк было записано

    srand ((unsigned int)time (0));

    size_t i = rand () % (nLines - 2);  //< индекс дл€ первой строки
    size_t j = rand () % (nLines - 2);  //< индекс дл€ второй строки

    size_t step = 14;

    for (; str_num < nLines; i = (i*j + nLines/step) % (nLines - 2),
                             j = (i*j + nLines/step) % (nLines - 2))
        {
        if      (str_num % step == 0 )
            str_num += DoRhyme (file, str, i, j    , i + 2, j + 2);


        else if (str_num % step == 4 )
            str_num += DoRhyme (file, str, i, i + 2, j    , j + 2);

        else if (str_num % step == 8 )
            str_num += DoRhyme (file, str, i, j    , j + 2, i + 2);


        else if (str_num % step == 12)
            str_num += DoRhyme (file, str, i, i + 2, EMPTY, EMPTY);

        }
    }

//-----------------------------------------------------------------------------

size_t DoRhyme    (FILE *file, const string* rhymes, int num1, int num2,
                                                     int num3, int num4)
    {
    #define PrintString(num) if((num) == EMPTY)                                 \
                                fprintf(file, "\n");                            \
                             else                                               \
                                {                                               \
                                fprintf (file, "%.*s", (rhymes + (num))->size,  \
                                                       (rhymes + (num))->begin);\
                                ++nStr;                                         \
                                }

    size_t nStr = 0;

    PrintString(num1)
    PrintString(num2)
    PrintString(num3)
    PrintString(num4)

    #undef PrintStirng

    return nStr;
    }

//-----------------------------------------------------------------------------

size_t SizeOfFile (FILE *file)
    {
    assert (file != NULL);

    size_t pos = 0;

    fseek (file, 0, SEEK_END);
    pos = ftell(file);
    fseek (file, 0, SEEK_SET);

    return pos;
    }


//-----------------------------------------------------------------------------

bool ispoint (char ch)
    {
    return ch == '.';
    }

//-----------------------------------------------------------------------------

int GetStrings (string     *str, size_t nLines,
                const char *ch,  size_t nChars)
    {
    assert (ch != NULL);

    size_t ch_num  = 0;
    size_t str_num = 0;
    int    sz_str  = 1;
    bool   inside  = false;

    for (;ch_num < nChars && str_num < nLines; ++ch_num, ++sz_str)
        {
        if (!inside)
            {
            if (isChapterTitle (ch + ch_num))
                {
                while (*(ch + ch_num) != '\n' && ch_num < nChars) ++ch_num;
                --sz_str;
                continue;
                }

            while ((isspace (*(ch + ch_num))  ||
                    ispoint (*(ch + ch_num))) && ch_num < nChars)
                ++ch_num;

            (str + str_num)->begin = ch + ch_num;   //!?  const char * const ch
            inside = true;
            }

        else if (*(ch + ch_num) == '\n')
            {
            (str + str_num)->size = sz_str;
            sz_str = 0;

            ++str_num;

            inside = false;
            }

        }

    return str_num;
    }

//-----------------------------------------------------------------------------

void PrintTitle (FILE* file, const char *str)
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

bool isChapterTitle (const char* str)
    {
    assert (str != NULL);

    for (size_t i = 0; *(str + i) != '\n' && *(str + i) != NULL; ++i)
        {
        if (isprint (*(str + i))) continue;

        if ( !(*(str + i) >= 'A' && *(str + i) <= 'Z' ||
               *(str + i) == '’'                      ||
               *(str + i) >= 'ј' && *(str + i) <= 'я'   ))
            return false;
        }

    return true;
    }

//-----------------------------------------------------------------------------

