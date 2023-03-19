//
//  main.cpp
//  OpenCV
//
//  Created by 전효재 on 2023/03/16.
//

#include <stdio.h>
#include <opencv2/opencv.hpp>

IplImage* Flip(IplImage *src) // 1번 사진
{
    CvSize size = cvGetSize(src);
    int width = size.width;
    int height = size.height;
    
    IplImage *dst = cvCreateImage(cvGetSize(src), 8, 3); // destination
    
    for(int y = height-1, y2 = 0; y >= 0, y2 < height; y--, y2++)
    {
        for(int x = 0; x < width; x++)
        {
            CvScalar f = cvGet2D(src, y, x);
            
            cvSet2D(dst, y2, x, f);
        }
    }
    
    return dst;
}

IplImage* Half(IplImage *src) // 2번 사진
{
    CvSize size = cvGetSize(src);
    int width = size.width;
    int height = size.height;
    
    IplImage *dst = cvCreateImage(cvGetSize(src), 8, 3); // destination
    
    for(int y = 0; y<height; y++)
    {
        for(int x = 0; x < width; x++)
        {
            if(x < width/2)
            {
                CvScalar f = cvGet2D(src, y, x + width/2);
                cvSet2D(dst, y, x, f);
            }
            else
            {
                CvScalar f = cvGet2D(src, y, x - width/2);
                cvSet2D(dst, y, x, f);
            }
        }
    }
    
    return dst;
}

IplImage* Diamond(IplImage *src) // 3번 사진
{
    CvSize size = cvGetSize(src);
    int width = size.width;
    int height = size.height;
    
    IplImage *dst = cvCreateImage(cvGetSize(src), 8, 3); // destination
    
    for(int y = 0; y<height; y++)
    {
        for(int x = 0; x < width; x++)
        {
            float nx = float(x) / (width - 1) * 2 - 1; // -1 ≤ nx ≤ 1
            float ny = float(y) / (height - 1) * 2 - 1;// -1 ≤ ny ≤ 1
            
            float a = abs(nx) + abs(ny);
            
            CvScalar color = cvGet2D(src, y, x);
            float brightness = ((color.val[0] + color.val[1] + color.val[2]) / 3);
            CvScalar grayScale = cvScalar(brightness, brightness, brightness);
            
            
            if(a > 1.0f)
                cvSet2D(dst, y, x, grayScale);
            else
                cvSet2D(dst, y, x, color);
        }
    }
    
    return dst;
}

IplImage* Round(IplImage *src) // 4번 사진
{
    CvSize size = cvGetSize(src);
    int width = size.width;
    int height = size.height;
    
    IplImage *dst = cvCreateImage(cvGetSize(src), 8, 3); // destination
    
    for(int y = 0; y<height; y++)
    {
        for(int x = 0; x < width; x++)
        {
            float nx = float(x) / (width - 1) * 2 - 1; // -1 ≤ nx ≤ 1
            float ny = float(y) / (height - 1) * 2 - 1;// -1 ≤ ny ≤ 1
            
            float a = sqrt(nx*nx + ny*ny);
            
            CvScalar color = cvGet2D(src, y, x);
            float brightness = ((color.val[0] + color.val[1] + color.val[2]) / 3);
            CvScalar grayScale = cvScalar(brightness, brightness, brightness);
            
            
            if(a > 1.0f)
                cvSet2D(dst, y, x, grayScale);
            else
                cvSet2D(dst, y, x, color);
        }
    }
    
    return dst;
}

IplImage* Wave(IplImage *src) // 5반 사진
{
    CvSize size = cvGetSize(src);
    int width = size.width;
    int height = size.height;
    
    IplImage *dst = cvCreateImage(cvGetSize(src), 8, 3); // destination
    CvScalar black = cvScalar(0,0,0);
    
    for(int y = 0; y<height; y++)
    {
        for(int x = 0; x < width; x++)
        {
            float nx = float(x) / (width - 1) * 2 - 1; // -1 ≤ nx ≤ 1
            float ny = float(y) / (height - 1) * 2 - 1;// -1 ≤ ny ≤ 1
            
            float a = sqrt(nx*nx + ny*ny);
            
            CvScalar color = cvGet2D(src, y, x);
            
            if(a >= 0 && fmod(a, 0.2) < 0.1)
                cvSet2D(dst, y, x, color);
            else
                cvSet2D(dst, y, x, black);
        }
    }
    return dst;
}

IplImage* Checkerboard(IplImage *src)
{
    CvSize size = cvGetSize(src);
    float width = size.width;
    float height = size.height;
    
    IplImage *dst = cvCreateImage(cvGetSize(src), 8, 3); // destination
    
    int flag = 1; // 0 이면 컬러, 1 이면 회색
    
    // Width:   300
    // Height:  227
    float offsetX = width / 10;
    float offsetY = height / 10;
    
    CvScalar black = cvScalar(0, 0, 0); // black
    
    for(int y = 0; y < height; y++)
    {
        for(int x = 0; x < width; x++)
        {
            CvScalar color = cvScalar(cvGet2D(src, y, x));
            cvSet2D(dst, y, x, flag == 0 ? color : black);
            if(x == int(offsetX))
            {
                offsetX = offsetX + width / 10;
                flag = !flag;
            }
        }
        offsetX = width / 10;
        flag = !flag;
        
        if(y >= offsetY)
        {
            offsetY = offsetY + height / 10;
            flag = !flag;
        }
    }
    
    /*
     for(float y = 0; y < height; y++)
     {
     for(float x = 0; x < width; x++)
     {
     CvScalar color = cvGet2D(src, y, x); // Color
     
     if(x >= posXOrigin + (width / 10))
     {
     if(colorFlag == 0)
     colorFlag = 1;
     else
     colorFlag = 0;
     
     posXOrigin = x;
     }
     
     if(colorFlag == 0)
     cvSet2D(dst, y, x, color);
     else
     cvSet2D(dst, y, x, black);
     }
     
     
     // Y 좌표가 다를때
     if(y == posYOrigin + (height / 10))
     {
     if(colorFlag == 0)
     colorFlag = 1;
     else
     colorFlag = 0;
     
     posYOrigin += (height / 10);
     }
     posXOrigin = 0;
     
     }*/
    return dst;
}

int main()
{
    IplImage *src = cvLoadImage("Assignments/Assignment #1/sejong_small.jpg"); // source
    
    IplImage *dst01 = Flip(src);
    IplImage *dst02 = Half(src);
    IplImage *dst03 = Diamond(src);
    IplImage *dst04 = Round(src);
    IplImage *dst05 = Wave(src);
    IplImage *dst06 = Checkerboard(src);
    
    cvShowImage("Flip", dst01);
    cvShowImage("Half", dst02);
    cvShowImage("Diamond", dst03);
    cvShowImage("Round", dst04);
    cvShowImage("Wave", dst05);
    cvShowImage("CheckerBoard", dst06);
    
    cvWaitKey();
    
    return 0;
}
