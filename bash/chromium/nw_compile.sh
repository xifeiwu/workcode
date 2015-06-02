#!/bin/bash
if [ $# -lt 1 ]; then
    echo Working Directory Must Be Set By First Parameter.
    exit 1
fi
read -p "Working Directory Is: $(cd $1; pwd) [y/n] " yn
if [ "${yn}" == "y" -o "${yn}" == "Y" ]; then
    if [ ! -d $1 ]; then
        mkdir -p $1 || exit 1
    fi
else
    echo Wrong Choice.
    exit 1
fi
cd $1
WORKDIR=`pwd`
echo Change To Directory ${WORKDIR}

if [ -d nw-gitlab ]; then
    echo Directory nw-gitlab Alreay Exist.
else
    if [ ! -f nw-gitlab.tar.gz ]; then  
        scp box@192.168.162.142:/home/box/Workspace/Public/code/nw-gitlab.tar.gz ./
    fi
    tar -zxvf nw-gitlab.tar.gz
    if [ ! $? ]; then
        echo tar -zxvf nw-gitlab.tar.gz Error.
        exit 2
    fi
fi
if [ -d depot_tools ]; then
    echo Directory depot_tools Alreay Exist.
else
    if [ ! -f depot_tools_20140731.tar.gz ]; then
        scp box@192.168.162.142:/home/box/Workspace/Public/code/depot_tools_20140731.tar.gz ./
    fi
    tar -zxvf depot_tools_20140731.tar.gz
    if [ ! $? ]; then
        echo tar -zxvf depot_tools_20140731.tar.gz Error.
        exit 2
    fi
fi
export PATH=${WORKDIR}/depot_tools:$PATH

cd ${WORKDIR}/nw-gitlab/src
#nw v8 blink node breakpad
REPO_DIRS=(
content/nw
v8
third_party/WebKit
third_party/node
breakpad/src
)
for ((i=0; i<${#REPO_DIRS[@]}; i++))
do
    pushd ${REPO_DIRS[$i]}
    git pull origin runtime-dev
    popd
done

echo Install Build Dependent.
sudo build/install-build-deps.sh 
export GYP_GENERATORS='ninja'
./build/gyp_chromium content/content.gyp
ninja -C out/Release nw -j4
if [ ! $0 ]; then
    echo Build Error During ninja -C out/Release nw -j4
    exit 3
fi
