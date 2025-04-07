int main() {
    int a = 0;
    if (a==0) {
        a = 1;
        if (a==1) {
            a = 2;
            if (a==2) {
                a = 3;
                if (a < 4) {
                    a = 4;
                }
                else {
                    a = 5;
                }
            }
        }
    }
    else {
        a = 6;
    }
    return a;
}