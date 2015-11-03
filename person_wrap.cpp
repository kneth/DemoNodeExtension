#include <node.h>

#include <iostream>
#include <string.h>

#include "person_wrap.hpp"
#include "person.hpp"

using namespace v8;

Persistent<Function> PersonWrap::Constructor;

PersonWrap::PersonWrap() : m_person() {}

PersonWrap::~PersonWrap() {}

// Add wrapper class to runtime environment
void PersonWrap::Init(Handle<Object> exports) {
    Isolate* isolate = exports->GetIsolate();
    
    Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, PersonWrap::New);
    tpl->SetClassName(String::NewFromUtf8(isolate, "Person"));
    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    // Getters and setters
    tpl->InstanceTemplate()->SetAccessor(String::NewFromUtf8(isolate, "firstname"), PersonWrap::FirstnameGetter, PersonWrap::FirstnameSetter);
    tpl->InstanceTemplate()->SetAccessor(String::NewFromUtf8(isolate, "lastname"),  PersonWrap::LastnameGetter,  PersonWrap::LastnameSetter);
    tpl->InstanceTemplate()->SetAccessor(String::NewFromUtf8(isolate, "birthday"),  PersonWrap::BirthdayGetter,  PersonWrap::BirthdaySetter);


    // Methods
    NODE_SET_PROTOTYPE_METHOD(tpl, "toString", PersonWrap::ToString);

    Constructor.Reset(isolate, tpl->GetFunction());
    exports->Set(String::NewFromUtf8(isolate, "Person"), tpl->GetFunction());
}


// Create a new instance of the PersonWrap class
void PersonWrap::New(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);

    // constructor take no arguments
    if (args.IsConstructCall()) {
        if (args.Length() == 0) {
            PersonWrap* pw = new PersonWrap();
            Person *p      = new Person();
            pw->m_person   = p;
            pw->Wrap(args.This());
            args.GetReturnValue().Set(args.This());
        }
        else if (args.Length() == 1) { // we have a Person object allocated
            PersonWrap* pw = new PersonWrap();
            pw->Wrap(args.This());
            args.GetReturnValue().Set(args.This());
        }
        else {
            isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Zero or one argument expected")));
            args.GetReturnValue().SetUndefined();
        }
    }
    else {
        isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Called as plain function is not permitted")));
        args.GetReturnValue().SetUndefined();
    }
}


// Create a new instance of the PersonWrap from a book
Handle<Object> PersonWrap::New(Isolate* isolate, Book* b, uint32_t index) {
    EscapableHandleScope scope(isolate);

    // Get a new instance of PersonWrap
    Handle<Value> argv[] = { Boolean::New(isolate, true) };
    Local<Function> cons = Local<Function>::New(isolate, Constructor);
    Handle<Object> obj = cons->NewInstance(1, argv);
    PersonWrap* pw = PersonWrap::Unwrap<PersonWrap>(obj);

    pw->m_person = (*b)[size_t(index)];

    return scope.Escape(obj);
}

// Create a wrapper for a Person object
Handle<Object> PersonWrap::NewInstance() {
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);

    Local<Value> argv[] = { Boolean::New(isolate, true) };
    Local<Function> cons = Local<Function>::New(isolate, Constructor);
    Handle<Object> obj = cons->NewInstance(1, argv);
    return obj;
}


void PersonWrap::FirstnameGetter(Local<String> property, const PropertyCallbackInfo<Value>& info) {
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);

    PersonWrap *pw = ObjectWrap::Unwrap<PersonWrap>(info.This());
    Person *p = pw->m_person;

    info.GetReturnValue().Set(String::NewFromUtf8(isolate, p->firstname().c_str()));
}

void PersonWrap::FirstnameSetter(Local<String> property, Local<Value> value, const PropertyCallbackInfo<void>& info) {
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);

    PersonWrap *pw = ObjectWrap::Unwrap<PersonWrap>(info.This());
    Person *p = pw->m_person;

    if (value->IsString()) {
        const String::Utf8Value v(value->ToString());
        p->firstname(*v);
    }
    else {
        isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "String expected")));
        info.GetReturnValue().SetUndefined();
    }
}

void PersonWrap::LastnameGetter(Local<String> property, const PropertyCallbackInfo<Value>& info) {
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);

    PersonWrap *pw = ObjectWrap::Unwrap<PersonWrap>(info.This());
    Person *p = pw->m_person;

    info.GetReturnValue().Set(String::NewFromUtf8(isolate, p->lastname().c_str()));
}

void PersonWrap::LastnameSetter(Local<String> property, Local<Value> value, const PropertyCallbackInfo<void>& info) {
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);

    PersonWrap *pw = ObjectWrap::Unwrap<PersonWrap>(info.This());
    Person *p = pw->m_person;

    if (value->IsString()) {
        const String::Utf8Value v(value->ToString());
        p->lastname(*v);
    }
    else {
        isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "String expected")));
        info.GetReturnValue().SetUndefined();
    }
}

void PersonWrap::BirthdayGetter(Local<String> property, const PropertyCallbackInfo<Value>& info) {
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);

    PersonWrap *pw = ObjectWrap::Unwrap<PersonWrap>(info.This());
    Person *p = pw->m_person;

    info.GetReturnValue().Set(Date::New(isolate, double(p->birthday())));
}


void PersonWrap::BirthdaySetter(Local<String> property, Local<Value> value, const PropertyCallbackInfo<void>& info) {
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);

    PersonWrap *pw = ObjectWrap::Unwrap<PersonWrap>(info.This());
    Person *p = pw->m_person;

    if (value->IsDate()) {
        time_t epoc = time_t(0.001*value->NumberValue());
        p->birthday(epoc);
    }
    else {
        isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Date expected")));
        info.GetReturnValue().SetUndefined();
    }
}


// Convert object to string
void PersonWrap::ToString(const v8::FunctionCallbackInfo<v8::Value>& args) {
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);
    
    if (args.Length() == 0) {
        PersonWrap* pw = ObjectWrap::Unwrap<PersonWrap>(args.This());
        const std::string s = pw->m_person->to_str();
        Handle<String> r = String::NewFromUtf8(isolate, s.c_str());
        args.GetReturnValue().Set(r);
    }
    else {
        isolate->ThrowException(Exception::SyntaxError(String::NewFromUtf8(isolate, "No arguments expected")));
        args.GetReturnValue().SetUndefined();
    }
}
