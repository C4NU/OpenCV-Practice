//
//  01.cpp
//  OpenCV
//
//  Created by 전효재 on 2023/03/23.
//

#include <stdio.h>
#include <opencv2/opencv.hpp>

CvScalar GammaCorrection(CvScalar in, float gamma)
{
    CvScalar out;
    
    for(int k =0; k<3; k++)
        out.val[k] = pow(in.val[k]/255.0f, gamma) * 255.0f; // 감마 승(제곱) 한 값 저장
    
    return out;
}

int main()
{
    IplImage *src = cvLoadImage("2023.03.16/Lenna.png");
    CvSize size = cvGetSize(src);
    IplImage *dst = cvCreateImage(size, 8, 3);
    
    float gamma = 1.0f;
    
    while(true)
    {
        
        for(int y = 0; y<size.height; y++)
        {
            for(int x = 0; x<size.width; x++)
            {
                CvScalar f = cvGet2D(src, y, x);
                CvScalar g = GammaCorrection(f, gamma);
                
                cvSet2D(dst, y, x, g);
            }
        }
        cvShowImage("dst", dst);
        
        int key = cvWaitKey();
        
        if(key == '1')          gamma += 0.1f;
        else if(key == '2')     gamma -= 0.1f;
        else if (key == 'q')    break;
        
        printf("gamma = %f \n", gamma);
    }
    return 0;
}
