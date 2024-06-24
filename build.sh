#!/bin/bash
# set -x
set -o pipefail
CUR_DIR=$(cd "$(dirname $0)";pwd)

function warning() {
    local yellow="33m"
    echo -e "\033[${yellow}$* \033[0m" 2>&1
}

function error() {
    local red="31m"
    echo -e "\033[${red}$* \033[0m" 1>&2
    exit 1
}

function info() {
    local blue="94m"
    echo -e "\033[${blue}$* \033[0m" 1>&2
}

function success() {
    local green="32m"
    echo -e "\033[${green}$* \033[0m" 2>&1
}

function reset()
{
    PROC_TO_KILL=("skt_server.exe" "skt_client.exe")
    for proc in "${PROC_TO_KILL[@]}"; do
        proc_ids=($(pgrep -u $(whoami) "$proc"))

        if [ ${#proc_ids[@]} -eq 0 ]; then
            info "Process $proc cleanup successfully"
            continue
        fi

        for pid in "${proc_ids[@]}"; do
            sudo kill -9 "$pid" &> /dev/null
            warning "Send force terminate signal to process $pid"
        done
    done
    rm -f *.exe
}

trap 'reset ; error User canceled' INT

echo "1. build exe"
gcc -o skt_client.exe skt_client.c
if [ $? -ne 0 ]; then error "build exe failed"; fi
gcc -o skt_server.exe skt_server.c
if [ $? -ne 0 ]; then error "build exe failed"; fi

echo "2. launch server"
./skt_server.exe &
sleep 1

echo "3. launch client"
./skt_client.exe
if [ $? -ne 0 ]; then reset; error "launch client failed"; fi

echo "4. reset envrionment"
rm -f *.exe

success "Test done!"
