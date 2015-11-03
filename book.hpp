#ifndef BOOK_HPP
#define BOOK_HPP

#include <exception>
#include <vector>

#include "person.hpp"

using namespace std;

class Book {
public:
    Book() {};
    ~Book() {};

    Person* lookup(string name);
    void add(Person*);
    Person* operator[] (const size_t index);
    void remove(const size_t index);
    size_t size() { return m_persons.size(); };
private:
    vector<Person*> m_persons;
};


class BookLookupException {
public:
    BookLookupException() {};
    ~BookLookupException() {};

    const char* what() const {
        return "Name not found";
    }
};

class BookRangeException {
public:
    BookRangeException() {};
    ~BookRangeException() {};
    const char* what() const {
        return "Index is out of range";
    }
};


#endif
