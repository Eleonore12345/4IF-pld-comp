int main() {
    int a = 2147483647; /*255 sera retourné car le code de retour est interprété sur 1 octet*/
    int b = 1;
    return a/b;
}