// compile: clang++ -o main main.cpp book.cpp person.cpp

#include "person.hpp"
#include "book.hpp"

#include <iostream>

using namespace std;

int main(int argc, char* argv[]) {
    Book   b;

    Person *p1 = new Person();
    p1->firstname("Arthur");
    p1->lastname("Clarke");
    p1->birthday(-1642381200); // 1917-12-16
    b.add(p1);

    Person *p2 = new Person();
    p2->firstname("Peter");
    p2->lastname("Hamilton");
    p2->birthday(-310352400);  // 1960-03-02
    b.add(p2);

    Person *p3 = new Person();
    p3->firstname("Cory");
    p3->lastname("Doctorow");
    p3->birthday(48553200);    // 1971-07-17
    b.add(p3);

    Person *p4 = new Person();
    p4->firstname("Charlie");
    p4->lastname("Stross");
    p4->birthday(-164221200);  // 1964-10-18
    b.add(p4);

    for(size_t i=0; i<b.size(); ++i) {
        Person *p = b[i];
        cout << p->to_str() << endl;
    }

    cout << "Looking for Peter" << endl;
    Person *p5 = b.lookup("Peter");
    cout << p5->to_str() << endl;

    b.remove(1);
    cout << "After remove: " << b.size() << endl;
}
