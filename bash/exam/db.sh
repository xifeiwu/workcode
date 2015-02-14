
if [ $# -gt 2 -o $# -lt 1 ]; then
    echo A Parameter is needed.
    exit 1
fi
action=$1

isInstallSqlite3=`which sqlite3`
if ! which sqlite3 > /dev/null; then
    echo The program 'sqlite3' is currently not installed. You can install it by typing:
    echo sudo apt-get install sqlite3
    exit 2
fi

DBFILE=$PWD/db.sqlite3


function initdb(){
    if [ -e $DBFILE ]; then
        echo $DBFILE already exist. Please remove First.
        return 1
    fi
    local initSQL="BEGIN TRANSACTION;\
CREATE TABLE feedback (id INTEGER PRIMARY KEY, title TEXT, content TEXT, username TEXT, useremail TEXT, userphone NUMERIC);\
CREATE TABLE category (logoPath TEXT, id INTEGER PRIMARY KEY, type TEXT, desc TEXT);\
CREATE TABLE contact (URI TEXT, photoPath TEXT, id INTEGER PRIMARY KEY, name TEXT, phone NUMERIC, phone2 NUMERIC, phone3 NUMERIC, phone4 NUMERIC, phone5 NUMERIC, sex TEXT, age NUMERIC, email TEXT, email2 TEXT,createTime TEXT,createDev TEXT, lastAccessTime TEXT,lastAccessDev TEXT,lastModifyTime TEXT,lastModifyDev TEXT,others TEXT,is_deleted TEXT default 0);\
CREATE TABLE devices (lastSyncTime TEXT, resourcePath TEXT, ip TEXT, name TEXT, id INTEGER PRIMARY KEY, device_id TEXT, account TEXT);\
CREATE TABLE document (URI TEXT, postfix TEXT, filename TEXT, id INTEGER PRIMARY KEY, size TEXT, path TEXT, project TEXT, createTime TEXT,createDev TEXT, lastAccessTime TEXT,lastAccessDev TEXT,lastModifyTime TEXT,lastModifyDev TEXT,others TEXT,is_deleted TEXT default 0);\
CREATE TABLE music (URI TEXT, postfix TEXT, filename TEXT, id INTEGER PRIMARY KEY, size TEXT, path TEXT,format TEXT,bit_rate TEXT,frequency TEXT,TIT2 TEXT,TPE1 TEXT,TALB TEXT,TDRC TEXT,APIC TEXT,track TEXT,TXXX TEXT,COMM TEXT,createTime TEXT,createDev TEXT, lastAccessTime TEXT,lastAccessDev TEXT,lastModifyTime TEXT,lastModifyDev TEXT,others TEXT,is_deleted TEXT default 0);\
CREATE TABLE picture (URI TEXT, postfix TEXT, filename TEXT, id INTEGER PRIMARY KEY, size TEXT, path TEXT, location TEXT, createTime TEXT,createDev TEXT, lastAccessTime TEXT,lastAccessDev TEXT,lastModifyTime TEXT,lastModifyDev TEXT,others TEXT,is_deleted TEXT default 0);\
CREATE TABLE tags (id INTEGER PRIMARY KEY, file_URI TEXT, tag TEXT);\
CREATE TABLE video (URI TEXT, postfix TEXT, filename TEXT, id INTEGER PRIMARY KEY, size TEXT, path TEXT, directorName TEXT, actorName TEXT, createTime TEXT,createDev TEXT, lastAccessTime TEXT,lastAccessDev TEXT,lastModifyTime TEXT,lastModifyDev TEXT,others TEXT,format_long_name TEXT,width INTEGER ,height INTEGER ,display_aspect_ratio TEXT,pix_fmt TEXT,duration TEXT,major_brand TEXT,minor_version TEXT,compatible_brands TEXT,is_deleted TEXT default 0);\
CREATE TABLE other (filename TEXT, postfix TEXT, path TEXT, URI TEXT, createTime TEXT, id INTEGER PRIMARY KEY,others TEXT, size TEXT,createDev TEXT, lastAccessTime TEXT,lastAccessDev TEXT,lastModifyTime TEXT,lastModifyDev TEXT,is_deleted TEXT default 0);\

INSERT INTO feedback VALUES(0, '名称', '内容','用户名','邮箱', 123456);\
INSERT INTO category VALUES('./frontend-dev/images/contacts.jpg',101,'Contact','联系人');\
INSERT INTO category VALUES('./frontend-dev/images/pictures.png',102,'Picture','图片');\
INSERT INTO category VALUES('./frontend-dev/images/videos.png',103,'Video','视频');\
INSERT INTO category VALUES('./frontend-dev/images/documents.jpg',104,'Document','文档');\
INSERT INTO category VALUES('./frontend-dev/images/music.png',105,'Music','音乐');\
INSERT INTO category VALUES('./frontend-dev/images/other.jpg',106,'Other','其他');\
INSERT INTO category VALUES('./frontend-dev/images/devices.jpg',107,'Devices','设备');\
COMMIT;"
    echo $initSQL | sqlite3 $DBFILE
    if [ $? ]; then
        echo Init Sqlite DataBase $DBFILE Success.
    else
        echo Init Sqlite DataBase $DBFILE Fail.
    fi
}

case $action in
    "init")
    if ! initdb ; then
        exit 3
    fi
    ;;
    "remove")
    if [ -e $DBFILE ]; then
        rm -rf $DBFILE
        echo remove Sqlite DataBase $DBFILE Success.
    else
        echo Sqlite DataBase $DBFILE Does Not Exist.
    fi
    ;;
    *)
    echo Parameter is limited to init or remove
    exit 3
    ;;
esac
