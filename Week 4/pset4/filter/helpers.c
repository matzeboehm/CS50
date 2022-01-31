#include "helpers.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    int avg;

    for (int i = 0; i < height; i ++)
    {
        for (int j = 0; j < width; j++)
        {
            avg = (int) round((image[i][j].rgbtRed + image[i][j].rgbtGreen + image[i][j].rgbtBlue) / 3.0);

            if (avg > 255)
            {
                avg = 255;
            }

            image[i][j].rgbtRed = avg;
            image[i][j].rgbtGreen = avg;
            image[i][j].rgbtBlue = avg;

        }
    }

    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    int originalRed;
    int originalGreen;
    int originalBlue;

    int sepiaRed;
    int sepiaGreen;
    int sepiaBlue;

    for (int i = 0; i < height; i ++)
    {
        for (int j = 0; j < width; j++)
        {
            originalRed = image[i][j].rgbtRed;
            originalGreen = image[i][j].rgbtGreen;
            originalBlue = image[i][j].rgbtBlue;

            sepiaRed = (int) round(.393 * originalRed + .769 * originalGreen + .189 * originalBlue);
            sepiaGreen = (int) round(.349 * originalRed + .686 * originalGreen + .168 * originalBlue);
            sepiaBlue = (int) round(.272 * originalRed + .534 * originalGreen + .131 * originalBlue);

            if (sepiaRed > 255)
            {
                sepiaRed = 255;
            }
            if (sepiaGreen > 255)
            {
                sepiaGreen = 255;
            }
            if (sepiaBlue > 255)
            {
                sepiaBlue = 255;
            }

            image[i][j].rgbtRed = sepiaRed;
            image[i][j].rgbtGreen = sepiaGreen;
            image[i][j].rgbtBlue = sepiaBlue;
        }
    }

    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    int tempRed;
    int tempGreen;
    int tempBlue;

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width / 2; j++)
        {
            tempRed = image[i][j].rgbtRed;
            tempGreen = image[i][j].rgbtGreen;
            tempBlue = image[i][j].rgbtBlue;

            image[i][j].rgbtRed = image[i][width - j - 1].rgbtRed;
            image[i][j].rgbtGreen = image[i][width - j - 1].rgbtGreen;
            image[i][j].rgbtBlue = image[i][width - j - 1].rgbtBlue;

            image[i][width - j - 1].rgbtRed = tempRed;
            image[i][width - j - 1].rgbtGreen = tempGreen;
            image[i][width - j - 1].rgbtBlue = tempBlue;
        }
    }

    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE newFigure[height][width];
    int sumRed;
    int sumGreen;
    int sumBlue;
    int divisor;

    // Loop over every pixel
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            sumRed = 0;
            sumGreen = 0;
            sumBlue = 0;
            divisor = 0;
            
            // Generate an inteneral 3x3 loop
            for (int row = - 1; row <= 1; row++)
            {
                for (int col = -1; col <= 1; col++)
                {
                    // Check wether the condition is out of bounds or not
                    if ((i + row >= 0 && i + row < height) && (j + col >= 0 && j + col < width))
                    {
                        sumRed = sumRed + image[i + row][j + col].rgbtRed;
                        sumGreen = sumGreen + image[i + row][j + col].rgbtGreen;
                        sumBlue = sumBlue + image[i + row][j + col].rgbtBlue;
                        divisor = divisor + 1;
                    }
                }
            }
            
            // Save new RBG values
            newFigure[i][j].rgbtRed = (int) round((float)(sumRed) / (float)divisor); 
            newFigure[i][j].rgbtGreen = (int) round((float)(sumGreen) / (float)divisor); 
            newFigure[i][j].rgbtBlue = (int) round((float)(sumBlue) / (float)divisor); 
        }
    }
    
    // Copy result to original file
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = newFigure[i][j];
        }
    }
    
    return;
}
