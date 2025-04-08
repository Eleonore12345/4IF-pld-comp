int main() {
    int a = 0;
    int b = 0;
    int c = 0;

    while (a < 2) {
        b = 0;
        while (b < 3) {
            c = c + 1;
            b = b + 1;
        }
        a = a + 1;
    }

    return c;
}