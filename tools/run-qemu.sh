#!/bin/bash -e

while test $# -gt 0
do
    case "$1" in
        --gdb)
            echo "[*] Running with gdb"
            #terminator -e "sleep 1 && ./gdb-attach.sh" 2>/dev/null &
            CMD_GDB="cd ~/Documents/2018Spring_Purdue/OperatingSystem/xinu-spring2018/tools/ && sleep 1 && ./gdb-attach.sh"
            osascript -e "tell application \"Terminal\" to do script \"$CMD_GDB\"" 2>/dev/null &
            GDB_PID=$!
            DBG="-s -S"
            ;;
        *)
            echo "[ERROR] Unknown option"
            echo "[USAGE] $0 [--gdb]"
            exit
            ;;
    esac
    shift
done

cmds="qemu-system-i386 -cpu qemu32 -m 256M $DBG --nographic -kernel ../compile/xinu.bin"
echo "[*] Running: $cmds"
$cmds

kill -9 $GDB_PID
