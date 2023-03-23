//
//  01.cpp
//  OpenCV
//
//  Created by 전효재 on 2023/03/16.
//

// 그라데이션 설명

#include <stdio.h>
#include <opencv2/opencv.hpp>

int main()
{
    int width = 600;
    int height = 400;
    
    IplImage* img = cvCreateImage(cvSize(width, height), 8, 3);
    
    cvSet(img, cvScalar(255, 255, 255));
    
    CvScalar c1 = cvScalar(255, 0, 0);
    CvScalar c2= cvScalar(0, 0, 255);
    
    for(int y = 0; y<height; y++)
    {
        for(int x = 0; x < width; x++)
        {
            float nx = float(x) / (width - 1) * 2 - 1; // x: 0 ~ 2 ->
            float ny = float(y) / (height - 1) * 2 - 1;
            
            
            float a = sqrt(nx*nx + ny*ny);//nx; // (1-a)
            
            CvScalar c = cvScalar(0,0,0);
            
            for(int k = 0; k<3; k++)
                c.val[k] = a * c1.val[k] + (1 - a) * c2.val[k];
            
            /*
            float blue = 255.0f * nx;
            float green = 0;
            float red = 255 - 255.0f * nx;
            */
            
            cvSet2D(img, y, x, c);//cvScalar(blue,green,red)); // 순서 꼭 맞춰서
        }
    }
    
    cvShowImage("Image", img);
    
    cvWaitKey();
    
    return 0;
}
