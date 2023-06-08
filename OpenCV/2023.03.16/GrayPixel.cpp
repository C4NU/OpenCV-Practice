//
//  GrayPixel.cpp
//  OpenCV
//
//  Created by 전효재 on 2023/03/16.
//

#include <opencv2/opencv.hpp>

int main()
{
    // original : source        : src
    // result   : destination    : dst

    IplImage* src = cvLoadImage("Resources/Lenna.png");
    //if (src == nullptr) exit(-1);

    CvSize size = cvGetSize(src);
    IplImage* dst = cvCreateImage(size, 8, 3);

    for(int y=0; y<size.height; y++)
        for (int x = 0; x < size.width; x++)
        {
            CvScalar s = cvGet2D(src, y, x);
            int bri = (s.val[0] + s.val[1] + s.val[2])/3;
/*
            CvScalar d = s;
            if(x>size.width/2)
                d = cvScalar(bri, bri, bri);
*/
            CvScalar c = cvScalar(100, 200, 255);
            CvScalar d;
            for(int k=0; k<3; k++)
                d.val[k] = c.val[k] * bri / 255.0f;
            cvSet2D(dst, y, x, d);
        }

    cvShowImage("source", src);
    cvShowImage("destination", dst);
    cvWaitKey();
    return 0;

}
