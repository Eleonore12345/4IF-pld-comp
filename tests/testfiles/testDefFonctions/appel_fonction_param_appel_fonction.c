int addition(int a, int b, int c, int d) {
    int e = a * b;
    int f = c + d;
    return e/f;
}

int main() {
    return addition(4,addition(2,2,'5',5),2*3,2/1);
}