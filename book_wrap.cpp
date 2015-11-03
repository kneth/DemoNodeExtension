#include <node.h>
#include <iostream>

#include "book_wrap.hpp"
#include "person_wrap.hpp"
#include "book.hpp"

using namespace v8;


Persistent<Function> BookWrap::Constructor;

BookWrap::BookWrap() : m_book() {}

BookWrap::~BookWrap() {}

// Add wrapper class to runtime environment
void BookWrap::Init(Handle<Object> exports) {
    Isolate* isolate = exports->GetIsolate();
    
    Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, BookWrap::New);
    tpl->SetClassName(String::NewFromUtf8(isolate, "Book"));
    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    NODE_SET_PROTOTYPE_METHOD(tpl, "add",      Add);
    NODE_SET_PROTOTYPE_METHOD(tpl, "length",   Length);
    NODE_SET_PROTOTYPE_METHOD(tpl, "lookup",   Lookup);
    NODE_SET_PROTOTYPE_METHOD(tpl, "each",     Each);
    NODE_SET_PROTOTYPE_METHOD(tpl, "apply",    Apply);

    tpl->InstanceTemplate()->SetIndexedPropertyHandler(Getter, Setter, 0, Deleter, Enumerator);

    Constructor.Reset(isolate, tpl->GetFunction());
    exports->Set(String::NewFromUtf8(isolate, "Book"), tpl->GetFunction());
}


// Create a new instance of the BookWrap class
void BookWrap::New(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);

    // constructor take no arguments
    if (args.IsConstructCall()) {
        // constructor take no arguments
        if (args.Length() == 0) {
            BookWrap* bw = new BookWrap();
            Book *p      = new Book();
            bw->m_book   = p;
            bw->Wrap(args.This());
            args.GetReturnValue().Set(args.This());
        }
        else {
            isolate->ThrowException(Exception::SyntaxError(String::NewFromUtf8(isolate, "No arguments ecpected")));
            args.GetReturnValue().SetUndefined();
        }
    }
    else {
        isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Called as plain function is not permitted")));
        args.GetReturnValue().SetUndefined();
    }
}


void BookWrap::Getter(uint32_t index, const PropertyCallbackInfo<Value>& info) {
    Isolate* isolate = info.GetIsolate();
    HandleScope scope(isolate);

    BookWrap* bw = ObjectWrap::Unwrap<BookWrap>(info.This());
    Book*     b  = bw->m_book;

    if (index >= b->size()) {
        isolate->ThrowException(Exception::RangeError(String::NewFromUtf8(isolate, "invalid row index")));
        info.GetReturnValue().SetUndefined();
    }
    else {
        Handle<Object> result = PersonWrap::New(isolate, b, index);
        info.GetReturnValue().Set(result);
    }
}

void BookWrap::Setter(uint32_t index, Local<Value> value, const PropertyCallbackInfo<Value>& info) {
    Isolate* isolate = info.GetIsolate();
    HandleScope scope(isolate);
    
    BookWrap* bw = ObjectWrap::Unwrap<BookWrap>(info.This());
    Book*     b  = bw->m_book;

    if (value->IsArray()) {
        if (index < b->size()) {
            Local<v8::Array> arr = Local<v8::Array>::Cast(value);
            if (arr->Length() == 3) {
                const String::Utf8Value firstname(arr->Get(0)->ToString());
                const String::Utf8Value lastname(arr->Get(1)->ToString());
                const time_t birthday = time_t(0.001*(*arr->Get(2))->NumberValue());
                Person *p = (*b)[index];
                p->firstname(*firstname);
                p->lastname(*lastname);
                p->birthday(birthday);
            }
            else {
                isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Three elements expected"))); 
                info.GetReturnValue().SetUndefined();
                return;               
            }
        }
        if (index == b->size()) {
            Local<v8::Array> arr = Local<v8::Array>::Cast(value);
            if (arr->Length() == 3) {
                const String::Utf8Value firstname(arr->Get(0)->ToString());
                const String::Utf8Value lastname(arr->Get(1)->ToString());
                const time_t birthday = time_t(0.001*(*arr->Get(2))->NumberValue());
                Person *p = new Person();
                p->firstname(*firstname);
                p->lastname(*lastname);
                p->birthday(birthday);
                b->add(p);
            }
            else {
                isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Three elements expected")));
                info.GetReturnValue().SetUndefined();
                return;              
            }
        }
        else {
            isolate->ThrowException(Exception::RangeError(String::NewFromUtf8(isolate, "Invalid index")));
            info.GetReturnValue().SetUndefined();
            return;
        }
    }
    else {
        isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Object expected")));
        info.GetReturnValue().SetUndefined();
        return;
    }
    info.GetReturnValue().SetUndefined();
}

