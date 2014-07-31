<html>
<head>
  <title>Site submission results</title>
</head>
<body>
<h1>Site submission results</h1>
<?php

  // Extract form fields

  $url = $_REQUEST['url'];
  $email = $_REQUEST['email'];

  // Check the URL

  $url = parse_url($url);
  $host = $url['host'];
  echo "url:".json_encode($url);
  if(!($ip = gethostbyname($host)))
  {
    echo 'Host for URL does not have valid IP';
    exit;
  }

  echo "Host is at IP $ip <br>";  

  // Check the email address

  $email = explode('@', $email);
  $emailhost = $email[1];

  // note that the getmxrr() function is *not implemented* in 
  // Windows versions of PHP
  if (!getmxrr($emailhost, $mxhostsarr))
  {
    echo 'Email address is not at valid host';
    exit;   
  } 

  echo 'Email is delivered via: ';
  foreach ($mxhostsarr as $mx)
    echo "$mx "; 

  // If reached here, all ok

  echo '<br>All submitted details are ok.<br>';
  echo 'Thank you for submitting your site.<br>'
       .'It will be visited by one of our staff members soon.'  

  // In real case, add to db of waiting sites... 
?>
</body>
</html>
