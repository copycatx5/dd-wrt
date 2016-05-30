#!/bin/sh

nvram show |cut -d'=' -f1>/tmp/nvram-keys.txt

cat /tmp/nvram-keys.txt | while read line
do
        nvram unset "$line"
done

cat /etc/nvram-miwifi-stock.txt | while read line
do
        nvram set "$line"
done

echo "----if NO error happened, execute \"nvram commit\" and reboot---"

