<?php
function do_html_header($title) {
    // print an HTML header
    ?>
<html>
<head>
<title><?php echo $title;?></title>
<link rel="stylesheet" type="text/css" href="exam-parser.css">
<script type="text/javascript" src="exam-parser.js"></script>
<meta http-equiv="content-type" content="text/html; charset=UTF-8" />
</head>
<body>
<?php
}
function do_html_footer() {
    // print an HTML footer
    ?>
  </body>
</html>
<?php
}
do_html_header('test');
function echo_output($name, $str){
    echo $name.':'.$str.'<br>';
}
function test_php_function(){
    echo_output('dirname', dirname(__FILE__));
    echo_output('document root', $_SERVER['DOCUMENT_ROOT']);
    echo_output('php version', phpversion());
//     if(function_exists('floatval')){
//         echo_output('function floatval', ' exist');
//     }else{
//         echo_output('function floatval', ' not exist');        
//     }
//     if(function_exists('floatvadl')){
//         echo_output('function floatval', ' exist');
//     }else{
//         echo_output('function floatval', ' not exist');        
//     }


    
//     $a=array("a"=>"Cat","b"=>"Dog","c"=>"Horse");
//     print_r(array_change_key_case($a,CASE_UPPER));
    
    //phpinfo();
}
//test_php_function();
function test_function_htmlentities(){
    $str='<a href="test.html">测试页面</a>';
    echo htmlentities($str);
    $str='<a href="test.html">测试页面</a>';
    echo htmlspecialchars($str);
    $str = "John & 'Adams'";
    echo htmlentities($str, ENT_COMPAT);
    echo "<br />";
    echo htmlentities($str, ENT_QUOTES);
    echo "<br />";
    echo htmlentities($str, ENT_NOQUOTES);
}
//test_function_htmlentities();
function test_file_function(){
    $php_info = 'test';
    echo_output('phpinfo', $php_info);
    $file = fopen('./phpinfo', 'w');
    if(!$file){
        echo 'file is not create correct.<br>';
    }
    fwrite($file, $php_info);
    fclose($file);
}
function test_function_get_object_vars(){
    class Point2D {
        var $x, $y;
        var $label;
    
        function Point2D($x, $y)
        {
            $this->x = $x;
            $this->y = $y;
        }
    
        function setLabel($label)
        {
            $this->label = $label;
        }
    
        function getPoint()
        {
            return array("x" => $this->x,
            "y" => $this->y,
            "label" => $this->label);
        }
    }
    
    // "$label" is declared but not defined
    $p1 = new Point2D(1.233, 3.445);
    print_r(get_object_vars($p1));    
    $p1->setLabel("point #1");
    print_r(get_object_vars($p1));
}
//test_function_get_object_vars();
function test_function_var_dump(){
    $a = array(1, 2, array("a", "b", "c"));
    var_dump($a);
}
//test_function_var_dump();
function test_function_ob_get_contents(){    
    ob_start();    
    echo "Hello ";    
    $out1 = ob_get_contents();    
    echo "World";
    $out2 = ob_get_contents();    
    ob_end_clean();
    var_dump($out1, $out2);
    echo '<br>';
    echo $out1.' -- '.$out2;
    echo '<br>--------------------------------<br>';
    ob_start();    
    echo "Hello ";    
    $out1 = ob_get_contents();    
    echo "World";    
    $out2 = ob_get_contents();    
    ob_end_clean();
    var_dump($out1, $out2);
    echo '<br>';
    echo $out1.' -- '.$out2;
}
//test_function_ob_get_contents();
function test_function_addslash(){
    $str = "Who's John Adams?";
    echo $str . " This is not safe in a database query.<br />";
    echo addslashes($str) . " This is safe in a database query.";
}
//test_function_addslash();
function test_function_array_values(){
    $a=array("a"=>"Cat","b"=>"Dog","c"=>"Horse", 'd'=>array('e', 'f', 'g'));
    print_r(array_values($a));
}
//test_function_array_values();
function test_function_empty(){
    $var = 0;
    // 结果为 true，因为 $var 为空
    if (empty($var)) {
        echo '$var is either 0 or not set at all';
    }    
    // 结果为 false，因为 $var 已设置
    if (!isset($var)) {
        echo '$var is not set at all';
    }
}
//test_function_empty();
function test_function_get_defined_vars(){
    $b = array(1,1,2,3,5,8);
    $arr = get_defined_vars();
    echo '<br>';
    echo 'get defined vars:';print_r($arr);
    echo '<br>';
}
//test_function_get_defined_vars();
function test_function_func_get_args(){
    function foo()
    {
        $numargs = func_num_args();
        echo "Number of arguments: $numargs<br />\n";
        if ($numargs >= 2) {
            echo "Second argument is: " . func_get_arg(1) . "<br />\n";
        }
        $arg_list = func_get_args();
        for ($i = 0; $i < $numargs; $i++) {
            echo "Argument $i is: " . $arg_list[$i] . "<br />\n";
        }
        $args3 = array_slice($arg_list, 2, 3);
        echo '$args3:';print_r($args3); echo '<br>';
    }
    foo('a', 'b', 'c', 'd');
}
//test_function_func_get_args();
function test_call_user_func_array(){
    function foobar($arg, $arg2) {
        echo __FUNCTION__, " got $arg and $arg2\n";
    }
    class foo {
        function bar($arg, $arg2) {
            echo __METHOD__, " got $arg and $arg2\n";
        }
    }    
    // Call the foobar() function with 2 arguments
    call_user_func_array("foobar", array("one", "two"));
    echo '<br>';   
    // Call the $foo->bar() method with 2 arguments
    $foo = new foo;
    call_user_func_array(array($foo, "bar"), array("three", "four"));
}
//test_call_user_func_array();
/**
 * 把一个汉字转为unicode的通用函数，不依赖任何库，和别的自定义函数，但有条件
 * 条件：本文件以及函数的输入参数应该用utf－8编码，不然要加函数转换
 * 其实亦可轻易编写反向转换的函数，甚至不局限于汉字，奇怪为什么php没有现成函数
 * @author xieye
 *
 * @param {string} $word 必须是一个汉字，或代表汉字的一个数组(用str_split切割过)
 * @return {string} 一个十进制unicode码，如4f60，代表汉字 “你”
 */
