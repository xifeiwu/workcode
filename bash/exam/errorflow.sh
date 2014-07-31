if cp myfile myfile.bak >/dev/null 2> then
echo "good copy."
else
echo "bad copy."
fi
