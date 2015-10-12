var petalCount = 15;
function LateBloomer() {
  this.petalCount = Math.ceil(Math.random() * 12) + 1;
}

// Declare bloom after a delay of 1 second
LateBloomer.prototype.bloom = function() {
//  setTimeout(this.declare.bind(this), 1000);

// The follow can only be used in browser.
//  setTimeout(LateBloomer.prototype.declare, 100);

//  setTimeout(LateBloomer.prototype.declare.bind(this), 100);

  setTimeout(this.declare.call(this), 500);
};

LateBloomer.prototype.declare = function() {
  console.log('I am a beautiful flower with ' +
    this.petalCount + ' petals!');
};

var flower = new LateBloomer();
flower.bloom();  // after 1 second, triggers the 'declare' method
