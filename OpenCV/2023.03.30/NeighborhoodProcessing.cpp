//
//  NeighborhoodProcessing.cpp
//  OpenCV
//
//  Created by 전효재 on 2023/03/30.
//

#include <opencv2/opencv.hpp>


int main()
{
    IplImage* src = cvLoadImage("Resources/Lenna.png");
    CvSize size = cvGetSize(src);
    IplImage* dst = cvCreateImage(size, 8, 3);

    int Kn = 1;

    float kernel[3][3] = { {1, 1, 1},
                           {0, 0, 0},
                           {-1, -1, -1} };

    for (int y = Kn; y < size.height - Kn; y++)
        for (int x = Kn; x < size.width - Kn; x++)
        {
            CvScalar g = cvScalar(0, 0, 0);
            for (int v = -Kn; v <= Kn; v++)
                for (int u = -Kn; u <= Kn; u++)
                {
                    CvScalar f = cvGet2D(src, y + v, x + u);
                    for (int k = 0; k < 3; k++)
                        g.val[k] += kernel[v + Kn][u + Kn] * f.val[k];
                }

            for (int k = 0; k < 3; k++)
                g.val[k] = abs(g.val[k]);
            cvSet2D(dst, y, x, g);
        }


    float kernel1[3][3] = { {1, 0, -1},
                           {1, 0, -1},
                           {1, 0, -1} };

    for (int y = Kn; y < size.height - Kn; y++)
        for (int x = Kn; x < size.width - Kn; x++)
        {
            CvScalar g = cvScalar(0, 0, 0);
            for (int v = -Kn; v <= Kn; v++)
                for (int u = -Kn; u <= Kn; u++)
                {
                    CvScalar f = cvGet2D(src, y + v, x + u);
                    for (int k = 0; k < 3; k++)
                        g.val[k] += kernel1[v + Kn][u + Kn] * f.val[k];
                }
            CvScalar g1 = cvGet2D(dst, y, x);
            for (int k = 0; k < 3; k++)
                g1.val[k] += abs(g.val[k]);
            cvSet2D(dst, y, x, g1);
        }
//    cvSmooth(src, dst, CV_GAUSSIAN, 51);


    cvShowImage("src", src);
    cvShowImage("dst", dst);
    cvWaitKey();
    return 0;
}
