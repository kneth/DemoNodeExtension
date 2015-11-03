#include <node.h>
#include "person_wrap.hpp"
#include "book_wrap.hpp"

using namespace v8;

void InitAll (Handle<Object> exports) {
    PersonWrap::Init(exports);
    BookWrap::Init(exports);
}

NODE_MODULE(funstuff, InitAll)
