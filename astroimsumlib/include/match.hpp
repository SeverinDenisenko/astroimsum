#include <math.h>
#include <stdio.h>
#include <stdlib.h>

inline void triangle(
    double x1,
    double y1,
    double x2,
    double y2,
    double x3,
    double y3,
    double* trp)
{
    double d12 = sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
    double d13 = sqrt((x3 - x1) * (x3 - x1) + (y3 - y1) * (y3 - y1));
    trp[0]     = d13 / d12;
    trp[1]     = ((x2 - x1) * (x3 - x1) + (y2 - y1) * (y3 - y1)) / d13 / d12;
    trp[2] = fabs((x2 - x1) * (y3 - y1) + (x3 - x1) * (y2 - y1)) / d13 / d12;
} // end of triangle

inline void match(
    double* p1,
    double* p2,
    int n1,
    int n2,
    int* ID1,
    int* ID2,
    int nID1,
    int nID2,
    int* TRI1,
    int* TRI2)
{
    double* trp1 = new double[3];
    double* trp2 = new double[3];
    int I, J, K;
    double x1, y1, x2, y2, x3, y3;
    // int N = nID1*nID2*6;
    // double* D = new double[N];
    int Q[6][3] = { { 0, 1, 2 }, { 0, 2, 1 }, { 1, 0, 2 },
                    { 1, 2, 0 }, { 2, 0, 1 }, { 2, 1, 0 } };
    int Imin = 0, Jmin = 0, Kmin = 0;
    double d, dmin;
    for (int i = 0; i < nID1; i++) {
        // printf("%d,%d,%d\n",ID1[3*i],ID1[3*i+1],ID1[3*i+2]);
        I  = ID1[3 * i];
        J  = ID1[3 * i + 1];
        K  = ID1[3 * i + 2];
        x1 = p1[2 * I];
        y1 = p1[2 * I + 1];
        x2 = p1[2 * J];
        y2 = p1[2 * J + 1];
        x3 = p1[2 * K];
        y3 = p1[2 * K + 1];
        triangle(x1, y1, x2, y2, x3, y3, trp1);
        for (int j = 0; j < nID2; j++) {
            for (int k = 0; k < 6; k++) {
                // printf("%d,%d,%d\n",i,j,k);
                I  = ID2[3 * j + Q[k][0]];
                J  = ID2[3 * j + Q[k][1]];
                K  = ID2[3 * j + Q[k][2]];
                x1 = p2[2 * I];
                y1 = p2[2 * I + 1];
                x2 = p2[2 * J];
                y2 = p2[2 * J + 1];
                x3 = p2[2 * K];
                y3 = p2[2 * K + 1];
                triangle(x1, y1, x2, y2, x3, y3, trp2);
                // D[6*(i*nID2+j)+k]
                d = (trp1[0] - trp2[0]) * (trp1[0] - trp2[0])
                    + (trp1[1] - trp2[1]) * (trp1[1] - trp2[1])
                    + (trp1[2] - trp2[2]) * (trp1[2] - trp2[2]);
                if ((i == 0) && (j == 0) && (k == 0))
                    dmin = d;
                else {
                    if (d < dmin) {
                        Imin = i, Jmin = j, Kmin = k;
                        dmin = d;
                    } // end if
                } // end else
            } // end for k
        } // end for j
    } // end for i
    // printf("min = %d,%d,%d\n",Imin,Jmin,Kmin);
    TRI1[0] = ID1[3 * Imin];
    TRI1[1] = ID1[3 * Imin + 1];
    TRI1[2] = ID1[3 * Imin + 2];
    TRI2[0] = ID2[3 * Jmin + Q[Kmin][0]];
    TRI2[1] = ID2[3 * Jmin + Q[Kmin][1]];
    TRI2[2] = ID2[3 * Jmin + Q[Kmin][2]];
} // end match

inline double transform(int x, int y, double* Z, int n)
{
    int q    = 0;
    double u = 0;
    for (int i = 0; i <= n; i++)
        for (int j = 0; j <= n; j++)
            if (i + j <= n) {
                u += Z[q] * pow(x, i) * pow(y, j);
                q += 1;
            }
    return u;
} // end transform

inline void overlap(
    double* IMG,
    double* img,
    int W,
    int H,
    int w,
    int h,
    double* Zx,
    double* Zy,
    int n)
{
    double u, v;
    int U, V;
    for (int x = 0; x < W; x++)
        for (int y = 0; y < H; y++) {
            u = transform(x, y, Zx, n);
            v = transform(x, y, Zy, n);
            U = (int)u;
            V = (int)v;
            //
            if ((U >= 0) && (U < w) && (V >= 0) && (V < h)) {
                IMG[y * W + x] = img[V * w + U];
            }
        } // end for
} // end overlap

inline void
overlapaf(double* IMG, double* img, int W, int H, int w, int h, double* Z)
{
    double u, v;
    int U, V;
    for (int x = 0; x < W; x++)
        for (int y = 0; y < H; y++) {
            u = Z[1] + x * cos(Z[0]) + y * sin(Z[0]);
            v = Z[2] - x * sin(Z[0]) + y * cos(Z[0]);
            U = (int)u;
            V = (int)v;
            //
            if ((U >= 0) && (U < w) && (V >= 0) && (V < h)) {
                IMG[y * W + x] = img[V * w + U];
            }
        } // end for
} // end overlapaf

inline void overlap2(
    unsigned short* IMG,
    unsigned short* img,
    int W,
    int H,
    int w,
    int h,
    double* Zx,
    double* Zy,
    int n)
{
    double u, v;
    int U, V;
    for (int x = 0; x < W; x++)
        for (int y = 0; y < H; y++) {
            u = transform(x, y, Zx, n);
            v = transform(x, y, Zy, n);
            U = (int)u;
            V = (int)v;
            //
            if ((U >= 0) && (U < w) && (V >= 0) && (V < h)) {
                if (IMG[y * W + x] == 0)
                    IMG[y * W + x] = img[V * w + U];
                else
                    IMG[y * W + x] = (IMG[y * W + x] + img[V * w + U]) / 2;
            }
        } // end for
} // end overlap