void BookWrap::Deleter(uint32_t index, const PropertyCallbackInfo<Boolean>& info) {
    Isolate* isolate = info.GetIsolate();
    HandleScope scope(isolate);
    
    Book* b = ObjectWrap::Unwrap<BookWrap>(info.This())->m_book;
    try {
        b->remove(index);
        info.GetReturnValue().Set(Boolean::New(isolate, true));
    }
    catch (Exception e) {
        info.GetReturnValue().Set(Boolean::New(isolate, false));
    }
}

void BookWrap::Enumerator(const PropertyCallbackInfo<Array>& info) {
    Isolate* isolate = info.GetIsolate();
    HandleScope scope(isolate);
    
    Book* b = ObjectWrap::Unwrap<BookWrap>(info.This())->m_book;

    Handle<v8::Array> result = v8::Array::New(isolate, b->size());
    for(size_t i=0; i<b->size(); ++i) {
        result->Set(i, Integer::New(isolate, i));
    }
    info.GetReturnValue().Set(result);
}

void BookWrap::Length(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = args.GetIsolate();
    HandleScope scope(isolate);

    BookWrap* bw = ObjectWrap::Unwrap<BookWrap>(args.This());
    const int count = bw->m_book->size();
    Local<Integer> result = Integer::New(isolate, count);
    args.GetReturnValue().Set(result);
}


void BookWrap::Each(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = args.GetIsolate();
    HandleScope scope(isolate);

    Book* book = ObjectWrap::Unwrap<BookWrap>(args.This())->m_book;

    if (args.Length() == 1) {
        if (args[0]->IsFunction()) {
            Local<Function> fun = Local<Function>::Cast(args[0]);
            for(uint32_t i = 0; i < book->size(); ++i) {
                Local<Object> pw = PersonWrap::New(isolate, book, i);
                Local<Value> argv[1] = { pw };
                fun->Call(Null(isolate), 1, argv);
            }
            args.GetReturnValue().SetUndefined();
            return;
        }
        else {
            isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Function expected")));
            args.GetReturnValue().SetUndefined();
            return;
        }
    }
    else {
        isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "One argument expected")));
        args.GetReturnValue().SetUndefined();
        return;
            
    }
}

void BookWrap::Apply(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = args.GetIsolate();
    HandleScope scope(isolate);

    Local<Function> fun = Local<Function>::Cast(args[0]);
    Handle<Value> argv[] = { args.This() };
    TryCatch trycatch;
    Handle<Value> v = fun->Call(Null(isolate), 1, argv);
    if (trycatch.HasCaught()) {
        trycatch.ReThrow();
    }
    args.GetReturnValue().Set(v);
}

void BookWrap::Add(const v8::FunctionCallbackInfo<v8::Value>& args) {
    Isolate* isolate = args.GetIsolate();
    HandleScope scope(isolate);

    if (args.Length() == 1) {
        BookWrap* bw = ObjectWrap::Unwrap<BookWrap>(args.This());
        Book* b      = bw->m_book;
        PersonWrap* pw = ObjectWrap::Unwrap<PersonWrap>(args[0]->ToObject());
        b->add(pw->m_person);
    }
    else {
        isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Object expected")));
    }
    args.GetReturnValue().SetUndefined();
}


void BookWrap::Lookup(const v8::FunctionCallbackInfo<v8::Value>& args) {
    Isolate* isolate = args.GetIsolate();
    HandleScope scope(isolate);
    
    if (args.Length() == 1) {
        if (args[0]->IsString()) {
            const String::Utf8Value s(args[0]->ToString());
            Book* b = ObjectWrap::Unwrap<BookWrap>(args.This())->m_book;
            try {
                Person* p = b->lookup(*s);
                Local<Object> obj = PersonWrap::NewInstance();
                PersonWrap* pw = ObjectWrap::Unwrap<PersonWrap>(obj);
                pw->m_person = p;
                args.GetReturnValue().Set(obj);
            }
            catch (...) {
                isolate->ThrowException(Exception::RangeError(String::NewFromUtf8(isolate, "Not found")));
                args.GetReturnValue().SetUndefined();
            }
        }
        else {
            isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "String expected")));
            args.GetReturnValue().SetUndefined();
        }
    }
    else {
        isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "One argument expected")));
        args.GetReturnValue().SetUndefined();
    }
}
