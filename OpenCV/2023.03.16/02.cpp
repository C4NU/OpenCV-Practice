//
//  02.cpp
//  OpenCV
//
//  Created by 전효재 on 2023/03/16.
//

#include <opencv2/opencv.hpp>

int main()
{
    IplImage* img = cvLoadImage("/Users/jeonhyojae/Xcode/OpenCV/OpenCV/2023.03.07/sample.jpg");

    IplImage* img2 = cvLoadImage("/Users/jeonhyojae/Xcode/OpenCV/OpenCV/Assignments/Assignment #1/sejong_small.jpg");
//    int w = img->width;
//    int h = img->height;
    
    IplImage* result = cvCreateImage(cvGetSize(img), 8, 3);
    
    CvSize size = cvGetSize(img);
    int w = size.width;
    int h = size.height;



    printf("w= %d h = %d\n", w, h);

    CvScalar c;
    int x = 100;
    int y = 100;
    c = cvGet2D(img, y, x);

    printf("b = %f g = %f r = %f \n", c.val[0], c.val[1], c.val[2]);
    c.val[0] = 0;                    // blue
    c.val[1] = 0;                    // green
    c.val[2] = 255;                    // red

    c = cvScalar(0, 0, 255);

    cvShowImage("Hello Pixel", img);
//    cvShowImage("Hello Pizel2", img2);
    cvWaitKey();

    /*
    for (int y = 0; y < h; y++)
    {
        for (int x = 0; x < w; x++)
        {
            CvScalar c = cvGet2D(img, y, x);
            int b = c.val[0];
            int g = c.val[1];
            int r = c.val[2];

            cvSet2D(img, y, x, cvScalar(255-b, 255 - g, 255 - r));

        }

    }
    */
    for (float a = 0.f; a <= 1.0f; a += 0.01)
    {
        for (int y = 0; y < h; y++)
            for (int x = 0; x < w; x++)
            {
                if (x > img2->width - 1 || y > img2->height - 1) continue;
                CvScalar c1 = cvGet2D(img, y, x);
                CvScalar c2 = cvGet2D(img2, y, x);

                c.val[0] = (1 - a)*c1.val[0] + a * c2.val[0];
                c.val[1] = (1 - a) * c1.val[1] + a * c2.val[1];
                c.val[2] = (1 - a) * c1.val[2] + a * c2.val[2];

                cvSet2D(result, y, x, c);

            }
        cvShowImage("Hello Pixel", result);
        printf("a=%f\n", a);
        cvWaitKey(10);
    }






    return 0;
}
