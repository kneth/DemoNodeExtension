#include "book.hpp"
#include "person.hpp"

#include <string>

void Book::add(Person *p) {
    m_persons.push_back(p);
}

Person* Book::lookup(string name) {
    vector<Person*>::iterator it = m_persons.begin();
    while (it != m_persons.end()) {
        Person *p = *it;
        if (name.compare(p->firstname()) == 0) {
            return *it;
        }
        ++it;
    }
    throw BookLookupException();
}

Person* Book::operator[](const size_t index) {
    if (index >= 0 && index <= m_persons.size()) {
        return m_persons[index];
    }
    throw BookRangeException();
}

void Book::remove(const size_t index) {
    if (index >= 0 && index <= m_persons.size()) {
        m_persons.erase(m_persons.begin()+index);
    }
    else {
        throw BookRangeException();
    }
}
