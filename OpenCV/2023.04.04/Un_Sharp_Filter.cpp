//
//  02.cpp
//  OpenCV
//
//  Created by 전효재 on 2023/04/04.
//

//  Un-Sharp Filter

#include <stdio.h>
#include <opencv2/opencv.hpp>

int main()
{
    IplImage *src  = cvLoadImage("2023.03.16/Lenna.png");
    CvSize size = cvGetSize(src);
    
    IplImage *dst = cvCreateImage(size, 8, 3);
    IplImage *blur = cvCreateImage(size, 8, 3);
    IplImage *diff = cvCreateImage(size, 8, 3);
    
    //  src - blur = diff
    //  blur + diff = src
    
    cvSmooth(src, blur, CV_GAUSSIAN, 41);
    
    for(int y = 0; y<size.height; y++)
    {
        for(int x = 0; x<size.width; x++)
        {
            CvScalar f = cvGet2D(src, y, x);
            CvScalar b = cvGet2D(blur, y, x);
            
            CvScalar d;
            for(int k = 0; k<3; k++)
                d.val[k] = f.val[k] - b.val[k] + 128;
            
            cvSet2D(diff, y, x, d);
        }
    }
    
    
    for(float alpha = 0.0f; alpha < 2.0f; alpha+=0.1f)
    {
        for(int y = 0; y<size.height; y++)
        {
            for(int x = 0; x<size.width; x++)
            {
                CvScalar b = cvGet2D(blur, y, x);
                CvScalar d= cvGet2D(diff, y, x);
                
                CvScalar g;
                
                for(int k = 0; k<3; k++)
                    g.val[k] = b.val[k] + alpha*(d.val[k] - 128);
                
                cvSet2D(dst, y, x, g);
            }
        }
        cvShowImage("src", src);
        cvShowImage("dst", dst);
        cvShowImage("blur", blur);
        cvShowImage("diff", diff);
        
        cvWaitKey();
        
    }
    

    return 0;
}
