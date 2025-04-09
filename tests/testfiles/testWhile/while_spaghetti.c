int main() {
    int x = 0;
    int y = 5; 
    int z = 0;

    while (x < 5) {
        if (x % 2 == 0) {
            y = y + 1;

            if (y > 6) {
                z = z + 2;
            } 
            else if (y == 6) {
                z = z + 1;
            } 
            else {
                z = z - 1;
            }

        } 
        else if (x == 3) {
            while (y > 3) {
                y = y - 1;
                if (y == 4) {
                    z = z * 2;
                } 
                else {
                    z = z + 3;
                }
            }
        } 
        else {
            z = z + x;
        }

        x = x + 1;
    }

    return z;
}