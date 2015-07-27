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
    void distorting_mirror() {
        show("origin", data);
        int rW = W, rH = H;
        double ch = H / 2.0;
        for (int i = 0; i < rH; i++) {
            double x, y;
            if (i < ch)
                x = ch - pow(i-ch, 2)/ch;
            else
                x = pow(i-ch, 2)/ch + ch;
            for (int j = 0; j < rW; j++) {
                y = j;
                int lx, rx, ly, ry;
                lx = floor(x), rx = ceil(x);
                ly = floor(y), ry = ceil(y);
                int px[] = {lx, lx, rx, rx};
                int py[] = {ly, ry, ly, ry};
                int c = -1;
                double mndist = 1e+30;
                for (int k = 0; k < 4; k++) {
                    if (!isValid(px[k], py[k]))
                        continue;
                    double d = (x-px[k])*(x-px[k])+(y-py[k])*(y-py[k]);
                    if (c == -1 || mndist > d)
                        c = k, mndist = d;
                }
                assert (c >= 0);
                tmp[i][j] = data[px[c]][py[c]];
            }
        }
        W = rW, H = rH;
        for (int i = 0; i < H; i++) {
            for (int j = 0; j < W; j++) {
                data[i][j] = tmp[i][j];
            }
        }
        show("test", data);
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
    image.distorting_mirror();
    cvWaitKey(0);
    return 0;	
}

