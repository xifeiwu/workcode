for dir in `(ls)`
do
#echo $dir 
#continue
  if [ -d $dir ]; then
    pushd $dir > /dev/null
    if [ $(find . -name binding.gyp | wc -l) -ge 1 ] ; then
      echo 
      echo **--${dir}--**
      find . -name binding.gyp
    fi
    popd > /dev/null
  fi
done

