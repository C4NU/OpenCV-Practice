//
//  main.cpp
//  OpenCV
//
//  Created by 전효재 on 2023/05/02.
//

#include <stdio.h>
#include <opencv2/opencv.hpp>

void PaintLayer(IplImage *canvas, IplImage *src, int brushSize)
{
    
}

IplImage* Paint(int inputMode, IplImage *src)
{
    int brushSize;          //  붓의 크기
    CvScalar brushColor;    //  붓의 색상
    CvPoint brushPos;       //  붓질의 시작점
    CvPoint brushMovPos;    //  움직이는 경로
    
    CvSize canvasSize = cvGetSize(src);
    
    IplImage *canvas = cvCreateImage(canvasSize, 8, 3);
    
    
    return canvas;
}

int main()
{
    char filename[100] = {0,};  //  파일 경로
    int inputMode = 0;          //  모드 선택 ( 0 -> 원 / 1 -> 붓 )
    
    bool execution = true;
    
    IplImage *src;
    IplImage *dst;
    
    printf("=============================================\n");
    printf("Department of Software, Sejong University\n");
    printf("Multimedia Programming Homework #4\n");
    printf("Painterly Rendering\n");
    printf("=============================================\n");
    
    while(true)
    {
        printf("Input File Path: ");
        scanf("%s", filename);
        
        src = cvLoadImage(filename);
        
        if(src == nullptr) // 파일이 존재하지 않을때
            printf("File not Found!.\n");
        
        else
            break;
    }
    
    while(true)
    {
        printf("Select Drawing Mode (0=circle, 1=stroke):");
        scanf("%d", &inputMode);
        
        if(inputMode == 0 || inputMode == 1)
        {
            dst = Paint(inputMode, src);
            cvShowImage("src", src);
            cvShowImage("dst", dst);
            cvWaitKey();
            break;
        }
        else
            printf("Wrong Drawing Mode!\n");
    }

    return 0;
}
