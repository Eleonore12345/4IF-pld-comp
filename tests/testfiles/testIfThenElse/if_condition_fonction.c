int test(int a) {
    int x;
    if (a == 0) {
        x = 0;
    }
    else {
        x = 1;
    }
    return x;
}

int main() {
    int a = 0;
    int b;
    if (test(a)) {
        b = 1;
    }
    else {
        b = 2;
    }
    return b;
}