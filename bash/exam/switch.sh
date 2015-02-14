if [ $# -gt 2 -o $# -lt 1 ]; then
    echo A Parameter is needed.
    exit 1
fi
action=$1
echo $action

case $action in
    "init")
    echo "case init"
    ;;
    "clean")
    echo case clean
    ;;
    *)
    echo Parameter is limited to init or clean
    return 2
    ;;
esac
