#include "helpers.h"
#include "math.h"

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for(int i=0;i<height;i++)
    {
        for(int j=0;j<width;j++)
        {
            float temp = (image[i][j].rgbtRed+image[i][j].rgbtGreen+image[i][j].rgbtBlue)/3.0;
            int average = round(temp);
            image[i][j].rgbtRed=average;
            image[i][j].rgbtGreen=average;
            image[i][j].rgbtBlue=average;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for(int i=0;i<height;i++)
    {
        for(int j=0,k=width-1;j<=k;(j++,k--))
        {
            RGBTRIPLE temp=image[i][j];
            image[i][j]=image[i][k];
            image[i][k]=temp;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // For the temporary variables to store the average
    BYTE adhocred[height][width];
    BYTE adhocgreen[height][width];
    BYTE adhocblue[height][width];
    for(int i=0;i<height;i++)
    {
        for(int j=0;j<width;j++)
        {
            int count =0;
            int tempgreen=0;
            int tempblue=0;
            int tempred=0;
            for(int l=i-1;l<=i+1;l++)
            {
                for(int k=j-1;k<=j+1;k++)
                {
                    if(k>=0&&k<width&&l>=0&&l<height)
                    {
                        tempgreen+=(int)image[l][k].rgbtGreen;
                        tempblue+=(int)image[l][k].rgbtBlue;
                        tempred+=(int)image[l][k].rgbtRed;
                        count++;
                    }
                }
            }
            // Store the values in a makeshift(temporary variable)
            adhocred[i][j]=round((float)tempred/count);
            adhocgreen[i][j]=round((float)tempgreen/count);
            adhocblue[i][j]=round((float)tempblue/count);
        }
    }
    for(int i=0;i<height;i++)
    {
        for(int j=0;j<width;j++)
        {
            image[i][j].rgbtRed=adhocred[i][j];
            image[i][j].rgbtGreen=adhocgreen[i][j];
            image[i][j].rgbtBlue=adhocblue[i][j];
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    // For the x convuluted matrix
    int m[3][3]={{-1,0,1},
                 {-2,0,2},
                 {-1,0,1}};
    // A makeshift variable to store all the values
    int adhocred[height][width];
    int adhocgreen[height][width];
    int adhocblue[height][width];
    // To iterate across the pixels
    for(int i=0;i<height;i++)
    {
        for(int j=0;j<width;j++)
        {
            // The value of Gx amd Gy to be stored with red=0,green=1,blue=2.
            int Gx[3]={0,0,0};
            int Gy[3]={0,0,0};
            // For the surrounding pixels
            for(int l=i-1,n=0;l<=i+1;l++,n++)
            {
                for(int k=j-1,o=0;k<=j+1;k++,o++)
                {
                    // For the pixels inside the range
                    if(k>=0&&k<width&&l>=0&&l<height)
                    {
                        // For x matrix
                        Gx[0]+=((int)image[l][k].rgbtRed*m[n][o]);
                        Gx[1]+=((int)image[l][k].rgbtGreen*m[n][o]);
                        Gx[2]+=((int)image[l][k].rgbtBlue*m[n][o]);
                        // For y matrix
                        Gy[0]+=((int)image[l][k].rgbtRed*m[o][n]);
                        Gy[1]+=((int)image[l][k].rgbtGreen*m[o][n]);
                        Gy[2]+=((int)image[l][k].rgbtBlue*m[o][n]);
                    }
                }
            }
            adhocred[i][j]=round(sqrt((Gx[0]*Gx[0])+(Gy[0]*Gy[0])));
            adhocgreen[i][j]=round(sqrt((Gx[1]*Gx[1])+(Gy[1]*Gy[1])));
            adhocblue[i][j]=round(sqrt((Gx[2]*Gx[2])+(Gy[2]*Gy[2])));
            // within range of 0 to 255
            adhocred[i][j]=adhocred[i][j]>255? 255:adhocred[i][j];
            adhocgreen[i][j]=adhocgreen[i][j]>255? 255:adhocgreen[i][j];
            adhocblue[i][j]=adhocblue[i][j]>255? 255:adhocblue[i][j];
        }
    }
    for(int i=0;i<height;i++)
    {
        for(int j=0;j<width;j++)
        {
            // For value
            image[i][j].rgbtRed=(BYTE)adhocred[i][j];
            image[i][j].rgbtGreen=(BYTE)adhocgreen[i][j];
            image[i][j].rgbtBlue=(BYTE)adhocblue[i][j];
        }
    }
    return;
}
