//
//  main.cpp
//  OpenCV
//
//  Created by 전효재 on 2023/04/06.
//

#include <stdio.h>
#include <opencv2/opencv.hpp>

/*
 *  IplImage src:   소스
 *  IplImage dst:   결과값
 *  int k:          이웃의 범위를 정의하는 숫자. 즉, 커널의 한 변의 크기는 (2k+1)이 된다. 예) k=1 일 경우 커널의 사이즈는 3x3이 됨.
 */

void myFastestMeanFilter(IplImage *src, IplImage *dst, int k)
{
    //  소스 이미지의 사이즈 저장
    CvSize size = cvGetSize(src);
    
    //  Summed Area Table로 이용할 2차원 배열을 동적할당
    CvScalar **SAT = new CvScalar*[size.height];
    
    for (int i = 0; i < size.height; i++)
        SAT[i] = new CvScalar[size.width];
    
    //  커널 사이즈의 크기 계산
    int total = (2*k+1) * (2*k+1);
    
    //  Summed Area Table
    /*
     1. 먼저 0,0 좌표를 저장합니다.
     2. 열의 값을 누적하여 저장합니다.
     3. 행의 값을 누적하여 저장합니다.
     4. 나머지 좌표의 연산을 수행합니다.
     */
    
    //  0,0 좌표의 Scalar값 저장
    for(int k = 0; k<3; k++)
        SAT[0][0].val[k] = cvGet2D(src, 0, 0).val[k];
    
    //  X 축 (열) 저장
    for(int x = 1; x<size.width; x++)
    {
        CvScalar f = cvGet2D(src, 0, x);
        
        for(int i = 0; i<3; i++)
            SAT[0][x].val[i] = f.val[i] + SAT[0][x-1].val[i];
    }
    
    //  Y 축 (행) 저장
    for(int y = 1; y<size.height; y++)
    {
        CvScalar f = cvGet2D(src, y, 0);
        
        for(int i = 0; i<3; i++)
            SAT[y][0].val[i] = f.val[i] + SAT[y-1][0].val[i];
    }
    
    //  나머지 좌표 저장
    for (int y = 1; y < size.height; y++)
    {
        for (int x = 1; x < size.width; x++)
        {
            CvScalar f = cvGet2D(src, y, x);
            
            for(int i =0; i<3; i++)
                SAT[y][x].val[i] = f.val[i]
                                   + SAT[y][x-1].val[i]
                                   + SAT[y-1][x].val[i]
                                   - SAT[y-1][x-1].val[i];
        }
    }
    
    //  Mean Filter
    for (int y = k; y < size.height-k; y++)
    {
        for (int x = k; x < size.width-k; x++)
        {
            CvScalar g = cvScalar(0, 0, 0);
            
            for(int i = 0; i<3; i++)
                g.val[i] =
                (SAT[y+k][x+k].val[i]
                 - SAT[y+k][x-k].val[i]
                 - SAT[y-k][x+k].val[i]
                 + SAT[y-k][x-k].val[i])
                / total;
            
            cvSet2D(dst, y, x, g);
        }
    }
    
    cvShowImage("dst", dst);
    
    //  동적할당 해제
    for (int i = 0; i < size.height; i++)
        delete SAT[i];
    delete[] SAT;
}

int main()
{
    char filename[100] = {0,}; // 파일 경로
    int kernelSize;
    
    printf("=============================================\n");
    printf("Software Department, Sejong University\n");
    printf("Multimedia Programming Class\n");
    printf("Homework: Fastest Mean Filter\n");
    printf("=============================================\n\n");
    
    printf("Input File Name: ");
    scanf("%s", filename);
    
    IplImage *src = cvLoadImage(filename);
    
    if(src == nullptr) // 파일이 존재하지 않을때
        printf("File Not Exitst!.\n");
    
    else
    {
        printf("Input K: ");
        scanf("%d", &kernelSize);
        
        IplImage *dst = cvCreateImage(cvGetSize(src), 8, 3);          //  결과 이미지 변수
        
        cvShowImage("src", src);    //  원본 이미지 출력
        
        myFastestMeanFilter(src, dst, kernelSize);  //  Mean Filter 작업
        
        cvShowImage("dst", dst);    //  편집된 이미지 출력
        
        printf("Mean Filter Complete.\n");
        
        cvWaitKey();
    }
    
    return 0;
}
