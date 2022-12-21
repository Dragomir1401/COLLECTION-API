#include <stdio.h>
#include <stdlib.h>
#include "hashmap.h"
#include <string.h>
#include <time.h>

#define BUFFER_MAX 100
#define HMAX 1999999
char *parse_input_file()
{
    char *string = (char *)malloc(20 * sizeof(char));

    strcpy(string, "test.in");
    return string;
}

char *parse_output_file()
{
    char *string = (char *)malloc(20 * sizeof(char));

    strcpy(string, "test.out");
    return string;
}

int main()
{
    hashtable_t *ht = ht_create(HMAX, hash_function, compare_function_ints);
    char *in_file = parse_input_file();
    char *out_file = parse_output_file();
    // open input and output file
    FILE *in = fopen(in_file, "r");
    FILE *out = fopen(out_file, "w");
    char *chunk = malloc(BUFFER_MAX);

    clock_t t;
    t = clock();
    // read line by line
    while (fgets(chunk, BUFFER_MAX, in) != NULL)
    {
        // separate each line
        char *token = strtok(chunk, " ");
        int command = atoi(token);

        long arg1 = 0, arg2 = 0;
        // loop through the string to extract all other tokens
        if (command >= 0 && command <= 3)
        {
            token = strtok(NULL, " \n");
            if (token != NULL)
                arg1 = atoi(token);
        }

        if (token != NULL && command == 3)
        {
            token = strtok(NULL, " \n");
            if (token != NULL)
                arg2 = atoi(token);
        }

        switch (command)
        {
        case 0:
            ht_put(ht, &arg1, sizeof(long), &arg1, sizeof(long));
            break;
        case 1:
            ht_remove_entry(ht, &arg1);
            break;
        case 2:
            fprintf(out, "%d\n", ht_has_key(ht, &arg1));
            break;
        case 3:
            ht_put(ht, &arg2, sizeof(long), &arg2, sizeof(long));
            break;
        case 4:
            ht_print(ht, out);
            break;

        default:
            break;
        }
    }

    t = clock() - t;
    double time_taken = ((double)t) / CLOCKS_PER_SEC; // in seconds

    printf("TEST SINGULAR HT took %f seconds to execute.\n", time_taken);

    free(chunk);
    free(in_file);
    free(out_file);
    fclose(in);
    fclose(out);
    ht_free(ht);

    return 0;
}