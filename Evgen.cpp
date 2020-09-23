#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

//{----------------------------------------------------------------------------
//! \brief ������� ���������� ��������� ������� � �������.
//!
//! \param [in] Array- ������, � ������� ������������ �����;
//! \param [in] c    - ������� ������;
//! \param [in] size - ������ �������;
//!
//! \return ���������� ��������� ������� � ������.
//}----------------------------------------------------------------------------

size_t count      (const char *Array, size_t size, char sym);

//{----------------------------------------------------------------------------
//! \brief ���������� ����������� ������� �� a � b.
//}----------------------------------------------------------------------------

int    min        (int a, int b);

//{----------------------------------------------------------------------------
//! \brief ������� ���������� �������� �����.
//!
//! \param [in] file - ����, � ������� ��������� ���������� ��������;
//!
//! \return ���������� �������� �����.
//}----------------------------------------------------------------------------

size_t SizeOfFile (FILE *file);

//{----------------------------------------------------------------------------
//! \brief ���������� � ���� �����, �������� ��� ���������.
//!
//! \param [in] file - ����, � ������� ������������ ������;
//! \param [in] str  - ������������ �����;
//}----------------------------------------------------------------------------

void   PrintTitle (FILE* file, const char *str);

//{----------------------------------------------------------------------------
//! ��������� ������.
//}----------------------------------------------------------------------------

typedef struct string
    {
    char *begin = NULL;    ///< ��������� �� ������ ������;
    size_t size = 0;       ///< ������ ������;
    };

//{----------------------------------------------------------------------------
//! \brief ��������� ������ �����, ��������� ������ ��������.
//!
//! \param [in] str    - ��������� �� ������ ������� �����,
//!                      � ������� ������� ������;
//! \param [in] nLines - ������ ������� �����;
//! \param [in] ch     - ��������� �� ������ ������� ��������,
//!                      �� �������� ������� ������;
//! \param [in] nChars - ������ ������ ��������;
//!
//! \return ���������� ���������� � ������ �����.
//}----------------------------------------------------------------------------

int    GetStrings          (string *str, size_t nLines, const char *ch, size_t nChars);

//{----------------------------------------------------------------------------
//! \brief ���������� ���������� ������� ����� � ����.
//!
//! \param [in] str    - ��������� �� ������ ������� �����,
//!                      �� �������� ������� ������;
//! \param [in] nLines - ������ ������� �����;
//! \param [in] file   - ����, � ������� ������� ������;
//}----------------------------------------------------------------------------

void   WriteToFile         (const string* str, size_t nLines, FILE *file);

//{----------------------------------------------------------------------------
//! ������������ �������� ���������� ����������� ������ � ����.
//}----------------------------------------------------------------------------

void   RandomWriteToFile   (const string *text, size_t nLines, FILE *file);

//{----------------------------------------------------------------------------
//! ���������� ������ � �����, ��������� ����� ����������.
//}----------------------------------------------------------------------------

int    BackStringComp      (const string *str1, const string *str2);

//{----------------------------------------------------------------------------
//! ���������� ��� ��������� string.
//}----------------------------------------------------------------------------

int    StringComp          (const string *str1, const string *str2);

//{----------------------------------------------------------------------------
//! \brief ��������� �������� �� ������ ���c���
//!        ������ ��� ������� �� ������� ����.
//!
//! \param [in] - ����������� ������;
//}----------------------------------------------------------------------------

bool   isChapterTitle      (const char* str);

//{----------------------------------------------------------------------------
//! \brief ���������� ��� ������� ����� � ������ �.
//!        ����������� �� � ������ ��������������� �����
//!
//! \param [in] file_name - ��� ��������� �����;
//! \param [in] text      - ����� ��������� �� ������ �������
//!                         �����, � ������� ����� ������������
//!                         �������������� ������ �����
//!                        (��� ����� ������ �����, �������� ���� � ��);
//! \param [in] buffer    - ����� ��������� �� ������ ������� ��������,
//!                         � ������� ������������ ������� ����� file_name;
//!
//! \return ���������� ���������� "��������������" ����� � *text.
//}----------------------------------------------------------------------------

size_t GetSortedStrings    (const char *file_name,  string **text, char   **buffer,
                            int (*comp) (const void *, const void *));

//{----------------------------------------------------------------------------
//! \brief �������� ����� � ������ � ���������� ��������� �� ��� ������.
//!
//! \param [in] Text   - ��������� �� ������ �����;
//! \param [in] nLines - ������ ������� �����;
//!
//! \return ���������� ��������� �� ������ �������������� ������� �����.
//}----------------------------------------------------------------------------

string *CopyText           (string *Text, size_t nLines);

bool ispoint (char ch);

int main ()
    {
    printf ("Text sorting program\n\n");

    string *Onegin_sorted = NULL;
    char   *Onegin_buffer = NULL;

    size_t nStrings = GetSortedStrings ("Evgen1.txt", &Onegin_sorted, &Onegin_buffer,
                                        (int (*)(const void *, const void *)) StringComp);
    assert (Onegin_sorted != NULL);
    assert (Onegin_buffer != NULL);

    FILE *Answer_file = fopen ("Onegin.txt", "w");
    assert (Answer_file != NULL);

    printf ("Start writing to file...\n");

    PrintTitle  (Answer_file, "������� ������, ��������������� �� �����������");
    WriteToFile (Onegin_sorted, nStrings, Answer_file);

    PrintTitle        (Answer_file,  "           ������� ������ by Netflix...");
    RandomWriteToFile (Onegin_sorted, nStrings, Answer_file);

    PrintTitle  (Answer_file, "                   ������������ ������� ������");
    fprintf     (Answer_file, "%s", Onegin_buffer);

    printf ("Writing finished\n");

    fclose (Answer_file  );
    free   (Onegin_buffer);
    free   (Onegin_sorted);

    Answer_file   = NULL;
    Onegin_buffer = NULL;
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

size_t GetSortedStrings    (const char *file_name,  string **text, char **buffer,
                            int (*comp) (const void *, const void *))
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

    qsort (*text, nStrings, sizeof (string), comp);

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

    size_t nStr = 0; //< ������� ����� ���� ��������

    srand ((unsigned int)time (0));

    size_t i = rand () % nLines;  //< ������ ��� ������ ������
    size_t j = rand () % nLines;  //< ������ ��� ������ ������

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

int GetStrings          (string     *str, size_t nLines,
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

            (str + str_num)->begin = (char *)ch + ch_num;   //!?
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
    assert (str != NULL);

    for (size_t i = 0; *(str + i) != '\n' && *(str + i) != NULL; ++i)
        {
        if (isprint (*(str + i))) continue;

        if ( !(*(str + i) >= 'A' && *(str + i) <= 'Z' ||
               *(str + i) == '�'                      ||
               *(str + i) >= '�' && *(str + i) <= '�'   ))
            return false;
        }

    return true;
    }

//-----------------------------------------------------------------------------
