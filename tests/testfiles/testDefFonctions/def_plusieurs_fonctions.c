int multiplication(int r, int s) {
    return r * s;
}

int addition(int a, int b, int c, int d, int e, int f) {
    c = a + b;
    d = c + e;
    a = d + f;
    a = multiplication(c,d);
    return a;
}

int main() {
    int x = 2;
    int y = 3;
    int z = 4;
    int w = 5;
    int v = 6;
    int u = 7;
    int g = addition(x,y,z,w,v,u);
    return multiplication(g,x);
}