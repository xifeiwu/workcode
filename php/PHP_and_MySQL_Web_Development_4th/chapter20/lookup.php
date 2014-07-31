<html>
<head>
  <title>Stock Quote from NASDAQ</title>
</head>
<body>
<?php
  // choose stock to look at
  $symbol='AMZN';
  echo "<h1>Stock Quote for $symbol</h1>";

  $theurl="http://www.amex.com/equities/listCmp/EqLCDetQuote.jsp?Product_Symbol=$symbol";

  if (!($contents = file_get_contents($theurl)))
  {
    echo 'Could not open URL';
    exit;
  }
//  $file="contents";
//  $fh=fopen($file, "w");
//  echo "length:".fwrite($fh, $contents);
//  fclose($fh);
  // find the part of the page we want and output it
  $pattern = '(\\$[0-9 ]+\\.[0-9]+)';
  
  if (eregi($pattern, $contents, $quote))
  { 
    echo "<p>$symbol was last sold at: ";
    echo $quote[1];
    echo '</p>';
  } 
  else 
  {
    echo '<p>No quote available</p>';
  };
 
  
  // acknowledge source
  echo '<p>'
       .'This information retrieved from <br />'
       ."<a href=\"$theurl\">$theurl</a><br />"
       .'on '.(date('l jS F Y g:i a T')).'</p>';
?>
</body>
</html>
