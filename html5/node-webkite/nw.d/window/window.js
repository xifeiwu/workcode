// Load native UI library
var gui = require('nw.gui'); //or global.window.nwDispatcher.requireNwGui() (see https://github.com/rogerwang/node-webkit/issues/707)

// Get the current window
var win = gui.Window.get();

// Listen to the minimize event
win.on('minimize', function() {
  console.log('Window is minimized');
});

// Minimize the window
win.minimize();

// Unlisten the minimize event
win.removeAllListeners('minimize');

// Create a new window and get it
var new_win = gui.Window.get(
  window.open('https://github.com')
);

// And listen to new window's focus event
new_win.on('focus', function() {
  console.log('New window is focused');
});
