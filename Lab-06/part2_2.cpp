#include <bits/stdc++.h>
#include <cmath>
#include <vector>
#include <chrono>
#include <thread>
#include <iostream>
#include <semaphore.h>
#include <unistd.h> //for fork()
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <fcntl.h>

using namespace std;

#define SEM_NAME "/s"
#define SEM_PERMS (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP)
#define INITIAL_VALUE 1

//Creating struct structure for reading pixels easily in ppm3

struct pixel
{
    int red;
    int green;
    int blue;
};

//This function converts an image to Grayscale by iterating over pixels and then rows and updating pixel matrix
void ConvertGrayscale(key_t key, int height, int weight, int pid)
{
    if (pid > 0)
        return;

    int shmid = shmget(key, sizeof(struct pixel) * height * weight, 0666 | IPC_CREAT);
    struct pixel *matrix;
    matrix = (struct pixel *)shmat(shmid, NULL, 0);

    //access named semaphore
    sem_t *s = sem_open(SEM_NAME, O_RDWR);
        
    for (int p = 0; p < height; p++)
    {
        sem_wait(s);
        for (int q = 0; q < weight; q++)
        {
            
            struct pixel t;
            t = matrix[p * weight + q];
            int r_c = t.red;
            int g_c = t.green;
            int b_c = t.blue;
            t.red = (b_c * 0.114) + (r_c * 0.299) + (g_c * 0.587);
            t.green = (b_c * 0.114) + (r_c * 0.299) + (g_c * 0.587);
            t.blue = (b_c * 0.114) + (r_c * 0.299) + (g_c * 0.587);
            matrix[p * weight + q] = t;
            
        }
        sem_post(s);
    }
    //cout<<"In gray "<<matrix->red<<endl;
    // shmdt(NULL);
}

//This function does a heightorizontal blur weighthich makes a photo as if it is in motion.
void heightorizontalBlur(key_t key, int height, int weight, int pid)
{
    if (pid > 0)
        return;

    int shmid = shmget(key, sizeof(struct pixel) * height * weight, 0666 | IPC_CREAT);
    struct pixel *matrix;
    matrix = (struct pixel *)shmat(shmid, NULL, 0);

    struct pixel t;

    vector<vector<pixel> > tmatrix(height, vector<pixel>(weight));
    
    sem_t *s = sem_open(SEM_NAME, O_RDWR);
    int BLUR_AMOUNT = 50;
    for (int p = 0; p < height; p++)
    {
        sem_wait(s);
        for (int q = 0; q < weight; q++)
        {
            int r_c = matrix[(p * weight) + q].red/2;
            int g_c = matrix[(p * weight) + q].green/2;
            int b_c = matrix[(p * weight) + q].blue/2;
            if((weight-q)<BLUR_AMOUNT)
            {
                int NEW_BLUR_AMOUNT = weight-q;
                for(int u = q+1; u < weight; u++)
                {
                    r_c += matrix[(p * weight) + u].red * (0.5/NEW_BLUR_AMOUNT);
                    g_c += matrix[(p * weight) + u].green * (0.5/NEW_BLUR_AMOUNT);
                    b_c += matrix[(p * weight) + u].blue * (0.5/NEW_BLUR_AMOUNT);   
                }
                tmatrix[p][q].red = r_c;
                tmatrix[p][q].blue = b_c;
                tmatrix[p][q].green = g_c;
                // for(int i=0; i<head[0].height; i++){
                //     cout << "red color" << red_color;
                //     cout << "blue color" << blue_color;
                //     cout << "green color" << green_color;
                // }
                // matrix[(p * weight) + q] = tmatrix[p][q];
                //sem_post(s);
                continue;
            }
            for (int i = 1; i < BLUR_AMOUNT; i++)
            {
                r_c += matrix[(p * weight) + (q+i)].red * (0.5/BLUR_AMOUNT);
                g_c += matrix[(p * weight) + (q+i)].green * (0.5/BLUR_AMOUNT);
                b_c += matrix[(p * weight) + (q+i)].blue * (0.5/BLUR_AMOUNT);
            }
            tmatrix[p][q].red = r_c;
            tmatrix[p][q].blue = b_c;
            tmatrix[p][q].green = g_c;
            // matrix[(p * weight) + q] = tmatrix[p][q];
            
        }
        sem_post(s);
    }
    for (int i = height-1; i >= 0; i--)
    {
        for (int j = 0; j <= weight - 1; j++)
        {
            matrix[(i * weight) + j] = tmatrix[i][j];
        }
    }
    //cout<<"In blur matrix"<<matrix->red<<endl;
    //cout<<"In blur t"<<tmatrix[-1][-1].red<<endl;
    // shmdt(NULL);
}

