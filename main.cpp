#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#include "Strings.h"


int main ()
    {
    printf ("Text sorting program\n\n");

    string *Onegin_sorted = NULL;
    char   *Onegin_buffer = NULL;

    size_t nStrings = ReadFile ("Evgen1.txt", &Onegin_sorted, &Onegin_buffer);
    assert (Onegin_sorted != NULL);
    assert (Onegin_buffer != NULL);

    printf ("Start text sorting...\n");

    qsort (Onegin_sorted, nStrings, sizeof (string), StringComp);

    printf ("Sorting finished\n\n");

    FILE *Answer_file = fopen ("Onegin.txt", "w");
    assert (Answer_file != NULL);

    printf ("Start writing to file...\n");

    PrintTitle  (Answer_file, "Евгений Онегин, отсортированный по возрастанию");
    WriteToFile (Onegin_sorted, nStrings, Answer_file);

    PrintTitle  (Answer_file,  "                               Бредогенератор");
    PrintRhymes (Onegin_sorted, nStrings, Answer_file);

    PrintTitle  (Answer_file, "                   Оригинальный Евгений Онегин");
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

