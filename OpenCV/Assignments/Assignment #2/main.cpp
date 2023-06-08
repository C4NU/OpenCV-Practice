//
//  main.cpp
//  OpenCV
//
//  Created by 전효재 on 2023/03/23.
//

#include <stdio.h>
#include <opencv2/opencv.hpp>

float getDistance(CvScalar a, CvScalar b)
{
    return (a.val[0] - b.val[0]) * (a.val[0] - b.val[0])
        + (a.val[1] - b.val[1]) * (a.val[1] - b.val[1])
        + (a.val[2] - b.val[2]) * (a.val[2] - b.val[2]);
}

IplImage* ColorCorrection(IplImage *blue, IplImage *green, IplImage *red, CvSize size)
{
    IplImage *dst = cvCreateImage(size, 8, 3); // 결과값
    
    CvScalar color; // 색상
    
    for(int y = 0; y<size.height; y++)
    {
        for(int x = 0; x<size.width; x++)
        {
            color.val[0] = cvGet2D(blue, y, x).val[0];  // B 채널에 greyscale 값 추가
            color.val[1] = cvGet2D(green, y, x).val[0]; // G 채널에 greyscale 값 추가
            color.val[2] = cvGet2D(red, y, x).val[0];   // R 채널에 greyscale 값 추가
            
            cvSet2D(dst, y, x, color);  // 저장
        }
    }
    
    return dst; // 반환
}

IplImage* MergeImage(IplImage *src1, IplImage *src2,CvSize size)
{
    IplImage* result = cvCreateImage(size, 8, 3);

    int w = size.width;
    int h = size.height;

    int min_u = 0;
    int min_v = 0;
    float min_avg = FLT_MAX;

    // 이미지 중심부분 계산
    int startX = w / 4;
    int startY = h / 4;
    int endX = 3 * w / 4;
    int endY = 3 * h / 4;

    // 샘플링 간격 설정
    int step = 2;

    for (int v = -startY; v < endY; v += step)
    {
        for (int u = -startX; u < endX; u += step)
        {
            float sum = 0;
            int count = 0;
            cvSet(result, cvScalar(0, 0, 0));

            for (int y = startY; y < endY; y += step)
            {
                for (int x = startX; x < endX; x += step)
                {
                    if (x + u < startX || x + u >= endX) continue;
                    if (y + v < startY || y + v >= endY) continue;

                    CvScalar f1 = cvGet2D(src1, y, x);
                    CvScalar f2 = cvGet2D(src2, y + v, x + u);
                    sum += getDistance(f1, f2);
                    count++;

                    CvScalar g;
                    for (int k = 0; k < 3; k++)
                    {
                        g.val[k] = (f1.val[k] + f2.val[k]) / 2;
                    }
                    cvSet2D(result, y, x, g);
                }
            }

            float avg = sum / count;
            if (avg < min_avg)
            {
                min_avg = avg;
                min_u = u;
                min_v = v;
            }
        }
    }
    return result;

}

int main()
{
    /*
     처음에 사진을 받고, 사진을 3분할 한 뒤, r, g, b 로 저장함.
     저장한 후 정렬해서 이미지로 저장하고
     그 저장한 이미지에 rgb 채널별로 색상값을 계산해 넣고 결과 출력
     */
    char filename[100] = {0,}; // 파일 경로
    
    printf("Test CV\n");
    printf("Input File Name: ");
    scanf("%s", filename);
    
    IplImage *src = cvLoadImage(filename);
    
    if(src == nullptr) // 파일이 존재하지 않을때
        printf("File Not Exitst!.\n");
    
    else
    {
        // 파일이 존재할 때
        cvShowImage("src", src); // 이미지 출력
        
        CvSize size = cvGetSize(src);                       //  원본 이미지 크기
        CvSize cutSize = cvSize(src->width, src->height/3); //  규격으로 분할할 이미지 크기
        
        // 이미지 3분할
        IplImage *blue = cvCreateImage(cutSize, 8, 3);      //  파란 필터 이미지
        IplImage *green = cvCreateImage(cutSize, 8, 3);;    //  초록 필터 이미지
        IplImage *red = cvCreateImage(cutSize, 8, 3);;      //  붉은 필터 이미지
        
        int colorFlag = 1;  // 1 이면 blue, 2 면 green, 3 이면 red
        int offsetY = 0;    // 이미지 저장할 y 좌표값
        
        for(int y = 0; y<size.height; y++)
        {
            CvScalar color; //  이미지에 저장할 색상값
            
            for(int x = 0; x<src->width; x++)
            {
                color = cvGet2D(src, y, x);
                
                switch(colorFlag)
                {
                    case 1:
                        cvSet2D(blue, offsetY, x, color);   //  blue 변수에 저장
                        break;
                    case 2:
                        cvSet2D(green, offsetY, x, color);  //  green 변수에 저장
                        break;
                    case 3:
                        cvSet2D(red, offsetY, x, color);    //  red 변수에 저장
                        break;
                }
            }
            if(offsetY == cutSize.height - 1)   // 다음 변수로 넘어갈 때
            {
                offsetY = 0;    // 좌표값 초기화
                colorFlag++;    // 다음 색상값으로
            }
            else
                offsetY++;      // 좌표 늘리기
        }
        // 색상 병합
        IplImage *dst = cvCreateImage(cutSize, 8, 3);
        //dst = MergeImage(blue, green, red, cutSize);
        //dst = MergeImage(blue, green, cutSize);
        //dst = MergeImage(dst, red, cutSize);
        dst = ColorCorrection(blue, green, red, cutSize); // 색상값 합치기
        
        cvShowImage("dst", dst); // 결과값 출력
        cvWaitKey();
    }
    return 0;
}
