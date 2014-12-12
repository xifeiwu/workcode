function toJSON()
{
    console.log(arguments);
    console.log(JSON.stringify(arguments));
    console.log(JSON.stringify(arguments[1]));
}
args ="{}"; 
toJSON("ddd", ["abc", "def", "ghi"]);
