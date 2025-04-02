int main() {
    int a = 2;
    {
        {
            int a = 12;
        }
        return a;
    }
}