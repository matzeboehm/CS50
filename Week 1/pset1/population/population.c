#include <cs50.h>
#include <stdio.h>

int calculate(int current_n);

int main(void)
{
    // TODO: Prompt for start size
    int start_n;
    do
    {
        start_n = get_int("Start size: ");
    }
    while (start_n < 9);

    // TODO: Prompt for end size
    int end_n;
    do
    {
        end_n = get_int("End size: ");
    }
    while (end_n < start_n);

    // TODO: Calculate number of years until we reach threshold
    int n = 0;
    int current_n = start_n;

    while (current_n < end_n)
    {
        current_n = calculate(current_n);
        n++;
    }

    // TODO: Print number of years
    printf("Years: %i\n", n);
}

int calculate(int current_n)
{
    int born = current_n / 3;
    int pass = current_n / 4;

    current_n = current_n + born - pass;

    return current_n;
}