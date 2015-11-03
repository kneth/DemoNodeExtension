#ifndef PERSON_HPP
#define PERSON_HPP

#include <ctime>
#include <string>

using namespace std;

class Person {
public:
    Person() {};
    ~Person() {};

    void firstname(string s)        { m_firstname = s; };
    string firstname(void)          { return m_firstname; };

    void lastname(string s)          { m_lastname = s; };
    string lastname(void)            { return m_lastname; };

    void birthday(time_t t)          { m_birthday = t; };
    time_t birthday(void)            { return m_birthday; };

    string to_str(void);

private:
    string m_firstname;
    string m_lastname;
    time_t m_birthday;
};

#endif
