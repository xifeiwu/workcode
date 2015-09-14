echo `dirname ${#BASH_SOURCE[@]}`
CUR_DIR=$(cd `dirname ${BASH_SOURCE[0]}`; pwd)
echo Current dir: $CUR_DIR
parameter=$1
default=default
echo parameter:${parameter}
echo parameter :-: ${parameter:-${default}}
echo parameter :=: ${parameter:=${default}}
echo parameter :+: ${parameter:+${default}}
echo parameter :?: ${parameter:?${default}}

echo
echo for string operation
echo ${#default}
echo ${default:2:4}
echo ${default#def}
echo ${default#aul}
