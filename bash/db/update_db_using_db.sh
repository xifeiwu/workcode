#!/bin/bash

if [ -f './config.sh' ]; then
  source ./config.sh
else
  echo ./config.sh not found
fi


sql='SELECT DATE(user.`CREATE_DATE`) AS date, IFNULL(user.`WM`,-1) AS wm, COUNT(*) AS number FROM benew_user.`USER` user 
WHERE user.`CREATE_DATE` >= DATE('\'${TARGET_DATE}\'') AND user.`CREATE_DATE`<DATE(date_add('\'${TARGET_DATE}\'',interval 1 day)) 
GROUP BY DATE(user.`CREATE_DATE`), user.`WM` order by number desc'
echo $sql
result=`mysql --skip-column-names -u$PRODUCT_USER_NAME -p$PRODUCT_PASSWORD -h$PRODUCT_HOST -P$PRODUCT_PORT -e "$sql"`
line=''
values=''
count=1
for item in $result
do
   if [ `expr $count % 3` -eq 0 ]; then
      line='('${line}${item}')'
      values=$values${line},
      line=''
   else
      line=$line"'"$item"',"
   fi
   count=`expr $count + 1`
done
values=${values%?}
sql="insert into bn_register_sts(create_date, wm_code, num) values ${values}"
mysql -u$BI_USER_NAME -p$BI_PASSWORD -h$BI_HOST -P$BI_PORT $BI_DATABASE -e "$sql"
echo mysql -u$BI_USER_NAME -p$BI_PASSWORD -h$BI_HOST -P$BI_PORT $BI_DATABASE -e "$sql"
echo 

sql='select user.`USER_ID` from benew_user.`USER` as user where user.`CREATE_DATE` between DATE('\'${TARGET_DATE}\'') and DATE(date_add('\'${TARGET_DATE}\'', interval 1 day))'
echo $sql
result=`mysql --skip-column-names -u$PRODUCT_USER_NAME -p$PRODUCT_PASSWORD -h$PRODUCT_HOST -P$PRODUCT_PORT -e "$sql"`
line=''
values=''
count=1
for item in $result
do
   if [ `expr $count % 20` -eq 0 ]; then
      line='('${line}${item}')'
      values=$values${line},
      line=''
   else
      line=$line"'"$item"',"
   fi
   count=`expr $count + 1`
done
values=${values%?}
sql="insert into USER values ${values}"
# mysql -u$BI_USER_NAME -p$BI_PASSWORD -h$BI_HOST -P$BI_PORT $BI_DATABASE -e "$sql"
echo mysql -u$BI_USER_NAME -p$BI_PASSWORD -h$BI_HOST -P$BI_PORT $BI_DATABASE -e "$sql"

