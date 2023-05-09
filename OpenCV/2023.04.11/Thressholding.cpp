//
//  01.cpp
//  OpenCV
//
//  Created by 전효재 on 2023/04/11.
//

#include <stdio.h>
#include <opencv2/opencv.hpp>

int main()
{
    IplImage *src = cvLoadImage("Resources/Lenna.png");
    CvSize size = cvGetSize(src);
    
    IplImage *dst = cvCreateImage(size, 8, 3);
    
    for(int y = 0; y<size.height; y++)
    {
        for(int x = 0; x<size.width; x++)
        {
            CvScalar f = cvGet2D(src, y, x);
            
            int brightness = (f.val[0] + f.val[1] + f.val[2])/3;
            
            CvScalar g = cvScalar(0,0,0);
            
            if(brightness > 128)    //
            {
                g = cvScalar(255,255,255);
            }
            
            cvSet2D(dst, y, x ,g);
        }
    }
    
    cvShowImage("src", src);
    cvShowImage("dst", dst);
    cvWaitKey();
    
    return 0;
}
