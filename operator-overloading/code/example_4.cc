#include <stdio.h>

class Yori {
public:
    Yori(int val): val_(val) {}

    bool operator&&(const Yori& other) {
        return val_ && other.val_;
    }

    bool operator||(const Yori& other) {
        return val_ || other.val_;
    }

    bool operator!() {
        return !val_;
    }
private:
    int val_ = 0;
};

int main(int argc, char* argv[]) {
    printf(Yori(0) && Yori(1) ? "true" : "false");
    printf("\n");
    printf(Yori(0) || Yori(1) ? "true" : "false");
    printf("\n");
    printf(!Yori(0) ? "true" : "false");
    printf("\n");
    return 0;
}