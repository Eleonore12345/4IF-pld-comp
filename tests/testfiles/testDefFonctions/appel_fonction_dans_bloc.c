int multiplication(int a, int b) {
    {
        return a * b;
    }
}

int addition(int a, int b, int c, int d) {
    int e;
    int f;
    {
        e = multiplication(a,b);
        f = c + d;
    }
    return e/f;
}

int main() {
    return addition(4,5+6,2*3,2/1);
}