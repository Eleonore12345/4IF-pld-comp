int main() {
    int a = 2, b = 3;
    int c = (a = b) + 1;
    return c;
}