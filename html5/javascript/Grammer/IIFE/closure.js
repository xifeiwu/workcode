(function(exports) {
  var test_dom = document.createElement("div");
  test_dom.id = "closure";
  test_dom.innerHTML = 
  `
  <ul id="myList">
    <li><a>Item 1</a></li>
    <li><a>Item 2</a></li>
    <li><a>Item 3</a></li>
  </ul>
  `;
  document.body.appendChild(test_dom);

  // This doesn't work like you might think, because the value of `i` never
  // gets locked in. Instead, every link, when clicked (well after the loop
  // has finished executing), alerts the total number of elements, because
  // that's what the value of `i` actually is at that point.
/*
  var elems = document.getElementsByTagName('a');
  for (var i = 0; i < elems.length; i++) {
    elems[i].addEventListener('click',
    function(e) {
      e.preventDefault();
      alert('I am link #' + i);
    },
    'false');
  }
  */
  // This works, because inside the IIFE, the value of `i` is locked in as
  // `lockedInIndex`. After the loop has finished executing, even though the
  // value of `i` is the total number of elements, inside the IIFE the value
  // of `lockedInIndex` is whatever the value passed into it (`i`) was when
  // the function expression was invoked, so when a link is clicked, the
  // correct value is alerted.

  var elems = document.getElementsByTagName('a');
  for (var i = 0; i < elems.length; i++) { (function(lockedInIndex) {
      elems[i].addEventListener('click',
      function(e) {
        e.preventDefault();
        alert('I am link #' + lockedInIndex);
      },
      'false');
    })(i);
  }
  

  // You could also use an IIFE like this, encompassing (and returning) only
  // the click handler function, and not the entire `addEventListener`
  // assignment. Either way, while both examples lock in the value using an
  // IIFE, I find the previous example to be more readable.
/*
  var elems = document.getElementsByTagName('a');
  for (var i = 0; i < elems.length; i++) {
    elems[i].addEventListener('click', (function(lockedInIndex) {
      return function(e) {
        e.preventDefault();
        alert('I am link #' + lockedInIndex);
      };
    })(i), 'false');
  }
*/
})(window);
