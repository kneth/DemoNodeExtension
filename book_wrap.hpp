#ifndef BOOK_WRAP_H
#define BOOK_WRAP_H

#include <node.h>
#include <node_object_wrap.h>

#include "book.hpp"
#include "person.hpp"

class BookWrap : public node::ObjectWrap {
public:
    static void Init(v8::Handle<v8::Object> exports);
    static void New(const v8::FunctionCallbackInfo<v8::Value>& args);

    // getter and setter
    static void Getter(uint32_t index, const v8::PropertyCallbackInfo<v8::Value>& info);
    static void Setter(uint32_t index, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<v8::Value>& info);
    static void Deleter(uint32_t index, const v8::PropertyCallbackInfo<v8::Boolean>& info);
    static void Enumerator(const v8::PropertyCallbackInfo<v8::Array>& info);


    // function wrappers
    static void Add(const v8::FunctionCallbackInfo<v8::Value>& args);
    static void Lookup(const v8::FunctionCallbackInfo<v8::Value>& args);
    static void Length(const v8::FunctionCallbackInfo<v8::Value>& args);
    static void Each(const v8::FunctionCallbackInfo<v8::Value>& args);
    static void Apply(const v8::FunctionCallbackInfo<v8::Value>& args);

    BookWrap();

private:
    ~BookWrap();

    Book* m_book;
    static v8::Persistent<v8::Function> Constructor;
};

#endif
