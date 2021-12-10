#include "helpers.h"
#include <math.h>

// Struct for storing pixels temporarily
typedef struct
{
    int16_t Red;
    int16_t Green;
    int16_t Blue;
}
TMP_PIXEL;

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
    // initializing temporary variables for storing sum of surrounding pixels
    TMP_PIXEL temp;
    temp = (TMP_PIXEL)
    {
        0, 0, 0
    };

    // copy of image
    RGBTRIPLE blur[height][width];

    // n_cells is the number of surrounding cells
    BYTE n_cells = 0;

    // adding surrounding pixel to temp then storing it in image
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
                    temp.Red += image[i + k][j + l].rgbtRed;
                    temp.Green += image[i + k][j + l].rgbtGreen;
                    temp.Blue += image[i + k][j + l].rgbtBlue;
                    n_cells++;
                }
            }

            // Storing averaged pixel values in temporary array
            blur[i][j].rgbtRed = round(temp.Red / (float)n_cells);
            blur[i][j].rgbtGreen = round(temp.Green / (float)n_cells);
            blur[i][j].rgbtBlue = round(temp.Blue / (float)n_cells);

            // Resetting these variables again to zero for next iteration
            temp = (TMP_PIXEL)
            {
                0, 0, 0
            };
            n_cells = 0;
        }
    }

    // Copying pixels from temporary array 'blur' to 'image'
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j].rgbtRed = blur[i][j].rgbtRed;
            image[i][j].rgbtGreen = blur[i][j].rgbtGreen;
            image[i][j].rgbtBlue = blur[i][j].rgbtBlue;
        }
    }

    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    // for each values from Gx and Gy matrices
    int Gx = 0, Gy = 0;

    // for temporarily storing Sobel sum of each pixel
    TMP_PIXEL Sum_Gx, Sum_Gy;

    // initializing Sobel sum variables to 0
    Sum_Gx = (TMP_PIXEL)
    {
        0, 0, 0
    };
    Sum_Gy = (TMP_PIXEL)
    {
        0, 0, 0
    };

    // New pixels stored in this temporary array
    TMP_PIXEL edge[height][width];

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
                    Sum_Gx.Red += Gx * image[i + k][j + l].rgbtRed;
                    Sum_Gx.Green += Gx * image[i + k][j + l].rgbtGreen;
                    Sum_Gx.Blue += Gx * image[i + k][j + l].rgbtBlue;

                    Sum_Gy.Red += Gy * image[i + k][j + l].rgbtRed;
                    Sum_Gy.Green += Gy * image[i + k][j + l].rgbtGreen;
                    Sum_Gy.Blue += Gy * image[i + k][j + l].rgbtBlue;
                }
            }

            // Storing the root mean squared values of Sum_Gx and Sum_Gy in temporary array
            edge[i][j].Red = round(sqrt(pow(Sum_Gx.Red, 2) + pow(Sum_Gy.Red, 2)));
            edge[i][j].Green = round(sqrt(pow(Sum_Gx.Green, 2) + pow(Sum_Gy.Green, 2)));
            edge[i][j].Blue = round(sqrt(pow(Sum_Gx.Blue, 2) + pow(Sum_Gy.Blue, 2)));

            // Reseting the Sum_Gx and Sum_Gy to zero
            Sum_Gx = (TMP_PIXEL)
            {
                0, 0, 0
            };
            Sum_Gy = (TMP_PIXEL)
            {
                0, 0, 0
            };
        }
    }

    // Copying pixels from temporary array 'edge' to 'image'
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (edge[i][j].Red > 255)
            {
                edge[i][j].Red = 255;
            }
            if (edge[i][j].Green > 255)
            {
                edge[i][j].Green = 255;
            }
            if (edge[i][j].Blue > 255)
            {
                edge[i][j].Blue = 255;
            }

            image[i][j].rgbtRed = edge[i][j].Red;
            image[i][j].rgbtGreen = edge[i][j].Green;
            image[i][j].rgbtBlue = edge[i][j].Blue;
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