#include <stdio.h>
#include <cs50.h>

int main(void)
{
    // Getting user input
    string name = get_string("Enter your name: \n");
    
    // Greet user name
    printf("Hello, %s\n", name);
}