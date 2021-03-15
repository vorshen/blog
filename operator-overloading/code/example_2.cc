#include <stdio.h>

class Yori {
public:
    Yori(int val): val_(val) {}

    int operator^(const Yori& other) {
        // 异或
        return val_ ^ other.val_;
    }

    int operator&(const Yori& other) {
        // 与
        return val_ & other.val_;
    }

    int operator|(const Yori& other) {
        // 或
        return val_ | other.val_;
    }

    int operator~() {
        // 取反
        return ~val_;
    }

    int operator<<(const Yori& other) {
        // 左移
        return val_ << other.val_;
    }

    int operator>>(const Yori& other) {
        // 右移
        return val_ >> other.val_;
    }
private:
    int val_ = 0;
};

int main(int argc, char* argv[]) {
    printf("异或: %d\n", Yori(1) ^ Yori(1));
    printf("与: %d\n", Yori(1) & Yori(1));
    printf("或: %d\n", Yori(1) | Yori(0));
    printf("取反: %d\n", ~Yori(1));
    printf("左移: %d\n", Yori(1) << Yori(1));
    printf("右移: %d\n", Yori(8) >> Yori(1));
    return 0;
}