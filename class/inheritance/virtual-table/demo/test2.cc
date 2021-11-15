#include <stdio.h>
#pragma pack(4)

class Base1 {
public:
    int a;

    virtual void func1() {
        printf("Base1::func1\n");
    }

    virtual void func2() {
        printf("Base1::func2\n");
    }
};

class Base2 {
public:
    int b;

    virtual void func2() {
        printf("Base2::func2\n");
    }

    virtual void func3() {
        printf("Base2::func2\n");
    }
};

class Derive : public Base1, public Base2 {
public:
    int c;

    void func2() {
        printf("Derive::func2\n");
    }

    void func3() {
        printf("Derive::func3\n");
    }

    virtual void func4() {
        printf("Derive::func4\n");
    }
};

typedef void(*fun)(void);

int main(int argc, char* argv[]) {
    Derive d;

    // +0 Base1::func1
    // +1 Derive::func2
    // +2 Derive::func3
    // +3 Derive::func4
    fun fun1 = *((fun*)*((long long*)(&d)) + 3);
    fun1();

    // +0 Derive::func2
    // +0 Derive::func3
    fun fun2 = *((fun*)*((long long*)(&d) + 2) + 1);
    fun2();
    return 0;
}
