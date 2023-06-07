# Roguelite

# Usage
1. `make run`
2. Pray.

## Debugger
The VSCode debugger is completely and utterly fucked. To debug, manually use gdb.

1. Run the command `make debug`
2. Run the command `gdb bin/main`
3. Run the gdb command `layout src`
4. Run the gdb command `break main`
5. Run the gdb command `run`
6. Debug the program

## Debugger script
In case gdb decides to fuck up as well:

### Installation
1. `sudo apt install tmux`
2. `sudo apt install gdbserver`

### Usage
1. Open a new terminal window.
2. Navigate to the project root.
3. Run the command `./scripts/debug.sh`
4. Press ENTER if required.
5. Write `c` and press ENTER.
6. Press CTRL + B and then the right arrow to switch to the app.
7. Press CTRL + B and then the left arrow to switch to the debugger.

## Debugger Tests

### Memory Leaks
1. Start valgrind: `valgrind --vgdb=yes --vgdb-error=0 bin/main`
2. Copy the command valgrind gives (looks like `target remote | /usr/bin/vgdb --pid=159205`)
3. Open gdb: `gdb bin/main`. Skip the message.
4. Paste the command into gdb.
5. Run the command in gdb: `monitor leak_check`.
6. It should output `All heap blocks were freed -- no leaks are possible`
7. Run the command in gdb: `monitor v.kill`
8. Exit gdb.

# Notice
If you feel offended by any of these comments, feel free to fuck off elsewhere, preferably Twitter.

*For legal reasons, all offensive statements are a joke and should not be treated outside of the context of such.*
