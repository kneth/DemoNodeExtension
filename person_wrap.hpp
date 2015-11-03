#ifndef PERSON_WRAP_H
#define PERSON_WRAP_H

#include <node.h>
#include <node_object_wrap.h>

#include "person.hpp"
#include "book.hpp"

class PersonWrap : public node::ObjectWrap {
    friend class BookWrap;
public:
    static void Init(v8::Handle<v8::Object> exports);
    static void New(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Handle<v8::Object> New(v8::Isolate* isolate, Book* n, uint32_t index);
    static v8::Handle<v8::Object> NewInstance();

    // getters and setters
    static void FirstnameGetter(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info);
    static void FirstnameSetter(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info);
    static void LastnameGetter(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info);
    static void LastnameSetter(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info);
    static void BirthdayGetter(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info);
    static void BirthdaySetter(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info);

    // function wrappers
    static void ToString(const v8::FunctionCallbackInfo<v8::Value>& args);

    PersonWrap();

private:
    ~PersonWrap();

    Person* m_person;
    static v8::Persistent<v8::Function> Constructor;
};

#endif // PERSON_WRAP_H
