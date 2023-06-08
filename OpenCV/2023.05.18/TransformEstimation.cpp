//
//  TransformEstimation.cpp
//  OpenCV
//
//  Created by 전효재 on 2023/06/08.
//

#include <opencv2/opencv.hpp>
#include "MatrixInverse.h"

IplImage* src;
IplImage* dst;

CvPoint P[3] = { cvPoint(250,100),
                   cvPoint(100,350),
                   cvPoint(400,350) };
CvPoint Q[3] = { cvPoint(250,100),
                   cvPoint(100,350),
                   cvPoint(400,350) };
/*
CvPoint Q[3] = { cvPoint(100,100),
                 cvPoint(200, 400),
                 cvPoint(400, 100) };
*/

void drawPoints(IplImage* img, CvPoint p[], int num, CvScalar c)
{
    for (int i = 0; i < num; i++)
        cvCircle(img, p[i], 5, c, -1);
}

void applyAffineTransform(IplImage* src, IplImage* dst, float M[][3])
{
    cvSet(dst, cvScalar(0, 0, 0));

    for(int y1 = 0; y1<src->height; y1++)
        for (int x1 = 0; x1 < src->width; x1++)
        {
            float w1 = 1.0f;

            float x2 = M[0][0] * x1 + M[0][1] * y1 + M[0][2] * w1;
            float y2 = M[1][0] * x1 + M[1][1] * y1 + M[1][2] * w1;
            float w2 = M[2][0] * x1 + M[2][1] * y1 + M[2][2] * w1;

            x2 /= w2;
            y2 /= w2;

            if (x2<0 || x2> dst->width - 1) continue;
            if (y2<0 || y2>dst->height - 1) continue;

            CvScalar f = cvGet2D(src, y1, x1);
            cvSet2D(dst, y2, x2, f);

        }
        
}

void applyInverseAffineTransform(IplImage* src, IplImage* dst, float M[][3])
{
    cvSet(dst, cvScalar(0, 0, 0));

    for (int y2 = 0; y2 < dst->height; y2++)
        for (int x2 = 0; x2 < dst->width; x2++)
        {
            float w2 = 1.0f;

            float x1 = M[0][0] * x2 + M[0][1] * y2 + M[0][2] * w2;
            float y1 = M[1][0] * x2 + M[1][1] * y2 + M[1][2] * w2;
            float w1 = M[2][0] * x2 + M[2][1] * y2 + M[2][2] * w2;

            x1 /= w1;
            y1 /= w1;

            if (x1<0 || x1> src->width - 1) continue;
            if (y1<0 || y1> src->height - 1) continue;

            CvScalar f = cvGet2D(src, y1, x1);
            cvSet2D(dst, y2, x2, f);

        }

}




void getMultiply(float M[][3], float A[][3], float B[][3])
{
    for(int i = 0; i<3; i++)
        for (int j = 0; j < 3; j++)
        {
            M[i][j] = 0.0f;
            for (int k = 0; k < 3; k++)
                M[i][j] += A[i][k] * B[k][j];
        }
}


void getTransform(float M[][3], CvPoint P[], CvPoint Q[])
{
    float A[3][3];        // <== P
    float B[3][3];        // <== Q

    for (int i = 0; i < 3; i++)
    {
        A[0][i] = P[i].x;
        A[1][i] = P[i].y;
        A[2][i] = 1.0f;
        
        B[0][i] = Q[i].x;
        B[1][i] = Q[i].y;
        B[2][i] = 1.0f;
    }
    // B invA = M
    // M = B invA
    float invA[3][3];
    InverseMatrixGJ(A, invA, 3);

    getMultiply(M, B, invA);
}

int selected = -1;
void myMouse(int event, int x, int y, int flags, void*)
{
    if (event == CV_EVENT_LBUTTONDOWN)
    {
        selected = -1;
        for (int i = 0; i < 3; i++)
        {
            float dist2 = (Q[i].x - x) * (Q[i].x - x) + (Q[i].y - y) * (Q[i].y - y);
            if (dist2 < 5 * 5)
                selected = i;
        }
        printf("selected = %d x = %d y = %d\n", selected, x, y);

    }
    if (event == CV_EVENT_LBUTTONUP || event == CV_EVENT_MOUSEMOVE)
    {
        if (selected < 0) return;

        Q[selected] = cvPoint(x, y);
        float M[3][3];
        getTransform(M, Q, P);
        applyInverseAffineTransform(src, dst, M);
        drawPoints(dst, Q, 3, cvScalar(255, 0, 0));
        cvShowImage("dst", dst);

        if(event == CV_EVENT_LBUTTONUP)
            selected = -1;
    }
}


int main()
{
    src = cvLoadImage("c:\\temp\\lena_triangle.png");
    CvSize size = cvGetSize(src);
    dst = cvCreateImage(size, 8, 3);

    cvCopy(src, dst);
    drawPoints(dst, Q, 3, cvScalar(255, 0, 0));


    cvShowImage("dst", dst);
    cvSetMouseCallback("dst", myMouse);
    cvWaitKey();


    return 0;
}
