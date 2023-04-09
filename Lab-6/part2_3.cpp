#include <bits/stdc++.h>
#include <iostream>
#include <string>
#include <cmath>
#include <vector>
#include <chrono>
#include <thread>
#include <unistd.h>
#include <sys/wait.h>
#include <atomic>

using namespace std;

//Creating struct structure for reading pixels easily in ppm3

struct pixel
{
    int red;
    int green;
    int blue;
};

struct fileInformation
{
    int height;
    int weight;
    int maxAscii;
    char ppm_version[100];
};

int grey_value(int blue_color, int red_color, int green_color)
{
    int value = blue_color * 0.114;
    value += red_color * 0.299;
    value += green_color * 0.587;
    return value;
}

//This function converts an image to Grayscale by iterating over pixels and then rows and updating pixel matrix
void ConvertGrayscale(char *input, int pipeCreate[2], int pipeCreate2[2], int pid)
{
    if (pid > 0)
    {
        return;
    }

    //open ppm file and read
    int height, weight, maxAscii;
    char ppm_version[100];

    FILE *input_image = fopen(input, "r");

    struct fileInformation head[1];
    fscanf(input_image, "%s%d%d%d", head->ppm_version, &weight, &height, &maxAscii); // reading from file the PPM Version, weightidth, Height and Maximum Ascii value allowed.
    
    head[0].height = height;
    head[0].weight = weight;
    head[0].maxAscii = maxAscii;
    write(pipeCreate2[1], head, sizeof(struct fileInformation));
    
    vector<vector<pixel> > matrix(height, vector<pixel>(weight)); //Vector for reading and storing pixels as a matrix. 
    int red, green, blue;
    for (int i = height - 1; i >= 0; i--)
    {
        for (int j = 0; j <= weight - 1; j++)
        {   //Storing RGB pixel matrix into above created matrix.
            fscanf(input_image, "%d%d%d", &red, &green, &blue);
            matrix[i][j].red = red;
            matrix[i][j].green = green;
            matrix[i][j].blue = blue;
            // for(int i=0; i<head[0].height; i++){
            //     cout << "red color" << red_color;
            //     cout << "blue color" << blue_color;
            //     cout << "green color" << green_color;
            // }
        }
    }
    fclose(input_image);

    struct pixel data_fwd[9];
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
    for (int i = 0; i <= height - 3; i += 3)
    {
        for (int j = 0; j <= weight - 3; j += 3)
        {
            data_fwd[0] = matrix[i][j];
            data_fwd[1] = matrix[i][j + 1];
            data_fwd[2] = matrix[i][j + 2];

            data_fwd[3] = matrix[i + 1][j];
            data_fwd[4] = matrix[i + 1][j + 1];
            data_fwd[5] = matrix[i + 1][j + 2];

            data_fwd[6] = matrix[i + 2][j];
            data_fwd[7] = matrix[i + 2][j + 1];
            data_fwd[8] = matrix[i + 2][j + 2];

            write(pipeCreate[1], data_fwd, sizeof(data_fwd));
        }
    }
    exit(0);
}

