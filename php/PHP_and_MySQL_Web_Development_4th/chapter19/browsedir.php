<html>
<head>
  <title>Browse Directories</title>
</head>
<body>
<h1>Browsing</h1>
<?php
  $current_dir = 'upload/';
  $dir = opendir($current_dir);

  echo "<p>Upload directory is $current_dir</p>";
  echo '<p>Directory Listing:</p><ul>';
  
  while(false != ($file = readdir($dir)))
    //strip out the two entries of . and ..
  	if($file != "." && $file != "..")
  	{
    	  echo "<li>$file</li>";
  	}
  echo '</ul>';
  closedir($dir);
?>
</body>
</html>
