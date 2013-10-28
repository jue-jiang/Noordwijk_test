#!/bin/bash
KERNEL="linux-3.9.7-tc"
KERNELSEND="linux-sender"
#BACKEND=Debian-Squeeze-AMD64-root_fs

gnome-terminal --title="switch1" --command="./uml_switch/uml_switch -unix /tmp/switch1" &
sleep 1
gnome-terminal --title="switch2" --command="./uml_switch/uml_switch -unix /tmp/switch2" &
sleep 1

gnome-terminal --title="client_receive" --command="./$KERNEL mem=512M ubd0=fs/client_receiver.ext3 umid=local1 eth0=daemon,,unix,/tmp/switch1" &
sleep 1
gnome-terminal --title="forwarder" --command="./$KERNEL mem=512M ubd0=fs/forwarder.ext3 umid=local2 eth0=daemon,,unix,/tmp/switch1 eth1=daemon,,unix,/tmp/switch2" &
sleep 1
gnome-terminal --title="server_send" --command="./$KERNELSEND mem=512M ubd0=fs/server_send.ext3 umid=local3 eth0=daemon,,unix,/tmp/switch2" &
sleep 1
