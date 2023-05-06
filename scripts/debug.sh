#!/bin/bash

SCRIPT_DIR=$(dirname $(readlink -f "$0"))

cd $SCRIPT_DIR/../

make clean && make debug

cd $SCRIPT_DIR

# tmux splitw -h -p 50 "gdbserver :12345 $SCRIPT_DIR/../bin/main"
# tmux selectp -t 0
# gdb -x debug.gdb


# tmux new-session -d -s SplitDbg
# tmux selectp -t SplitDbg
# tmux split-window -v
# tmux send-keys -t SplitDbg.1 "gdbserver :12345 $SCRIPT_DIR/../bin/main" ENTER
# tmux attach -t SplitDbg
# gdb -x debug.gdb

tmux new-session -d -s SplitDbg
tmux selectp -t SplitDbg
tmux splitw -h -p 50 "gdbserver :12345 $SCRIPT_DIR/../bin/main"
tmux send-keys -t 0 "gdb $SCRIPT_DIR/../bin/main -x $SCRIPT_DIR/debug.gdb" Enter
tmux send-keys -t 0 Enter C-x o
tmux select-pane -t 0
tmux attach -t SplitDbg
# gdb -x debug.gdb
