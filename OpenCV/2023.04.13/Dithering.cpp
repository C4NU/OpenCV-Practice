//
//  Dithering.cpp
//  OpenCV
//
//  Created by 전효재 on 2023/04/13.
//
//  가까운 색 찾기

#include <opencv2/opencv.hpp>

float getDiff2(CvScalar a, CvScalar b)
{
    return (a.val[0] - b.val[0]) * (a.val[0] - b.val[0])
        + (a.val[1] - b.val[1]) * (a.val[1] - b.val[1])
        + (a.val[2] - b.val[2]) * (a.val[2] - b.val[2]);
}

int main()
{
    IplImage* src = cvLoadImage("Resources/lenna.png");
    CvSize size = cvGetSize(src);
    IplImage* dst = cvCreateImage(size, 8, 3);

    CvScalar pal[] = { cvScalar(0,0,0),
                       cvScalar(255,255,255),
                        cvScalar(255,0,0),
                        cvScalar(0,255,0),
                        cvScalar(0,0,255),
                        cvScalar(255,255,0),
                        cvScalar(0,255,255),
                        cvScalar(255,0,255),
                     };

    int numPal = 8;

    for(int y=0; y<size.height; y++)
        for (int x = 0; x < size.width; x++)
        {
            CvScalar f = cvGet2D(src, y, x);
            int min_ind = 0;
            float min_diff = 10000000;
            for (int i = 0; i < numPal; i++)
            {
                float diff = getDiff2(f, pal[i]);
                if (diff < min_diff)
                {
                    min_diff = diff;
                    min_ind = i;
                }
            }
            CvScalar g = pal[min_ind];

            cvSet2D(dst, y, x, g);
            
            CvScalar d;
            
            for(int k = 0; k<3; k++)
                d.val[k] = f.val[k] - g.val[k]; // 원본 - 조정값
            
            if(x<size.width-1)
            {
                CvScalar f1 = cvGet2D(src, y, x + 1);
                for(int k = 0; k<3; k++)
                    f1.val[k] += 7 / 16.0f * d.val[k];
                cvSet2D(src, y, x+1, f1);
            }
            if(y >= size.height -1) continue;
            
            if(x > 0)
            {
                CvScalar f2 = cvGet2D(src, y+1, x - 1);
                for(int k = 0; k<3; k++)
                    f2.val[k] += 3 / 16.0f * d.val[k];
                cvSet2D(src, y+1, x-1, f2);
            }
            
            CvScalar f3 = cvGet2D(src, y+1, x);
            for(int k = 0; k<3; k++)
                f3.val[k] += 5 / 16.0f * d.val[k];
            cvSet2D(src, y+1, x, f3);
                
            if(x < size.width - 1)
            {
                CvScalar f4 = cvGet2D(src, y+1, x + 1);
                for(int k = 0; k<3; k++)
                    f4.val[k] += 1 / 16.0f * d.val[k];
                cvSet2D(src, y+1, x+1, f4);
            }
        }

    cvShowImage("src", src);
    cvShowImage("dst", dst);
    cvWaitKey();

    return 0;
}
