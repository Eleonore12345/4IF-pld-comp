int addition(int a, int b) {
    return a+b;
}

int main() {
    int a = addition(2,3);
    int b = addition(addition(4,5),a);
    return b;
}