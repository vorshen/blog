#include <stdio.h>
#pragma pack(8)

class Foo {
public:
    int a;
};

int main(int argc, char* argv[]) {
    char a;
    char b;
    Foo foo;
    printf("a adress: %p\n", &a);
    printf("b adress: %p\n", &b);
    printf("Foo adress: %p\n", &foo);
    return 0;
}
