//
//  Homework2_Practice.cpp
//  OpenCV
//
//  Created by 전효재 on 2023/03/28.
//

#include <opencv2/opencv.hpp>

float getDistance(CvScalar a, CvScalar b)
{
    return (a.val[0] - b.val[0]) * (a.val[0] - b.val[0])
        + (a.val[1] - b.val[1]) * (a.val[1] - b.val[1])
        + (a.val[2] - b.val[2]) * (a.val[2] - b.val[2]);
}


int main()
{
    IplImage* src1 = cvLoadImage("Resources/hw2_1.jpg");
    IplImage* src2 = cvLoadImage("Resources/hw2_2.jpg");
    CvSize size = cvGetSize(src1);
    IplImage* dst = cvCreateImage(size, 8, 3);

    int w = size.width;
    int h = size.height;

    int min_u = 0;
    int min_v = 0;
    float min_avg = FLT_MAX;
    
    for(int v = -h/4; v<h/4; v++)
        for (int u = -w / 4; u < w / 4; u++)
        {
            float sum = 0;
            int count = 0;
            cvSet(dst, cvScalar(0, 0, 0));

            for (int y = 0; y < h; y++)
                for (int x = 0; x < w; x++)
                {
                    if (x + u<0 || x + u>w - 1) continue;
                    if (y + v<0 || y + v > h - 1) continue;
                    CvScalar f1 = cvGet2D(src1, y, x);
                    CvScalar f2 = cvGet2D(src2, y + v, x + u);
                    sum += getDistance(f1, f2);
                    count++;
                    CvScalar g;
                    for(int k =0; k<3; k++)
                        g.val[k] = (f1.val[k] + f2.val[k])/2;
                    cvSet2D(dst, y, x, g);
                }
            
            float avg = sum / count;

            if (avg < min_avg)
            {
                min_avg = avg;
                min_u = u;
                min_v = v;
                printf("u = %d, v = %d, avg error = %f \n", u, v, avg);
                cvShowImage("dst", dst);
                cvWaitKey(1);
            }
            
        }
    cvShowImage("dst", dst);
    cvWaitKey();


    return 0;
}
