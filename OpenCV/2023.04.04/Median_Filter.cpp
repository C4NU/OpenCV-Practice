//
//  01.cpp
//  OpenCV
//
//  Created by 전효재 on 2023/04/04.
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

int main()
{
    IplImage *src = cvLoadImage("2023.04.04/saltpepper.jpg");
    CvSize size = cvGetSize(src);
    
    IplImage *dst = cvCreateImage(size, 8, 3);
    
    //  CV_BLUR         :   mean filter
    //  CV_GAUSSIAN     :   gaussian filter
    //  CV_MEDIAN       :   median filter
    

    
    int nK = 25; // 좌 우 픽셀 좌표 offset
    int total = (2 * nK + 1) * (2 * nK + 1);
    
    /*
    1   2   3
    4   5   6
    7   8   9
     */
    //cvSmooth(src, dst, CV_MEDIAN, 2*nK+1);
    
    CvScalar *f = new CvScalar[total];
    
    for(int y = nK; y<size.height-nK; y++)
    {
        for(int x = nK; x<size.width-nK; x++)
        {
            int count = 0;
            for(int v = -nK; v <= nK; v++)
            {
                for(int u = -nK; u <= nK; u++)
                {
                    f[count] = cvGet2D(src, y+v, x+u);
                    count++;
                }
            }
            // sorting
            //  제일 큰 거 찾기 -> 그거 맨 앞으로 옮기기
            //  f[0]: max
            for(int i = 0; i<total-1; i++)
            {
                for(int j = i+1; j<total; j++)
                {
                    if(compare(f[i], f[j]) < 0)
                    {
                        //  swap f[0] with f[i]
                        CvScalar temp = f[i];
                        f[i] = f[j];
                        f[j] = temp;
                    }
                }
            }
            CvScalar g = f[total/2];
            cvSet2D(dst,y, x, g);
        }
    }
    
    cvShowImage("src", src);
    cvShowImage("dst", dst);
    cvWaitKey();
    
    delete[] f;
    
    return 0;
}
