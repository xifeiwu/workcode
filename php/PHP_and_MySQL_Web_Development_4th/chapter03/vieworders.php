<?php
//create short variable name
$DOCUMENT_ROOT = $_SERVER['DOCUMENT_ROOT'];
$DOCUMENT_ROOT = exec("pwd");

$orders= file("$DOCUMENT_ROOT/../chapter02/orders.txt");

$number_of_orders = count($orders);
echo "<p>DOCUMENT_ROOT: ".$DOCUMENT_ROOT."</p>";
echo "<p>number_of_orders: ".$number_of_orders."</p>";
if ($number_of_orders == 0) {
  echo "<p><strong>No orders pending.
       Please try again later.</strong></p>";
}

for ($i=0; $i<$number_of_orders; $i++) {
  echo $orders[$i]."<br />";
}
?>
