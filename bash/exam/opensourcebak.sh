#!/bin/sh
 
#/etc/init.d/mysqld stop   #执行备份前先停止MySql，防止有数据正在写入，备份出错           

#备份规划：在本地新建一个文件夹备份当天的文件，文件名格式：2013-04-0。
#文件夹下的mysql存放数据库的备份文件，project目前未使用。
#对mysql文件夹打包，找到项目目录打包并复制到备份文件夹下，bak.log文件记录备份信息。
MAXDAYS=15                     		#MAXDAYS=15代表删除15天前的备份，即只保留最近15天的备份
BK_DIR=/root/BackUp/		   	#备份文件存放路径:/root/BackUp/
SUBDIR_NAME=`date +%Y-%m-%d`		#每一天的信息存储到一个文件夹
BK_SUBDIR=$BK_DIR$SUBDIR_NAME/		#
MYSQL_DIR="$BK_SUBDIR"mysql
PROJECT_DIR="$BK_SUBDIR"project
BK_LOG="$BK_SUBDIR"bak.log   		#日志文件


LINUX_USER=root           		#系统用户名
DB_USER=root        			#数据库
DB_PASSWORD=ztb2012    			#数据库密码:iscas1006ztb

FTP_ADDR=192.168.161.222		#备份ftp数据库
FTP_USER=xifei				#ftp用户名:sunlv
FTP_PASSWD=iscasztb			#ftp密码:13u579
FTP_ROOTDIR=/ 				#wikidb存放目录:/analysis/wikidb/server06
FTP_TMP=/tmp/tmp.txt 			#存放备份服务器目录

if [ ! -e "${BK_DIR}" ]; then 		#检查主文件夹是否存在
   			mkdir -p "${BK_DIR}"
		fi
chown -R $LINUX_USER:$LINUX_USER $BK_DIR  #更改备份数据库文件的所有者

if [ ! -e "${BK_SUBDIR}" ]; then 	#检查子文件夹是否存在
   			mkdir -p "${BK_SUBDIR}"
		fi
if [ ! -e "${MYSQL_DIR}" ]; then 	#检查子文件夹下的数据库目录是否存在
   			mkdir -p "${MYSQL_DIR}"
		fi
if [ ! -e "${PROJECT_DIR}" ]; then 	#检查子文件夹的工程目录是否存在
   			mkdir -p "${PROJECT_DIR}"
		fi

if [ ! -e "${BK_LOG}" ]; then 		#检查日志文件是否存在
   			touch "${BK_LOG}"
		fi
#获取ftp上wiki备份中所有文件夹
function lsftp
{
    ftp -n $FTP_ADDR <<FTP                                                                                                                        
user $FTP_USER $FTP_PASSWD 
cd $FTP_ROOTDIR                                                                                                                                              
ls                                                                                                                            
FTP
}

#在ftp上新建文件夹
function ftpmkdir
{
    ftp -n $FTP_ADDR <<EOF
user $FTP_USER $FTP_PASSWD
cd $FTP_ROOTDIR
mkdir $1
EOF
}
#在ftp上删除文件夹
function ftprmdir
{
    ftp -n $FTP_ADDR <<EOF
user $FTP_USER $FTP_PASSWD
cd $FTP_ROOTDIR
rmdir $1
EOF
}
#上传文件
function upload
{
    ftp -n $FTP_ADDR <<EOF
user $FTP_USER $FTP_PASSWD
binary
prompt

cd $FTP_ROOTDIR"/"$1
mput $2
quit
EOF
}

#删除7天前的文件
function ftpdelfile
{
    ftp -n $FTP_ADDR <<EOF
user $FTP_USER $FTP_PASSWD

cd $FTP_ROOTDIR"/"$1
delete $2
quit
EOF
}

date=`date +%Y%m%d`			#获取当前日期
#本地备份数据库文件
cd $MYSQL_DIR
mysqldump -u $DB_USER --password=$DB_PASSWORD rwwb > rwwb.sql
mysqldump -u $DB_USER --password=$DB_PASSWORD community > community.sql
mysqldump -u $DB_USER --password=$DB_PASSWORD phpcms > phpcms.sql
cd ..
echo "=====================1.make tarball for database back up=======================" > $BK_LOG
tar -zcvf opensource-$date.sql.tar.gz mysql >> ${BK_LOG}
#本地备份工程文件
cd /opt/lampp
if [ -e "project.tar.gz" ]; then 		#检查日志文件是否存在
   			rm project.tar.gz
		fi
echo "====================2.make tarball for project back up========================" >> $BK_LOG
tar -zcf project.tar.gz web >> ${BK_LOG}
mv project.tar.gz "$BK_SUBDIR"/opensource-$date.project.tar.gz

#/etc/init.d/mysqld start  #备份完成后，启动MySql

#将备份文件上传到ftp服务器
#opensource备份夹是否存在，不存在则新建
lsftp|grep opensource > $FTP_TMP
if [ ! $? -eq 0 ]; then {
	ftpmkdir opensource
	echo "=====================3.opensource dir has been created=======================" >> $BK_LOG
}   
fi
cd $BK_SUBDIR
echo "======================3.upload file to ftp server======================" >> $BK_LOG
upload opensource opensource-$date.sql.tar.gz
upload opensource opensource-$date.project.tar.gz



#删除15天前的备份文件()
#删除本地15天前的备份
cd $BK_DIR		   	
dirname=`date -d -"$MAXDAYS"day +%Y-%m-%d`
rm -rf $dirname
#删除ftp服务器空间15天前的备份
deldate=`date -d -"$MAXDAYS"day +%Y%m%d` 
ftpdelfile opensource opensource-$deldate.sql.tar.gz
ftpdelfile opensource opensource-$deldate.project.tar.gz
