#include <stdio.h>

typedef void(*fun)(void);

class Foo {
    virtual void echo() {
        printf("Hello World\n");
    }

    virtual void echo2() {
        printf("balabalabala\n");
    }
};

int main(int argc, char* argv[]) {
    Foo foo;
    // 因为 private，所以无法直接访问
    // foo.echo();

    // 但是可以通过指针的方式来访问
    fun echo = *((fun*)*((long long*)(&foo)));
    echo();

    fun echo2a = *((fun*)*((long long*)(&foo)) + 1);
    echo2a();

    fun echo2b = *(fun*)(*((long long*)(&foo)) + 8);
    echo2b();
    return 0;
}