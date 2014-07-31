#/bin/sh
t_encoding="utf-8"  
f_encoding="gbk"
FILES=`find . -type f | egrep "\.php$|\.css$"`
for file in ${FILES}
do
    echo "iconv  -f ${f_encoding} -t ${t_encoding}  ${file} -o ${file}"
    iconv  -f ${f_encoding} -t ${t_encoding}  ${file} -o ${file}
done
echo "success."
