int addition(int a, int b) {
    return a + b;
}

int main() {
    int a = 1, b = 2;
    a = addition(a = b, b);
    return a;
}