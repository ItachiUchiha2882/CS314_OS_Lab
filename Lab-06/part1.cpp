#include <bits/stdc++.h>
#include <iostream>
#include <vector>
#include <chrono>

using namespace std;

// Creating struct structure for reading pixels easily in ppm3

struct pixel
{
    int red;
    int green;
    int blue;
};

int grey_value(int blue_color, int red_color, int green_color)
{
    int value = blue_color * 0.114;
    value += red_color * 0.299;
    value += green_color * 0.587;
    return value;
}

// This function converts an image to Grayscale by iterating over pixels and then rows and updating pixel matrix
void ConvertGrayscale(int weight, int height, vector<vector<pixel> > &matrix)
{
    for (int p = 0; p < height; p++)
    {
        for (int q = 0; q < weight; q++)
        {
            int grey_val = grey_value(matrix[p][q].red, matrix[p][q].green, matrix[p][q].blue);
            matrix[p][q].red = grey_val;
            matrix[p][q].green = grey_val;
            matrix[p][q].blue = grey_val;
        }
    }
}

// This function does a heightorizontal blur weighthich makes a photo as if it is in motion.
void heightorizontalBlur(int weight, int height, vector<vector<pixel> > &matrix)
{
    int BLUR_AMOUNT = 50;
    for (int p = 0; p < height; p++)
    {
        for (int q = 0; q < weight; q++)
        {
            int red_color = matrix[p][q].red / 2;
            int green_color = matrix[p][q].green / 2;
            int blue_color = matrix[p][q].blue / 2;
            if ((weight - q) < BLUR_AMOUNT)
            {
                int NEW_BLUR_AMOUNT = weight - q;
                for (int u = q + 1; u < weight; u++)
                {
                    red_color += matrix[p][u].red * (0.5 / NEW_BLUR_AMOUNT);
                    green_color += matrix[p][u].green * (0.5 / NEW_BLUR_AMOUNT);
                    blue_color += matrix[p][u].blue * (0.5 / NEW_BLUR_AMOUNT);
                }
                matrix[p][q].red = red_color;
                matrix[p][q].blue = blue_color;
                matrix[p][q].green = green_color;
                // for(int i=0; i<head[0].height; i++){
                //     cout << "red color" << red_color;
                //     cout << "blue color" << blue_color;
                //     cout << "green color" << green_color;
                // }
                continue;
            }
            for (int i = 1; i < BLUR_AMOUNT; i++)
            {
                red_color += matrix[p][q + i].red * (0.5 / BLUR_AMOUNT);
                green_color += matrix[p][q + i].green * (0.5 / BLUR_AMOUNT);
                blue_color += matrix[p][q + i].blue * (0.5 / BLUR_AMOUNT);
            }
            matrix[p][q].red = red_color;
            matrix[p][q].blue = blue_color;
            matrix[p][q].green = green_color;
        }
    }
}

int main(int argc, char *argv[])
{
    int weight, height, max_ASCII;
    char ppm_version[10];
    FILE *ip_img = fopen(argv[1], "r");
    fscanf(ip_img, "%s%d%d%d", ppm_version, &weight, &height, &max_ASCII); // reading from file the PPM Version, weightidth, heighteight and Maximum Ascii value allowed.
    vector<vector<pixel> > matrix(height, vector<pixel>(weight));               // Vector for reading and storing pixels as a matrix.
    int red, green, blue;
    for (int i = height - 1; i >= 0; i--)
    {
        for (int j = 0; j <= weight - 1; j++)
        { // Storing RGB pixel matrix into above created matrix.
            fscanf(ip_img, "%d%d%d", &red, &green, &blue);
            matrix[i][j].red = red;
            matrix[i][j].green = green;
            matrix[i][j].blue = blue;
        }
    }
    fclose(ip_img);
    auto start = chrono::high_resolution_clock::now();                          // Starting the clock
    ConvertGrayscale(weight, height, matrix);                                   // T1 function on image
    heightorizontalBlur(weight, height, matrix);                                // T2 function on image
    auto stop = chrono::high_resolution_clock::now();                            // Stopping the clock
    auto time_taken = chrono::duration_cast<chrono::microseconds>(stop - start); // Calculating the time taken by T1 and T2.
    cout << "Time: " << time_taken.count() << " microseconds" << endl;
    FILE *op_img = fopen(argv[2], "w");
    fprintf(op_img, "%s\n%d %d\n%d\n", ppm_version, weight, height, max_ASCII); // Printing to the file the PPM Version, width, heighteight and Maximum Ascii value allowed.
    for (int i = height - 1; i >= 0; i--)
    {
        for (int j = 0; j <= weight - 1; j++)
        { // Printing RGB pixel matrix from above updated image matrix.
            fprintf(op_img, "%d ", matrix[i][j].red);
            fprintf(op_img, "%d ", matrix[i][j].green);
            fprintf(op_img, "%d ", matrix[i][j].blue);
        }
        fprintf(op_img, "\n");
    }
    fclose(op_img);
    return 0;
}