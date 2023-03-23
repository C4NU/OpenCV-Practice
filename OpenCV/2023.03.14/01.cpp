//
//  01.cpp
//  OpenCV
//
//  Created by 전효재 on 2023/03/14.
//

#include <stdio.h>
#include <opencv2/opencv.hpp>

int main()
{
    IplImage* img = cvLoadImage("/Users/jeonhyojae/Xcode/OpenCV/OpenCV/2023.03.07/sample.jpg");
    int width = img->width;
    int height = img->height;
    
    printf("Width = %d, Height = %d\n", width, height);
    
    cvWaitKey(1); // 키 입력 대기
    
    CvScalar c; // B, G, R 순서
    
    int x = 100;
    int y = 100;
    
    c = cvGet2D(img, y, x); // Get
    
    printf("b = %f g = %f r = %f\n", c.val[0], c.val[1], c.val[2]);
    
    //c1 = cvScalar();
    
    for(int y = 100; y<200; y++)
    {
        for(int x = 100; x<200; x++)
        {
            cvSet2D(img, y, x, c);
        }
    }
    
    cvShowImage("Hello Pixel", img);
    cvWaitKey();
    /*

    
    cvSet2D(img, y, x ,c); // Set
    
    printf("b = %f g = %f r = %f", c.val[0], c.val[1], c.val[2]);
    
    c = cvScalar();
    
    
    for(int y = 100; y < 200; y++)
    {
        for(int x = 100; x<200; x++)
        {
            cvSet2D(img, y, x , c);
        }
    }
    
    cvShowImage("Hello Pixel", img);
    cvWaitKey();
    
    
    for(int y = 0; y<h; y++)
    {
        for(int x = 0; x<width; x++)
        {
            CvScalar C = cvGet2D(img, y, x);
            
            int b = c.val[0];
            int g = c.val[1];
            int r = c.val[2];
            
            
            cvSet2D(img, y, x, cvScalar(255 - b, 255 - g ,255 - r));
        }
    }
    
    for(float a)
        
        for(int y = 0; y<height; y++)
        {
            for(int x = 0; x<width; x++)
            {
                if(x < img2->width && y < img2->height)
                {
                    
                    CvScalar c1 = cvGet2D(img, y, x);
                    CvScalar c2 = cvGet2D(img2, y, x);
                    
                    c.val[0] = c1.val[0] + c2.val[0];
                    c.val[1] = c1.val[1] + c2.val[2];
                    c.val[2] = c1.val[1] + c2.val[2];
                }
                
                cvSet2D(img, y, x ,c);
            }
        }
     */
    return 0;
}