function getUnicodeFromOneUTF8($word) {
    //获取其字符的内部数组表示，所以本文件应用utf-8编码！
    if (is_array( $word))
        $arr = $word;
    else
        $arr = str_split($word);
    //此时，$arr应类似array(228, 189, 160)
    //定义一个空字符串存储
    $bin_str = '';
    //转成数字再转成二进制字符串，最后联合起来。
    foreach ($arr as $value)
        $bin_str .= decbin(ord($value));
    //此时，$bin_str应类似111001001011110110100000,如果是汉字"你"
    //正则截取
    $bin_str = preg_replace('/^.{4}(.{4}).{2}(.{6}).{2}(.{6})$/','$1$2$3', $bin_str);
    //此时， $bin_str应类似0100111101100000,如果是汉字"你"
    //return bindec($bin_str); //返回类似20320， 汉字"你"
    return dechex(bindec($bin_str)); //如想返回十六进制4f60，用这句
}
function test_function_ord(){
    echo '汉'.ord('汉').'<br>';
    echo '语'.ord('语').'<br>';
    echo '汉'.getUnicodeFromOneUTF8('汉').'<br>';
    echo '语'.getUnicodeFromOneUTF8('语').'<br>';
    echo '你们'.getUnicodeFromOneUTF8('你们').'<br>';
    //echo '<br>split result:';
    //print_r(str_split('hello world'));
}
//test_function_ord();

function test_function_serialize(){
    class dog {    
        var $name;
        var $age;
        var $owner;    
        function dog($in_name="unnamed",$in_age="0",$in_owner="unknown") {
            $this->name = $in_name;
            $this->age = $in_age;
            $this->owner = $in_owner;
        }    
        function getage() {
            return ($this->age * 365);
        }    
        function getowner() {
            return ($this->owner);
        }    
        function getname() {
            return ($this->name);
        }
    }
    //实例化这个类
    $ourfirstdog = new dog("Rover",12,"Lisa and Graham");
    //用serialize函数将这个实例转化为一个序列化的字符串
    $dogdisc = serialize($ourfirstdog);
    print $dogdisc; //$ourfirstdog 已经序列化为字符串 O:3:"dog":3:{s:4:"name";s:5:"Rover";s:3:"age";i:12;s:5:"owner";s:15:"Lisa and Graham";}
    
    print '<BR>';
}
//test_function_serialize();
function test_function_parse_url(){
    $url = 'http://username:password@hostname/path?arg=value#anchor';
    print_r(parse_url($url));
    echo parse_url($url, PHP_URL_PATH);
}
//test_function_parse_url();

function test_predefine_variable(){
    echo_output('__LINE__', __LINE__);
    echo_output('__FILE__', __FILE__);
    echo_output('__DIR__', __DIR__);
    echo_output('__FUNCTION__', __FUNCTION__);
    echo_output('__CLASS__', __CLASS__);
    echo_output('__METHOD__', __METHOD__);
    echo_output('__NAMESPACE__', __NAMESPACE__);
}
test_predefine_variable();

do_html_footer()
?>