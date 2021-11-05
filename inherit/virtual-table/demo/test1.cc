#include <stdio.h>

class Base {
public:
    int a;
    virtual void func1() {
        printf("Base::func1\n");
    }
    virtual void func2() {
        printf("Base::func2\n");
    }
};

class Derive : public Base {
public:
    int b;
    void func2() {
        printf("Derive::func2\n");
    }
    virtual void func3() {
        printf("Derive::func3\n");
    }
    void func4() {
        printf("Derive::func4\n");
    }
};

int main(int argc, char* argv[]) {
    return 0;
}