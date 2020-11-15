#### How to set

* /etc/rc.local
<pre>
_IP=$(hostname -I) || true
if [ "$_IP" ]; then
  printf "My IP address is %s\n" "$_IP"
fi

/usr/bin/my_usb
/home/pi/transfer & 
cat /dev/ttyGS0 >> bulk.log &

exit 0
</pre>
