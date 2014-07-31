<?php
function create_table2($data, $border=1, $cellpadding=4, $cellspacing=4 ) {
  echo "<table border=\"".$border."\" cellpadding=\"".$cellpadding."\"
       cellspacing=\"".$cellspacing."\">";
  reset($data);
  $value = current($data);
  while ($value) {

     echo "<tr><td>".$value."</td></tr>\n";
     $value = next($data);
  }
  echo "</table>";
}



$my_array = array('Line one.','Line two.','Line three.');
create_table2($my_array, 3, 8, 8);
?>
