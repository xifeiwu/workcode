for file in  `find . -name "*.js"`
do
    cat $file | grep "require" | grep "commonHandle"
    if [ $? -eq 0 ]; then
        echo Found in file: $file;
    fi
done
#| grep "require" | grep "device"
