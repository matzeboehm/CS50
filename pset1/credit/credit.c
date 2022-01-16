#include <cs50.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

void decision(int amount_digits, int firstdigit, int seconddigit, int security);

int main(void)
{
    long number;
    do
    {
        number = get_long("Credit Card Number: ");
    }
    while (number < 1 || number > 9999999999999999);
    
    // Get amount of digits
    int amount_digits = floor(log10(number)) + 1;
    
    // Initialize variables
    int firstdigit;
    int seconddigit;
    long temp_number = number;
    int temp;
    int sumeven = 0;
    int sumodd = 0;
    
    for (int i = 1; i < amount_digits + 1; i++)
    {
        temp = temp_number % 10;
        temp_number = (temp_number - temp) / 10;
        
        // Check if first or second digit from front
        if (i == amount_digits)
        {
            firstdigit = temp;
        }
        else if (i == amount_digits - 1)
        {
            seconddigit = temp;
        }
        
        // Check if every other number
        if (i % 2 == 0)
        {
            temp = temp * 2;
            if (temp < 10)
            {
                sumeven = sumeven + temp;
            }
            else
            {
                sumeven = sumeven + temp % 10 + (temp -  temp % 10) / 10;
            }
        }
        // Sum of odd numbers
        else 
        {
            sumodd = sumodd + temp;
        }
    }
    
    // Finding security number
    int security = (sumodd + sumeven) % 10;
    
    // Final Decision
    decision(amount_digits, firstdigit, seconddigit, security);
    
}

void decision(int amount_digits, int firstdigit, int seconddigit, int security)
{
    // Check if AMEX
    if (amount_digits == 15 && security == 0 && firstdigit == 3)
    {
        if (seconddigit == 4 || seconddigit == 7)
        {
            printf("AMEX\n");
        }
        else
        {
            printf("INVALID\n");
        }
    }
    //Check if Master
    else if (amount_digits == 16 && security == 0 && firstdigit == 5)
    {
        if (seconddigit == 1 || seconddigit == 2 || seconddigit == 3 || seconddigit == 4 || seconddigit == 5)
        {
            printf("MASTERCARD\n");
        }
        else
        {
            printf("INVALID\n");
        }
    }
    // Check if Visa
    else if ((amount_digits == 13 || amount_digits == 16) && security == 0 && firstdigit == 4)
    {
        printf("VISA\n");
    }
    // Otherwise invalid
    else
    {
        printf("INVALID\n");
    }
}
