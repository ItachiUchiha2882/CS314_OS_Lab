// #include <bits/stdc++.h>
#include <iostream>
#include <vector>
#include <chrono>
// #include <iostream>
#include <thread>
#include <atomic>
#include <fstream>
#include <string.h>
// #include <vector>
#include <mutex>
#include <semaphore.h>
#include <thread>
using namespace std;
sem_t s;
// Creating struct structure for reading pixels easily in ppm3

int grey_value(int colour_blue, int colour_red, int colour_green)
{
    int value = colour_blue * 0.114;
    value += colour_red * 0.299;
    value += colour_green * 0.587;
    return value;
}
struct pixel
{
    int red;
    int green;
    int blue;
};
// This function converts an image to Grayscale by iterating over pixels and then rows and updating pixel val_matrix
void ConvertGrayscale(int w, int h, vector<vector<pixel>> &val_matrix)
{
    int i = 0;
    while (i < h) // Traverse through arrayofData (given image in height)
    {
        sem_wait(&s);
        int j = 0;
        while (j < w) // Traverse through arrayofData (given image in width)
        {

            // By considering weights for grayscale, appropriate weighted means are taken
            int grey_val = grey_value(val_matrix[i][j].red, val_matrix[i][j].green, val_matrix[i][j].blue);
            val_matrix[i][j].red = grey_val;
            val_matrix[i][j].green = grey_val;
            val_matrix[i][j].blue = grey_val;
            j++;
        }
        sem_post(&s);
        i++;
    }
}

// This function does a horizontal blur which makes a photo as if it is in motion.
void HorizontalBlur(int w, int h, vector<vector<pixel>> &val_matrix)
{
    int BLUR_AMOUNT = 50;
    for (int p = 0; p < h; p++)
    {

        for (int q = 0; q < w; q++)
        {
            sem_wait(&s);
            int colour_red = val_matrix[p][q].red / 2;
            int colour_green = val_matrix[p][q].green / 2;
            int colour_blue = val_matrix[p][q].blue / 2;
            if ((w - q) < BLUR_AMOUNT)
            {
                int NEW_BLUR_AMOUNT = w - q;
                for (int u = q + 1; u < w; u++)
                {
                    colour_red += val_matrix[p][u].red * (0.5 / NEW_BLUR_AMOUNT);
                    colour_green += val_matrix[p][u].green * (0.5 / NEW_BLUR_AMOUNT);
                    colour_blue += val_matrix[p][u].blue * (0.5 / NEW_BLUR_AMOUNT);
                }
                val_matrix[p][q].red = colour_red;
                val_matrix[p][q].blue = colour_blue;
                val_matrix[p][q].green = colour_green;
                sem_post(&s);
                continue;
            }
            for (int it = 1; it < BLUR_AMOUNT; it++)
            {
                colour_red += val_matrix[p][q + it].red * (0.5 / BLUR_AMOUNT);
                colour_green += val_matrix[p][q + it].green * (0.5 / BLUR_AMOUNT);
                colour_blue += val_matrix[p][q + it].blue * (0.5 / BLUR_AMOUNT);
            }
            val_matrix[p][q].red = colour_red;
            val_matrix[p][q].blue = colour_blue;
            val_matrix[p][q].green = colour_green;
            sem_post(&s);
        }
    }
}

int main(int argc, char *argv[])
{
    int w, h, maxAscii;
    char PPM_VERSION[10]; // String to check PPM version
    FILE *inpur = fopen(argv[1], "r");
    fscanf(inpur, "%s%d%d%d", PPM_VERSION, &w, &h, &maxAscii); // reading from file the PPM Version, Width, Height and Maximum Ascii value allowed.
    vector<vector<pixel>> val_matrix(h, vector<pixel>(w));     // Vector for reading and storing pixels as a matrix.
    int red, green, blue;
    int i = h - 1;
    while (i >= 0)
    {
        int j = 0;
        while (j <= w - 1)
        { // Storing RGB pixel val_matrix into above created matrix.
            fscanf(inpur, "%d%d%d", &red, &green, &blue);
            val_matrix[i][j].red = red;
            val_matrix[i][j].green = green;
            val_matrix[i][j].blue = blue;
            j++;
        }
        i--;
    }
    fclose(inpur);
    auto begin = chrono::high_resolution_clock::now(); // Starting the clock
    sem_init(&s, 0, 1);                                // Initialize Semaphore to 1 to use it as a lock.

    thread thread1(ConvertGrayscale, w, h, std::ref(val_matrix)); // Creating Thread for T1
    thread thread2(HorizontalBlur, w, h, std::ref(val_matrix));   // Creating Thread for T2

    thread1.join();
    thread2.join();
    // ConvertGrayscale(w, h, val_matrix);                    // T1 function on image
    //  HorizontalBlur(w, h, val_matrix); //T2 function on image
    auto end = chrono::high_resolution_clock::now();                            // Stopping the clock
    auto taken_time = chrono::duration_cast<chrono::microseconds>(end - begin); // Calculating the time taken by T1 and T2.
    cout << "Time: " << taken_time.count() << " microseconds" << endl;
    FILE *outw = fopen(argv[2], "w");
    fprintf(outw, "%s\n%d %d\n%d\n", PPM_VERSION, w, h, maxAscii); // Printing to the file the PPM Version, Width, Height and Maximum Ascii value allowed.
    i = h - 1;
    while (i >= 0)
    {
        int j = 0;
        while (j <= w - 1)
        { // Printing RGB pixel val_matrix from above updated image matrix.
            fprintf(outw, "%d ", val_matrix[i][j].red);
            fprintf(outw, "%d ", val_matrix[i][j].green);
            fprintf(outw, "%d ", val_matrix[i][j].blue);
            j++;
        }
        fprintf(outw, "\n");
        i--;
    }
    fclose(outw);
    return 0;
}