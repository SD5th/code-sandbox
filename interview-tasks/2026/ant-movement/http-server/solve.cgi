#!/bin/sh

X="${QUERY_STRING#*startcell_x=}"
X="${X%%&*}"

Y="${QUERY_STRING#*startcell_y=}"
Y="${Y%%&*}"

FUNC="${QUERY_STRING#*function=}"
FUNC="${FUNC%%&*}"

LIMIT="${QUERY_STRING#*limit=}"
LIMIT="${LIMIT%%&*}"

cd /app
./solve-ant-problem --x-start "$X" --y-start "$Y" --function "$FUNC" --limit "$LIMIT"

rm /app/http-server/*.png
mv *.png index.html /app/http-server/ 2>/dev/null || true

echo "Status: 303 See Other"
echo "Location: /"
echo ""