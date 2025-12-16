#!/bin/bash
# Simple script compile and run TCP server
PORT=${1:-9000}
ROOT_DIR=${2:-$(pwd)}

make
echo "Starting server on port $PORT with root $ROOT_DIR ..."
./tcp $PORT $ROOT_DIR
