#include <stdio.h>

class Yori {
public:
    Yori(int val): val_(val) {}

    bool operator<=(const Yori& other) {
        return val_ <= other.val_;
    }

    bool operator<(const Yori& other) {
        return val_ < other.val_;
    }

    bool operator>=(const Yori& other) {
        return val_ >= other.val_;
    }

    bool operator>(const Yori& other) {
        return val_ > other.val_;
    }

    bool operator==(const Yori& other) {
        return val_ == other.val_;
    }

    bool operator!=(const Yori& other) {
        return val_ != other.val_;
    }
private:
    int val_ = 0;
};

int main() {
    printf(Yori(1) <= Yori(2) ? "true" : "false");
    printf("\n");
    printf(Yori(1) < Yori(2) ? "true" : "false");
    printf("\n");
    printf(Yori(1) >= Yori(2) ? "true" : "false");
    printf("\n");
    printf(Yori(1) > Yori(2) ? "true" : "false");
    printf("\n");
    printf(Yori(1) == Yori(2) ? "true" : "false");
    printf("\n");
    printf(Yori(1) != Yori(2) ? "true" : "false");
    printf("\n");
    return 0;
}