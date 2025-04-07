int main() {
    int a = 0;
    if (a == 1) {
        a = 1;
    } 
    else if (a != 0) {
        a = 2;
    }
    else if (a > 2) {
        a = 3;
    }
    else if (a < -2) {
        a = 4;
    }
    else if (a*a > 2) {
        a = 5;
    }
    else if (a == 0) {
        a = 6;
    }
    else {
        a = 7;
    }
    return a;
}