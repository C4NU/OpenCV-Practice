//
//  FastMedianFilter.cpp
//  OpenCV
//
//  Created by 전효재 on 2023/04/06.
//

#include <stdio.h>
#include <opencv2/opencv.hpp>

int compare(CvScalar a, CvScalar b)
{
    //  0 : ==
    //  >0 : a>b
    //  <0 : a<b
    
    int a_brightness = a.val[0] + a.val[1] + a.val[2];
    int b_brightness = b.val[0] + b.val[1] + b.val[2];
    
    return a_brightness - b_brightness;
}

int Kernel(int K)
{
    return (2*K+1) * (2*K+1);
}

int main()
{
    IplImage *src = cvLoadImage("Resources/Lenna.png");
    CvSize size = cvGetSize(src);
    
    IplImage *mid = cvCreateImage(size, 8, 3);
    IplImage *dst = cvCreateImage(size, 8, 3);
    
    int K = 1;
    int total = Kernel(K);
    
    for(int y = K; y<size.height-K; y++)
    {
        for(int x = K; x<size.width-K; x++)
        {
            /*
             * 최적화가 가능한 구간 반복문
             */
            CvScalar g = cvScalar(0,0,0);
            for(int v = -K; v<= K; v++)
            {
                CvScalar f = cvGet2D(src, y+v, x);
                g.val[0] += f.val[0] / total;
                g.val[1] += f.val[1] / total;
                g.val[2] += f.val[2] / total;
            }
            cvSet2D(mid, y, x, g);
        }
    }
    
    
    cvShowImage("src", src);
    cvShowImage("mid", mid);
    //cvShowImage("dst", dst);
    cvWaitKey();
    
    for(int y = K; y<size.height-K; y++)
    {
        for(int x = K; x<size.width-K; x++)
        {
            /*
             * 최적화가 가능한 구간 반복문
             */
            CvScalar g = cvScalar(0,0,0);
            for(int u = -K; u<= K; u++)
            {
                CvScalar f = cvGet2D(mid, y, x+u);
                g.val[0] += f.val[0] / total;
                g.val[1] += f.val[1] / total;
                g.val[2] += f.val[2] / total;
            }
            cvSet2D(dst, y, x, g);
        }
    }
    
    cvShowImage("dst", dst);
    cvWaitKey();
    
    return 0;
}