int main(int argc, char *argv[])
{
    int weight, height, maxASCII;
    char ppm_version[10];
    FILE *ip_img = fopen(argv[1], "r");
    fscanf(ip_img, "%s%d%d%d", ppm_version, &weight, &height, &maxASCII); // reading from file the PPM Version, weightidth, heighteight and Maximum Ascii value allowed.
    
    struct pixel *matrix;

    key_t key = 0x1234;
    int shmid = shmget(key, sizeof(struct pixel) * (height)*weight, 0666 | IPC_CREAT);

    matrix = (struct pixel *)shmat(shmid, NULL, 0);

    struct pixel t;
    
    //vector<vector<pixel>> matrix(h, vector<pixel>(w)); //Vector for reading and storing pixels as a matrix. 
    int red, green, blue;
    for (int i = height - 1; i >= 0; i--)
    {
        for (int j = 0; j <= weight - 1; j++)
        {   //Storing RGB pixel matrix into above created matrix.
            fscanf(ip_img, "%d%d%d", &red, &green, &blue);
            t.red = red;
            t.green = green;
            t.blue = blue;
            matrix[(i * weight) + j] = t;
            // for(int i=0; i<head[0].height; i++){
                //     cout << "red color" << red_color;
                //     cout << "blue color" << blue_color;
                //     cout << "green color" << green_color;
                // }
        }
    }
    //cout<<"1 "<<matrix->red<<endl;
    fclose(ip_img);

    auto begin = chrono::high_resolution_clock::now(); // Starting the clock

    sem_t *s = sem_open(SEM_NAME, O_CREAT | O_EXCL, SEM_PERMS, INITIAL_VALUE); //named semaphore

    heightorizontalBlur(key, height, weight, fork());
    ConvertGrayscale(key, height, weight, fork());

    wait(NULL);
    wait(NULL);

    auto end = chrono::high_resolution_clock::now(); //Stopping the clock
    auto time_taken = chrono::duration_cast<chrono::microseconds>(end - begin); // Calculating the time taken by T1 and T2.
    cout << "Time: " << time_taken.count() << " microseconds"<< endl;
    FILE *op_img = fopen(argv[2], "w");
    fprintf(op_img, "%s\n%d %d\n%d\n", ppm_version, weight, height, maxASCII); // Printing to the file the PPM Version, weightidth, heighteight and Maximum Ascii value allowed.
    for (int i = height - 1; i >= 0; i--)
    {
        for (int j = 0; j <= weight - 1; j++)
        {   // Printing RGB pixel matrix from above updated image matrix.
            t = matrix[(i * weight) + j];
            fprintf(op_img, "%d ", t.red);
            fprintf(op_img, "%d ", t.green);
            fprintf(op_img, "%d ", t.blue);
            // for(int i=0; i<head[0].height; i++){
                //     cout << "red color" << red_color;
                //     cout << "blue color" << blue_color;
                //     cout << "green color" << green_color;
                // }
        }
        fprintf(op_img, "\n");
    }
    fclose(op_img);
    shmdt(matrix);
    shmctl(shmid,IPC_RMID,NULL);
    return 0;
}