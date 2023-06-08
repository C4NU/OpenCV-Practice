//
//  FeatureBasedMorphing_2.cpp
//  OpenCV
//
//  Created by 전효재 on 2023/06/08.
//

//  2가지 이미지

#include <opencv2/opencv.hpp>

struct Line
{
    CvPoint P;
    CvPoint Q;
};

IplImage* src1;
IplImage* src1Copy;
IplImage* src2;
IplImage* src2Copy;
IplImage* dst;


Line A[100];
int numA = 0;
Line B[100];
int numB = 0;

#include <string>

void drawLine(IplImage* img, Line L[], int num, CvScalar c)
{
    for (int i = 0; i < num; i++)
        cvLine(img, L[i].P, L[i].Q, c, 3);
}

void myMouse1(int event, int x, int y, int flags, void*)
{
    if (event == CV_EVENT_LBUTTONDOWN)
        A[numA].P = cvPoint(x, y);
    if (event == CV_EVENT_LBUTTONUP)
    {
        A[numA].Q = cvPoint(x, y);
        numA++;
        printf("Num Line A = %d \n", numA);
        cvCopy(src1, src1Copy);
        drawLine(src1Copy, A, numA, cvScalar(0, 0, 255));
        //cvLine(src, A.P, A.Q, cvScalar(0, 0, 255), 3);
        cvShowImage("src1", src1Copy);
    }
}
void myMouse2(int event, int x, int y, int flags, void*)
{
    if (event == CV_EVENT_LBUTTONDOWN)
        B[numB].P = cvPoint(x, y);
    if (event == CV_EVENT_LBUTTONUP)
    {
        B[numB].Q = cvPoint(x, y);
        numB++;
        printf("Num Line B = %d \n", numB);
        cvCopy(src2, src2Copy);
        drawLine(src2Copy, B, numB, cvScalar(255, 0, 0));
        //cvLine(dst, B.P, B.Q, cvScalar(255, 0, 0), 3);
        cvShowImage("src2", src2Copy);
    }
}
float getTransformed(Line A, Line B, float x1, float y1, float* x2, float* y2)
{

    float XPx = x1 - A.P.x;
    float XPy = y1 - A.P.y;
    float QPx = A.Q.x - A.P.x;
    float QPy = A.Q.y - A.P.y;
    float QPlen = sqrt(QPx * QPx + QPy * QPy);

    float u = (XPx * QPx + XPy * QPy)/(QPlen*QPlen); // (X-P) (Q-P) / |Q-P|^2
    
    float perQPx = -QPy;
    float perQPy = QPx;
    
    float v = (XPx * perQPx + XPy * perQPy) / (QPlen);

    float nQPx = B.Q.x - B.P.x;
    float nQPy = B.Q.y - B.P.y;
    float nPerQPx = -nQPy;
    float nPerQPy = nQPx;
    float nQPlen = sqrt(nQPx * nQPx + nQPy * nQPy);

    *x2 = B.P.x + u * nQPx + v * nPerQPx / nQPlen;
    *y2 = B.P.y + u * nQPy + v * nPerQPy / nQPlen;

    float dist = abs(v);
    if (u < 0)
    {
        dist = sqrt(XPx * XPx + XPy * XPy);
    }
    else if(u>1)
    {
        float XQx = x1 - A.Q.x;
        float XQy = y1 - A.Q.y;
        dist = sqrt(XQx * XQx + XQy * XQy);
    }

    float w = (QPlen/(dist+0.0001f));
    return w;
}

