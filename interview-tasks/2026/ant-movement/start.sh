#!/bin/sh

./solve-ant-problem --x-start "1000" --y-start "1000" --function "SUM_OF_DIGITS" --limit "25"

mv *.png ./http-server/

mv index.html ./http-server/

echo "HTTP-server will open on localhost."
echo "Access it through http://127.0.0.1:8085"

lighttpd -D -f /etc/lighttpd/lighttpd.conf