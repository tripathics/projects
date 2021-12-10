/**
 *  Problem with this code. Fixed it in helpers.c
 *  
 **/

#include "helpers.h"
#include <math.h>

/**
 * Find Gx and Gy by passing index wrt current element
 * Input: in_a, in_b: Indices of Gx and Gy wrt current element (-1 to 1)
 *        int *a, int *b: pointers to Gx and Gy
*/
void sobel(int in_a, int in_b, int *a, int *b);

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // adding subpixel values and dividing by 3.0 (basically taking average of subpixels)
            float average = (image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / 3.0;

            // rounding to nearest whole number and storing it in new_pixel
            int new_pixel = round(average);

            // copying new_pixel value in the pixel of image
            image[i][j].rgbtBlue = new_pixel;
            image[i][j].rgbtGreen = new_pixel;
            image[i][j].rgbtRed = new_pixel;
        }
    }

    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    // temporary structure ref_image for storing each pixel of image
    RGBTRIPLE ref_image[height][width];

    // copying each pixel to ref_image in mirror position with respect to its current position
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            ref_image[i][width - 1 - j] = image[i][j];      // only changing it's width value as horizontally flipprf
        }
    }

    // storing ref_image values to image pixels
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = ref_image[i][j];
        }
    }

    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // for counting the number of pixels surrounding a particular pixel
    int n_blur_pixel[height][width];

    // initializing temporary variables for storing sum of surrounding pixels
    int temp_Red[height][width];
    int temp_Green[height][width];
    int temp_Blue[height][width];

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            temp_Red[i][j] = 0;
            temp_Green[i][j] = 0;
            temp_Blue[i][j] = 0;
            n_blur_pixel[i][j] = 0;
        }
    }

    // adding pixels surrounding pixel image[i][j]
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // following these loops for adding pixels surrounding the pixel image[i][j]
            for (int k = -1; k <= 1; k++)
            {
                for (int l = -1; l <= 1; l++)
                {
                    // if's for edges, setting pixels outside image to zero
                    if (i + k < 0 || j + l < 0)
                    {
                        continue;
                    }
                    if ((i + k >= height) || (j + l >= width))
                    {
                        break;
                    }

                    // adding each surrounding pixels using box blur algorithm
                    temp_Red[i][j] += image[i + k][j + l].rgbtRed;
                    temp_Green[i][j] += image[i + k][j + l].rgbtGreen;
                    temp_Blue[i][j] += image[i + k][j + l].rgbtBlue;
                    n_blur_pixel[i][j]++;
                }
            }
        }
    }

    // storing new pixel values by dividing values in temp pixel by surrounding pixels
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j].rgbtRed = round(temp_Red[i][j] / (float)n_blur_pixel[i][j]);
            image[i][j].rgbtGreen = round(temp_Green[i][j] / (float)n_blur_pixel[i][j]);
            image[i][j].rgbtBlue = round(temp_Blue[i][j] / (float)n_blur_pixel[i][j]);
        }
    }

    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    int Gx = 0, Gy = 0;
    
    int Gx_Red[height][width];
    int Gx_Green[height][width];
    int Gx_Blue[height][width];

    int Gy_Red[height][width];
    int Gy_Green[height][width];
    int Gy_Blue[height][width];

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            Gx_Red[i][j] = 0;
            Gx_Green[i][j] = 0;
            Gx_Blue[i][j] = 0;

            Gy_Red[i][j] = 0;
            Gy_Green[i][j] = 0;
            Gy_Blue[i][j] = 0;
        }
    }
    
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // following these loops for adding pixels surrounding the pixel image[i][j]
            for (int k = -1; k <= 1; k++)
            {
                for (int l = -1; l <= 1; l++)
                {
                    // if's for edges, setting pixels outside image to zero
                    if (i + k < 0 || j + l < 0)
                    {
                        continue;
                    }
                    if ((i + k >= height) || (j + l >= width))
                    {
                        break;
                    }

                    // Gx and Gy values for each pixel
                    sobel(k, l, &Gx, &Gy);

                    // filling the Gx and Gy matrices values
                    Gx_Red[i][j] += Gx * image[i + k][j + l].rgbtRed;
                    Gx_Green[i][j] += Gx * image[i + k][j + l].rgbtGreen;
                    Gx_Blue[i][j] += Gx * image[i + k][j + l].rgbtBlue;

                    Gy_Red[i][j] += Gy * image[i + k][j + l].rgbtRed;
                    Gy_Green[i][j] += Gy * image[i + k][j + l].rgbtGreen;
                    Gy_Blue[i][j] += Gy * image[i + k][j + l].rgbtBlue;
                }
            }
        }
    }

    // New pixels
    int new_red[height][width];
    int new_green[height][width];
    int new_blue[height][width];
    double test;

    // problem with round, pow or whatever .. problem from line 200
    // Taking the root mean squared values of Gx and Gy 
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            new_red[i][j] = round(sqrt(pow(Gx_Red[i][j], 2) + pow(Gy_Red[i][j], 2)));
            new_green[i][j] = round(sqrt(pow(Gx_Green[i][j], 2) + pow(Gy_Green[i][j], 2)));
            new_blue[i][j] = round(sqrt(pow(Gx_Blue[i][j], 2) + pow(Gy_Blue[i][j], 2)));
        }
    }
    
    // Filling in the new pixels in image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (new_red[i][j] > 255)
            {
                new_red[i][j] = 255;
            }
            if (new_green[i][j] > 255)
            {
                new_green[i][j] = 255;
            }
            if (new_blue[i][j] > 255)
            {
                new_blue[i][j] = 255;
            }
            
            image[i][j].rgbtRed = new_red[i][j];
            image[i][j].rgbtGreen = new_green[i][j];
            image[i][j].rgbtBlue = new_blue[i][j];
        }
    }
    
    return;
}

void sobel(int in_a, int in_b, int *a, int *b)
{
    if (in_a < 0 && in_b < 0)
    {
        *a = -1;
        *b = -1;
    }
    if (in_a < 0 && in_b == 0)
    {
        *a = 0;
        *b = -2;
    }
    if (in_a < 0 && in_b > 0)
    {
        *a = 1;
        *b = -1;
    }
    if (in_a == 0 && in_b < 0)
    {
        *a = -2;
        *b = 0;
    }
    if (in_a == 0 && in_b == 0)
    {
        *a = 0;
        *b = 0;
    }
    if (in_a == 0 && in_b > 0)
    {
        *a = 2;
        *b = 0;
    }
    if (in_a > 0 && in_b < 0)
    {
        *a = -1;
        *b = 1;
    }
    if (in_a > 0 && in_b == 0)
    {
        *a = 0;
        *b = 2;
    }
    if (in_a > 0 && in_b > 0)
    {
        *a = 1;
        *b = 1;
    }

    return;
}