void doMorphing(IplImage* src, IplImage* dst, Line A[], Line B[], int num)
{
    for(int y1 = 0; y1<src->height; y1++)
        for (int x1 = 0; x1 < src->width; x1++)
        {
            float x_sum = 0.0f;
            float y_sum = 0.0f;
            float w_sum = 0.0f;
            for (int i = 0; i < num; i++)
            {
                float x2, y2;
                float w = getTransformed(A[i], B[i], x1, y1, &x2, &y2);
                x_sum += w*x2;
                y_sum += w*y2;
                w_sum += w;
            }
            x_sum /= w_sum;
            y_sum /= w_sum;

            if (x_sum<0 || x_sum>dst->width - 1) continue;
            if (y_sum<0 || y_sum>dst->height - 1) continue;

            CvScalar f = cvGet2D(src, y1, x1);
            cvSet2D(dst, y_sum, x_sum, f);
        }
}
void doMorphing2(IplImage* src, IplImage* dst, Line A[], Line B[], int num)
{
    for (int y2 = 0; y2 < dst->height; y2++)
        for (int x2 = 0; x2 < dst->width; x2++)
        {
            float x_sum = 0.0f;
            float y_sum = 0.0f;
            float w_sum = 0.0f;
            for (int i = 0; i < num; i++)
            {
                float x1, y1;
                float w = getTransformed(B[i], A[i], x2, y2, &x1, &y1);
                x_sum += w * x1;
                y_sum += w * y1;
                w_sum += w;
            }
            x_sum /= w_sum;
            y_sum /= w_sum;

            if (x_sum<0 || x_sum>src->width - 1) continue;
            if (y_sum<0 || y_sum>src->height - 1) continue;

            CvScalar f = cvGet2D(src, y_sum, x_sum);
            cvSet2D(dst, y2, x2, f);
        }
}

int main()
{
    src1 = cvLoadImage("Resources/1.jpg");
    src2 = cvLoadImage("Resources/2.jpg");
    CvSize size = cvGetSize(src1);
    dst = cvCreateImage(size, 8, 3);
    src1Copy = cvCreateImage(size, 8, 3);
    src2Copy = cvCreateImage(size, 8, 3);

    cvShowImage("src1", src1);
    cvSetMouseCallback("src1", myMouse1);

    cvShowImage("src2", src2);
    cvSetMouseCallback("src2", myMouse2);
    cvWaitKey();


    IplImage* mid1 = cvCreateImage(size, 8, 3);
    IplImage* mid2 = cvCreateImage(size, 8, 3);
    Line M[100];
    int num = numA;
    int ind = 0;
    for (float alpha = 0.0f; alpha <= 1.0f; alpha += 0.05f)
    {
        for (int i = 0; i < num; i++)
        {
            M[i].P.x = (1 - alpha) * A[i].P.x + alpha * B[i].P.x;
            M[i].P.y = (1 - alpha) * A[i].P.y + alpha * B[i].P.y;
            M[i].Q.x = (1 - alpha) * A[i].Q.x + alpha * B[i].Q.x;
            M[i].Q.y = (1 - alpha) * A[i].Q.y + alpha * B[i].Q.y;
        }
        doMorphing2(src1, mid1, A, M, num);
        doMorphing2(src2, mid2, B, M, num);
        //    cvShowImage("mid1", mid1);
        //    cvShowImage("mid2", mid2);
        //    cvWaitKey();

        for (int y = 0; y < dst->height; y++)
            for (int x = 0; x < dst->width; x++)
            {
                CvScalar f1 = cvGet2D(mid1, y, x);
                CvScalar f2 = cvGet2D(mid2, y, x);

                CvScalar g;
                for (int k = 0; k < 3; k++)
                    g.val[k] = (1 - alpha) * f1.val[k] + alpha * f2.val[k];
                cvSet2D(dst, y, x, g);
            }
        cvShowImage("dst", dst);
        
        std::string filename = "dst_";
        char n[2];
        n[0] = 'a' + ind;
        n[1] = '\0';
        filename += n;
        filename += ".jpg";

        cvSaveImage(filename.c_str(), dst);
        cvWaitKey();
        printf("alpha = %f \n", alpha);
        ind++;
    }

//    doMorphing2(src, dst, A, B, numA);
//    drawLine(dst, B, numB, cvScalar(255, 0, 0));
//    cvShowImage("dst", dst);
//    cvWaitKey();

    return 0;
}
