var funstuff = require('./build/Release/funstuff');

var book = new funstuff.Book();

console.log('Book: ' + book.length());

var person = new funstuff.Person();
person.firstname = "Isaac";
person.lastname = "Asimov";
person.birthday = new Date(1917, 11, 16);  // month is zero-based
book.add(person);

console.log('Book: ' + book.length());
var p1 = book.lookup("Isaac");
console.log("Person 1: " + p1);

var p2 = book[0];
console.log("Person 2: " + p2);

book.each(function (p) {
    console.log("Firstname: " + p.firstname);
});