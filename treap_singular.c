#include <stdio.h>
#include <stdlib.h>
#include "treap.h"
#include <string.h>
#include <time.h>
#define BUFFER_MAX 100
#define MAX_NODES 1000000
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
    // iterate through tests
    Treap_tree *treap = treap_create(compare_ints);
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

        int arg1 = 0, arg2 = 0;
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
            if (!get_key(treap->root, arg1, treap->compar))
                treap_insert(&treap->root, arg1, treap->compar);
            break;
        case 1:
            treap_delete(&treap->root, arg1, treap->compar);
            break;
        case 2:
            if (get_key(treap->root, arg1, treap->compar))
                fprintf(out, "%d\n", 1);
            else
                fprintf(out, "%d\n", 0);

            break;
        case 3:
            treap_insert(&treap->root, arg2, treap->compar);
            break;
        case 4:
            printTreap(out, treap->root);
            fprintf(out, "\n");
            break;

        default:
            break;
        }
    }

    t = clock() - t;
    double time_taken = ((double)t) / CLOCKS_PER_SEC; // in seconds

    printf("TEST SINGULAR TREAP took %f seconds to execute.\n", time_taken);

    free(chunk);
    free(in_file);
    free(out_file);
    fclose(in);
    fclose(out);
    treap_free(treap);

    return 0;
}