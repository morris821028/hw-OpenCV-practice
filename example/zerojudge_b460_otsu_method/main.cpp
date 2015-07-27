#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <fstream>
#include <stdio.h>

using namespace cv;
using namespace std;

class IMAGE {
public:
    struct Pixel {
        int r, g, b;
        Pixel(int x = 0, int y = 0, int z = 0):
        r(x), g(y), b(z) {}
        void read() {
            scanf("%d %d %d", &r, &g, &b);
        }
        Pixel operator-(const Pixel &x) const {
            return Pixel(r-x.r, g-x.g, b-x.b);
        }
        Pixel operator+(const Pixel &x) const {
            return Pixel(r+x.r, g+x.g, b+x.b);
        }
        Pixel operator*(const double x) const {
            return Pixel(r*x, g*x, b*x);
        }
        Pixel operator/(const double x) const {
            return Pixel(r/x, g/x, b/x);
        }
        bool operator==(const Pixel &x) const {
            return r == x.r && g == x.g && b == x.b;
        }
        void print() {
            printf("%3d", length());
        }
        int sum() {
            return r + g + b;
        }
        int length() {
            return abs(r) + abs(g) + abs(b);
        }
        int dist(Pixel x) {
            return abs((r + g + b) - (x.r + x.g + x.b));
        }
    };
    int W, H;
    static const int MAXN = 256;
    Pixel data[MAXN][MAXN], tmp[MAXN][MAXN];
    void read() {
        scanf("%d %d", &W, &H);
        for (int i = 0; i < H; i++)
            for (int j = 0; j < W; j++)
                data[i][j].read();
    }
    void print() {
        printf("%d %d\n", W, H);
        for (int i = 0; i < H; i++)
            for (int j = 0; j < W; j++)
                data[i][j].print(), printf("%c", j == W-1 ? '\n' : ' ');
    }
    int isValid(int x, int y) {
        return x >= 0 && y >= 0 && x < H && y < W;
    }
    void otsu() {
        show("origin", data);
        double cnt[512] = {}, sum[512] = {};
        for (int i = 0; i < H; i++) {
            for (int j = 0; j < W; j++) {
                cnt[(data[i][j].sum()+1)/3] ++;
                sum[(data[i][j].sum()+1)/3] += (data[i][j].sum()+1)/3;
            }
        }
        
        double total = H*W, mx = -1;
        double th = -1;
        double wB = 0, wF = 0, sumB = 0, mB, mF;
        for (int i = 0; i < 256; i++) {
            sum[i] = sum[i] + (i ? sum[i-1] : 0);
            cnt[i] = cnt[i] + (i ? cnt[i-1] : 0);
        }
        for (int i = 1; i < 256; i++) {
            wB = cnt[i-1], wF = total - cnt[i-1];
            if (wB == 0 || wF == 0)    continue;
            sumB = sum[i-1];
            mB = sumB / wB;
            mF = (sum[255] - sumB) / wF;
            double v = (double) wB * wF * (mB - mF) * (mB - mF);
            if (mx == -1 || v > mx) {
                th = i;
                mx = v;
            }
        }
        for (int i = 0; i < H; i++) {
            for (int j = 0; j < W; j++) {
                if (data[i][j].sum() >= th * 3)
                    data[i][j] = Pixel(255, 255, 255);
                else
                    data[i][j] = Pixel(0, 0, 0);
            }
        }
        show("otsu", data);
    }
    void show(string winID, Pixel data[][MAXN]) {
        static int X = 0, Y = 0;
        IplImage *img = cvCreateImage(cvSize(W, H), IPL_DEPTH_8U, 3);
        for (int i = 0; i < img->height; i++) {
            uchar* ptr = (uchar*) (img->imageData + i * img->widthStep);
            for( int j = 0; j < img->width; j++) {
                ptr[3*j+0] = data[i][j].b;
                ptr[3*j+1] = data[i][j].g;
                ptr[3*j+2] = data[i][j].r;
            }
        }
        cvShowImage(winID.c_str(), img);
        moveWindow(winID.c_str(), W * Y + 64, (H + 32) * X);
        Y++;
        if (Y == 5)
            X++, Y = 0;
    }
} image;

IplImage *img;

int main() {
    ifstream fin("in.txt");
    fin >> image.W >> image.H;
    for (int i = 0; i < image.H; i++) {
        for (int j = 0; j < image.W; j++) {
            int r, g, b;
            fin >> r >> g >> b;
            image.data[i][j] = IMAGE::Pixel(r, g, b);
        }
    }
    image.otsu();
    cvWaitKey(0);
    return 0;	
}

