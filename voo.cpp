#include <iostream>
#include <vector>

using namespace std;

class A {
    public:
        int a;

        A() { a = 42; }
        virtual ~A() {}
};

class B : public A {
    public:
        int b;

        B() { b = 68; }
};

int main() {
    A* a1 = new A();

    A* ab = new B;

    vector<A*> as;

    as.push_back(a1);
    as.push_back(ab);


    int n = as.size();

    for (int i = 0; i < n; i++) {
        B* bp = dynamic_cast<B*>(as[i]);

        if (bp != NULL) {
            cout << ((B*) ab)->b << endl;
        }
    }


    return 0;
}
