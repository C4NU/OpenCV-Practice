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
    int a;
    
    IplImage *img; // Intel-?? 이미지
    
    a = 10;
    
    img = cvLoadImage("/Users/jeonhyojae/Xcode/OpenCV/OpenCV/2023.03.07/sample.jpg", 0);
    
    if(img == NULL)
    {
        printf("File Not Found\n");
        
        return -1;
    }
    
    printf("Hello World %d\n", a);
    cvShowImage("Hello OpenCV!", img);
        
    cvWaitKey(0);
    
    return 0;
}
