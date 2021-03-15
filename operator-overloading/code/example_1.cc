#include <stdio.h>

class Yori {
public:
    friend int operator+(const Yori& lhs, const Yori& rhs);
    friend int operator-(const Yori& lhs, const Yori& rhs);
    friend int operator* (const Yori& lhs, const Yori& rhs);
    friend int operator/ (const Yori& lhs, const Yori& rhs);

    Yori(int val): val_(val) {}

    int operator+(const Yori& other) {
        printf("operator+ \n");
        return val_ + other.val_;
    }

    int operator-(const Yori& other) {
        printf("operator- \n");
        return val_ - other.val_;
    }

    int operator*(const Yori& other) {
        printf("operator* \n");
        return val_ * other.val_;
    }

    int operator/(const Yori& other) {
        printf("operator/ \n");
        return val_ / other.val_;
    }
private:
    int val_ = 0;
};

int operator+ (const Yori& lhs, const Yori& rhs) {
    return lhs.val_ + rhs.val_;
}

int operator- (const Yori& lhs, const Yori& rhs) {
    return lhs.val_ - rhs.val_;
}

int operator* (const Yori& lhs, const Yori& rhs) {
    return lhs.val_ * rhs.val_;
}

int operator/ (const Yori& lhs, const Yori& rhs) {
    return lhs.val_ / rhs.val_;
}

int main(int argc, char* argv[]) {
    int t = (Yori(3) + Yori(4) - Yori(1)) * Yori(3) / Yori(2);
    printf("Result: %d \nn", t);
    return 0;
}