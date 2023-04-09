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

/*
 void mean_filter(IplImage* src, IplImage* dst, int k)
 {
 CvSize size = cvGetSize(src);
 
 int h = src->height;
 int w = src->width;
 int step = src->widthStep;
 int channels = src->nChannels;
 uchar *data = (uchar *)src->imageData;
 uchar *result_data = (uchar *)dst->imageData;
 int r = size/2;
 
 // create summed area table
 int *integral_image = new int[(h+1)*(w+1)];
 memset(integral_image, 0, sizeof(int)*(h+1)*(w+1));
 for(int i=1; i<=h; i++)
 for(int j=1; j<=w; j++)
 for(int k=0; k<channels; k++)
 integral_image[i*(w+1)+j] += data[(i-1)*step + (j-1)*channels + k];
 for(int i=1; i<=h; i++)
 for(int j=1; j<=w; j++)
 for(int k=0; k<channels; k++)
 integral_image[i*(w+1)+j] += integral_image[(i-1)*(w+1)+j] + integral_image[i*(w+1)+j-1] - integral_image[(i-1)*(w+1)+j-1];
 
 // apply filter
 for(int i=r; i<h-r; i++)
 for(int j=r; j<w-r; j++)
 for(int k=0; k<channels; k++)
 result_data[i*step+j*channels+k] = (uchar)((integral_image[(i+r+1)*(w+1)+j+r+1] - integral_image[(i-r)*(w+1)+j+r+1] - integral_image[(i+r+1)*(w+1)+j-r] + integral_image[(i-r)*(w+1)+j-r])/((2*r+1)*(2*r+1)));
 }
 }*/
void myFastestMeanFilter(IplImage *src, IplImage *dst, int k)
{
    CvSize size = cvGetSize(src);   // src 파일의 크기를 저장
    
    // SAT (Summed Area Table)을 생성
    // 이미지에 필터를 씌움
    int h = src->height;
    int w = src->width;
    //int r = size/2;
    
    // create summed area table
    //int *SAT = new int[][];
    int *integral_image = new int[(h+1)*(w+1)];
    memset(integral_image, 0, sizeof(int)*(h+1)*(w+1));
    for(int i=1; i<=h; i++) {
        for(int j=1; j<=w; j++) {
            CvScalar scalar = cvGet2D(src, i-1, j-1);
            int sum = scalar.val[0];
            if (src->nChannels == 3) {
                sum += scalar.val[1] + scalar.val[2];
            }
            integral_image[i*(w+1)+j] += sum;
        }
    }
    for(int i=1; i<=h; i++) {
        for(int j=1; j<=w; j++) {
            integral_image[i*(w+1)+j] += integral_image[(i-1)*(w+1)+j] + integral_image[i*(w+1)+j-1] - integral_image[(i-1)*(w+1)+j-1];
        }
    }
    
    // apply filter
    for(int i=r; i<h-r; i++) {
        for(int j=r; j<w-r; j++) {
            CvScalar scalar = cvScalarAll(0);
            int count = (2*r + 1) * (2*r + 1);
            for(int k = 0; k < count; k++) {
                int ii = k / (2*r+1) - r;
                int jj = k % (2*r+1) - r;
                CvScalar pixel = cvGet2D(src, i+ii, j+jj);
                scalar.val[0] += pixel.val[0];
                if (src->nChannels == 3) {
                    scalar.val[1] += pixel.val[1];
                    scalar.val[2] += pixel.val[2];
                }
            }
            scalar.val[0] /= count;
            if (src->nChannels == 3) {
                scalar.val[1] /= count;
                scalar.val[2] /= count;
            }
            cvSet2D(dst, i, j, scalar);
        }
    }
    
    cvShowImage("src", src);
    cvShowImage("dst", dst);
    cvWaitKey();
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
        
        CvSize size = cvGetSize(src);                       //  원본 이미지 크기
        IplImage *dst = cvCreateImage(size, 8, 3);          //  결과 이미지 변수
        
        myFastestMeanFilter(src, dst, kernelSize);
    }
    
    return 0;
}
