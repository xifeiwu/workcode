<?
$prices = array( 'Tires' => 100 );
$prices['Oil'] = 10;
$prices['Spark Plugs'] = 4;

while ($element = each($prices)) {
  echo $element['key'];
  echo " - ";
  echo $element['value'];
  echo "<br />";
}
?>