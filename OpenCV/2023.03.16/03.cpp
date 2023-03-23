//
//  03.cpp
//  OpenCV
//
//  Created by 전효재 on 2023/03/16.
//

#include <stdio.h>
#include <opencv2/opencv.hpp>

int main()
{
    IplImage *src = cvLoadImage("2023.03.16/Lenna.png"); // source
    CvSize size = cvGetSize(src);
    int width = size.width;
    int height = size.height;
    
    IplImage *dst = cvCreateImage(cvGetSize(src), 8, 3); // destination
    

    for(int y = 0; y < height; y++)
    {
        for(int x = 0; x < width; x++)
        {
            CvScalar f = cvGet2D(src, y, x); // Color
            
            float brightness = ((f.val[0] + f.val[1] + f.val[2]) / 3)/255.0f;
            
            CvScalar g = cvScalar(brightness, brightness, brightness); // Gray-Scale
            
            /*
            if(x < width / 2)
                g = f;
             */
            CvScalar g2;
           
            //for(int k = 0; k<3; k++)
            //    g2.val[k] = c.val[k] * brightness;
            
            cvSet2D(dst, y, x, g2); // 붙여넣기
        }
    }
    
    cvShowImage("src", src);
    cvShowImage("dst", dst);
    
    cvWaitKey();
    return 0;
}
