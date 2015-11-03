#include "person.hpp"
#include <time.h>
#include <sstream>
#include <string>

using namespace std;

string Person::to_str(void) {
    stringstream ss;
    char c[32];
    struct tm *t = localtime(&m_birthday);
    
    strftime(c, 32, "%Y-%m-%d", t);
    ss << m_firstname << " " << m_lastname << ": " << c;
    return ss.str();
}
