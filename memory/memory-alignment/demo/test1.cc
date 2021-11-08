#include <stdio.h>
#pragma pack(8)

class Foo {
public:
    int a;
};

class Bar1 {
public:
    int a;
    long long b;
    int c;
    long long d;
};

class Bar2 {
public:
    long long a;
    int b;
    int c;
    long long d;
};

int main(int argc, char* argv[]) {
    printf("Foo size: %d\n", sizeof(Foo));
    printf("Bar1 size: %d\n", sizeof(Bar1));
    printf("Bar2 size: %d\n", sizeof(Bar2));
    return 0;
}
