{
    'targets': [
        {
            'target_name': 'funstuff',
            'sources': [ 'person.cpp', 'person_wrap.cpp', 'book.cpp', 'book_wrap.cpp', 'funstuff.cpp' ],
            'xcode_settings': {
                'OTHER_CFLAGS': [ '-mmacosx-version-min=10.8', '-std=c++11', '-stdlib=libc++', '-fexceptions', '-frtti' ]
            }
        }
    ]
}
