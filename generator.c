#include "treap.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

int randint(int lower, int upper)
{
    if (upper - lower + 1 <= 0)
        return 0;
    return (rand() % (upper - lower + 1)) + lower;
}

char *parse_input_file(int i)
{
    char *string = malloc(20);
    strcpy(string, "./in/test");
    int i_cop = i + 1;
    char c = i_cop + '0';
    if (i <= 9)
        string[9] = c;

    if (i_cop > 9 && i_cop <= 19)
    {
        c = c - 10;
        string[9] = '1';
        string[10] = c;
    }

    if (i_cop > 19 && i_cop <= 29)
    {
        c = c - 20;
        string[9] = '2';
        string[10] = c;
    }

    if (i_cop == 30)
    {
        string[9] = '3';
        string[10] = '0';
    }

    return string;
}

void add_only_test(char *in_file, int magnitude)
{
    FILE *fptr = fopen(in_file, "w");
    if (fptr == NULL)
    {
        printf("Error!");
        exit(1);
    }

    for (int i = 1; i < magnitude; i++)
    {
        fprintf(fptr, "%d %d\n", 0, randint(0, 100));
    }
    fclose(fptr);
}

int delete_from_array(int *array, int size)
{
    int randed = randint(0, size - 1);
    int res = array[randed];

    for (int i = randed; i < size - 1; i++)
        array[i] = array[i + 1];

    return res;
}

int pick_from_array(int *array, int size)
{
    int randed = randint(0, size - 1);
    int res = array[randed];
    return res;
}

void add_and_remove_test(char *in_file, int magnitude, int delete_frequency)
{
    FILE *fptr = fopen(in_file, "w");
    if (fptr == NULL)
    {
        printf("Error!");
        exit(1);
    }

    fprintf(fptr, "method----%d\n\n", delete_frequency);

    int add_counter = 0, remove_counter = 0, *added;
    added = malloc(100 * sizeof(int));

    for (int i = 1; i < magnitude; i++)
    {
        // rand some random
        int randed = randint(0, 100);
        fprintf(fptr, "%d %d\n", 0, randed);
        added[add_counter] = randed;
        add_counter++;

        if (add_counter >= remove_counter)
        {

            if (add_counter >= (delete_frequency + 1) * remove_counter)
            {
                fprintf(fptr, "%d %d\n", 1, delete_from_array(added, add_counter));
                add_counter--;
                remove_counter++;
            }
        }
    }

    free(added);
    fclose(fptr);
}

void add_remove_and_find_test(char *in_file, int magnitude, int delete_frequency, int find_frequency, int find_proportion)
{
    FILE *fptr = fopen(in_file, "w");
    if (fptr == NULL)
    {
        printf("Error!");
        exit(1);
    }

    fprintf(fptr, "method----%d\n\n", delete_frequency);

    int add_counter = 0, remove_counter = 0, *added, find_counter = 0;
    added = malloc(100 * sizeof(int));

    for (int i = 1; i < magnitude; i++)
    {
        // rand some random
        int randed = randint(0, 100);
        fprintf(fptr, "%d %d\n", 0, randed);
        added[add_counter] = randed;
        add_counter++;

        if (add_counter >= remove_counter)
        {
            if (add_counter >= (delete_frequency + 1) * remove_counter)
            {
                fprintf(fptr, "%d %d\n", 1, delete_from_array(added, add_counter));
                add_counter--;
                remove_counter++;
            }

            if (add_counter >= (find_frequency + 1) * find_counter)
            {
                if (find_proportion > 0)
                {
                    fprintf(fptr, "%d %d\n", 2, pick_from_array(added, add_counter));
                    find_counter++;
                    find_proportion--;
                }
                else
                {
                    fprintf(fptr, "%d %d\n", 2, randint(0, 1000));
                    find_counter++;
                }
            }
        }
    }

    free(added);
    fclose(fptr);
}

int main()
{
    int *magnitude_array = malloc(30 * sizeof(int));
    for (int i = 0; i < 30; i++)
    {
        magnitude_array[i] = i * 10 + 4;
    }

    for (int i = 0; i < 30; i++)
    {
        // create input file string
        char *in_string;
        in_string = parse_input_file(i);

        // generate some add only tests
        if (i + 1 == 1 || i + 1 == 3 || i + 1 == 5)
            add_only_test(in_string, magnitude_array[i]);

        // generate add and remove tests with
        if (i + 1 == 4 || i + 1 == 2 || i + 1 == 7)
            add_and_remove_test(in_string, magnitude_array[i], (i + 1) % 2);

        if (i + 1 == 8 || i + 1 == 6)
            add_and_remove_test(in_string, magnitude_array[i], 3);

        if (i + 1 == 9 || i + 1 == 10 || i + 1 == 11)
            add_remove_and_find_test(in_string, magnitude_array[i], randint(0, 5), randint(0, 3), magnitude_array[i] / 8);

        free(in_string);
    }
    return 0;
}