//This function does a horizontal blur weighthich makes a photo as if it is in motion.
void HorizontalBlur(char *prevIP, int pipeCreate[2], int pipeCreate2[2], int pid)
{
    if (pid > 0)
        return;
    //for the pipe
    struct pixel getDt[9];

    struct fileInformation head[1];
    read(pipeCreate2[0], head, sizeof(head));

    vector<vector<pixel> > matrix(head[0].height, vector<pixel>(head[0].weight));
    vector<vector<pixel> > tmatrix(head[0].height, vector<pixel>(head[0].weight));
    // cout<<"YES"<<stopl;
    int BLUR_AMOUNT = 50;


    for (int p = 0; p <= head[0].height - 3; p+=3)
    {
        for (int j = 0; j <= head[0].weight - 3; j+=3)
        {
            read(pipeCreate[0], getDt, sizeof(getDt));
            matrix[p][j] = getDt[0];
            matrix[p][j + 1] = getDt[1];
            matrix[p][j + 2] = getDt[2];

            matrix[p + 1][j] = getDt[3];
            matrix[p + 1][j + 1] = getDt[4];
            matrix[p + 1][j + 2] = getDt[5];

            matrix[p + 2][j] = getDt[6];
            matrix[p + 2][j + 1] = getDt[7];
            matrix[p + 2][j + 2] = getDt[8];
            // for(int i=0; i<head[0].height; i++){
                    //     cout << "red color" << red_color;
                    //     cout << "blue color" << blue_color;
                    //     cout << "green color" << green_color;
                    // }
        }
    }
    
    // cout << sizeof(matrix)<<stopl;
    for (int p = 0; p < head[0].height; p++)
    {
        for (int q = 0; q < head[0].weight; q++)
        {
            int red_color = matrix[p][q].red/2;
            int green_color = matrix[p][q].green/2;
            int blue_color = matrix[p][q].blue/2;
            if((head[0].weight - q)<BLUR_AMOUNT)
            {
                // for(int i=0; i<head[0].height; i++){
                    //     cout << "red color" << red_color;
                    //     cout << "blue color" << blue_color;
                    //     cout << "green color" << green_color;
                    // }
                int NEW_BLUR_AMOUNT = head[0].weight-q;
                for(int u = q+1; u < head[0].weight; u++)
                {
                    red_color += matrix[p][u].red * (0.5/NEW_BLUR_AMOUNT);
                    green_color += matrix[p][u].green * (0.5/NEW_BLUR_AMOUNT);
                    blue_color += matrix[p][u].blue * (0.5/NEW_BLUR_AMOUNT);   
                    // for(int i=0; i<head[0].height; i++){
                    //     cout << "red color" << red_color;
                    //     cout << "blue color" << blue_color;
                    //     cout << "green color" << green_color;
                    // }
                }
                matrix[p][q].red = red_color;
                matrix[p][q].blue = blue_color;
                matrix[p][q].green = green_color;
                continue;
            }
            for (int i = 1; i < BLUR_AMOUNT; i++)
            {
                red_color += matrix[p][q+i].red * (0.5/BLUR_AMOUNT);
                green_color += matrix[p][q+i].green * (0.5/BLUR_AMOUNT);
                blue_color += matrix[p][q+i].blue * (0.5/BLUR_AMOUNT);
            }
            // for(int i=0; i<head[0].height; i++){
            //     cout << "red color" << red_color;
            //     cout << "blue color" << blue_color;
            //     cout << "green color" << green_color;
            // }
            matrix[p][q].red = red_color;
            matrix[p][q].blue = blue_color;
            matrix[p][q].green = green_color;
        }
    }
    FILE *opImage = fopen(prevIP, "w");
    fprintf(opImage, "%s\n%d %d\n%d\n", head[0].ppm_version, head[0].weight, head[0].height, head[0].maxAscii); // Printing to the file the PPM Version, weightidth, Height and Maximum Ascii value allowed.
    for (int i = head[0].height - 1; i >= 0; i--)
    {
        for (int j = 0; j <= head[0].weight - 1; j++)
        {   // Printing RGB pixel matrix from above updated image matrix.
            fprintf(opImage, "%d ", matrix[i][j].red);
            fprintf(opImage, "%d ", matrix[i][j].green);
            fprintf(opImage, "%d ", matrix[i][j].blue);
            // for(int i=0; i<head[0].height; i++){
            //     cout << "red color" << red_color;
            //     cout << "blue color" << blue_color;
            //     cout << "green color" << green_color;
            // }
        }
        fprintf(opImage, "\n");
    }
    fclose(opImage);
    exit(0);
}

int main(int argc, char *argv[])
{
    //Creating two pipes for multi-directional communication
    int pipeCreate[2];
    int data;
    data = pipe(pipeCreate);
    if (data == -1)
    {
        perror("pipe");
    }
    int pipeCreate2[2];
    int data2;
    data2 = pipe(pipeCreate2);
    if (data2 == -1)
    {
        perror("pipe");
    }
    
    auto start = chrono::high_resolution_clock::now(); // Starting the clock
    ConvertGrayscale(argv[1], pipeCreate, pipeCreate2, fork()); //T1 function on image
    HorizontalBlur(argv[2], pipeCreate, pipeCreate2, fork()); //T2 function on image

    wait(NULL);
    wait(NULL);

    auto stop = chrono::high_resolution_clock::now(); //Stopping the clock
    auto time_taken = chrono::duration_cast<chrono::microseconds>(stop - start); // Calculating the time taken by T1 and T2.
    cout << "Time: " << time_taken.count() << " microseconds"<< endl;
    
}