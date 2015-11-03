var funstuff = require('./build/Release/funstuff');

var person = new funstuff.Person();
person.firstname = "Isaac";
person.lastname = "Asimov";
person.birthday = new Date(1917, 11, 16);  // month is zero-based
console.log("Person: " + person.toString());
