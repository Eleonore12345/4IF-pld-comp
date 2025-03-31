int addition(int a, int b, int c, int d, int e, int f, int g, int h, int i, int j) {
    a = b * c + d;
    a = a + f + e + g + h + i;
    a = a * j;
    return a;
}

int main() {
    int x = 2;
    int y = 3;
    int z = 4;
    int w = 5;
    int v = 6;
    int u = 7;
    int r = 2;
    int s = 3;
    int t = 1;
    int m = 2;
    return addition(x,y,z,w,v,u,r,s,t,m);
}