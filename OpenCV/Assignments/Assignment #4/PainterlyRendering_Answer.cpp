//
//  PainterlyRendering_Answer.cpp
//  OpenCV
//
//  Created by 전효재 on 2023/06/08.
//

#include <opencv2/opencv.hpp>

struct Stroke
{
    CvPoint pt;
    CvScalar color;
    int R;
};


float diff2(CvScalar a, CvScalar b)
{

    return (a.val[0] - b.val[0]) * (a.val[0] - b.val[0])
        + (a.val[1] - b.val[1]) * (a.val[1] - b.val[1])
        + (a.val[2] - b.val[2]) * (a.val[2] - b.val[2]);
}
void paintLayer(IplImage* dst, IplImage* ref, int R)
{
    int grid = R * 1.2f;
    int Xdiv = dst->width / grid;
    int Ydiv = dst->height / grid;

    int totalStroke = Xdiv * Ydiv;
    Stroke* s = new Stroke[totalStroke];
    int curNum = 0;

    float T = 400.0f;

    for(int i=0; i<Xdiv; i++)
        for (int j = 0; j < Ydiv; j++)
        {
            // (i, j)

            float err = 0.0f;
            int ct = 0;
            float max_err = 0.0f;
            CvScalar max_color;
            CvPoint max_pos;
            for(int u=0; u<grid; u++)
                for (int v = 0; v < grid; v++)
                {
                    int x = grid * i + u;
                    int y = grid * j + v;
                    if (x<0 || x>dst->width - 1) continue;
                    if (y<0 || y>dst->height - 1) continue;
                    
                    CvScalar f = cvGet2D(ref, y, x);
                    CvScalar d = cvGet2D(dst, y, x);
                    float curErr = diff2(f, d);
                    err += curErr;
                    ct++;
                    if (curErr > max_err)
                    {
                        max_err = curErr;
                        max_color = f;
                        max_pos = cvPoint(x, y);
                    }
                }
            err /= ct;
            if (err > T)
            {
                s[curNum].color = max_color;
                s[curNum].pt = max_pos;
                s[curNum].R = R;
                curNum++;
            }
        }

    for (int i = 0; i < curNum; i++)
    {
        int ind = rand() % curNum;
        Stroke temp = s[i];
        s[i] = s[ind];
        s[ind] = temp;
    }

    for (int i = 0; i < curNum; i++)
        cvCircle(dst, s[i].pt, s[i].R, s[i].color, -1);


    delete[] s;

}

int main()
{
    IplImage* src = cvLoadImage("c:\\temp\\lena.png");
    IplImage* dst = cvCreateImage(cvGetSize(src), 8, 3);
    cvSet(dst, cvScalar(255, 255, 255));
    IplImage* ref = cvCreateImage(cvGetSize(src), 8, 3);

    int R[4] = { 30, 15, 8, 4 };
    int num = 4;

    for (int i = 0; i < 4; i++)
    {
        cvSmooth(src, ref, CV_GAUSSIAN, R[i] * 4 + 1);
        paintLayer(dst, ref, R[i]);
        cvShowImage("dst", dst);
        cvWaitKey();
    }


    return 0;
}
