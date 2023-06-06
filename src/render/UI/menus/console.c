#include "console.h"

extern int EXIT;

static int SETUP = 0;

#define MAX_CONSOLE_INPUT 255
static char console_input[255];
static int console_input_ind = 0;
static int console_input_cursor = 0;
static int console_input_skip_tick = FALSE;

static char** console_output;
static int console_output_ind = 0;
static int console_output_control[4] = { 0, 0, 0, 0 }; // max lines, max line length, latest line, scroll offset

#define MAX_HISTORY_COMMANDS 11
char* history[MAX_HISTORY_COMMANDS];
int history_len = 0;

#define MAX_BLINK_TICKS 10
static int blink_ticks = 0;
static int blink = 0;

static int console_win[2] = { 0, 0 }; // mrows, mcols

static FILE* consoleOut;

void setupConsole();
void addMessage(const char* msg);
void addCommandToHistory(const char* command);
void removeCommandFromHistory(int index);
void _processCommand();
void _makeProcessorPatterns();

#pragma region Menu functions
void draw_ConsoleMenu(Menu menu) {
    int mrows = g_renderstate->nrows - 7;
    int mcols = g_renderstate->ncols - 7;

    console_win[0] = mrows;
    console_win[1] = mcols;

    wbkgd(menu->wnd, COLOR_PAIR(MATRIX_BG));

    mvwaddch(menu->wnd, 0, 0, ACS_ULCORNER);
    mvwaddch(menu->wnd, 0, mcols, ACS_URCORNER);
    mvwaddch(menu->wnd, mrows, 0, ACS_LLCORNER);
    mvwaddch(menu->wnd, mrows, mcols, ACS_LRCORNER);

    for (int row = 1; row < mrows; row++) {
        mvwaddch(menu->wnd, row, 0, ACS_VLINE);
        mvwaddch(menu->wnd, row, mcols, ACS_VLINE);
    }
    for (int col = 1; col < mcols; col++) {
        mvwaddch(menu->wnd, 0, col, ACS_HLINE);
        mvwaddch(menu->wnd, mrows - 2, col, ACS_HLINE);
        mvwaddch(menu->wnd, mrows, col, ACS_HLINE);
    }

    // Print messages
    int curLine = mrows - 3;
    for (int i = console_output_control[3]; i < mrows - 1 + console_output_control[3]; i++) {
        // mvwprintw(menu->wnd, curLine--, 2, console_output[i]);
        mvwaddstr(menu->wnd, curLine--, 2, console_output[i]);
    }

    // mvwprintw(menu->wnd, mrows - 3, mcols - 20, "%d | %d %d", curLine, mrows, mcols);
    // mvwprintw(menu->wnd, mrows - 4, mcols - 20, "%d %d %d %d", console_output_control[0], console_output_control[1], console_output_control[2], console_output_control[3]);
    // mvwprintw(menu->wnd, mrows - 5, mcols - 20, "%d", console_output_control[0] - mrows - 5 - 1);

    mvwprintw(menu->wnd, mrows - 1, 2, "> %s", console_input);
    if (blink) {
        wmove(menu->wnd, mrows - 1, 4 + console_input_cursor);
        waddch(menu->wnd, '_');
    }

    // wmove(menu->wnd, mrows - 1, 4 + console_input_cursor);
    // wattron(menu->wnd, A_BLINK);
    // waddch(menu->wnd, '_');
    // wattroff(menu->wnd, A_BLINK);

    //refresh();
}

void tick_ConsoleMenu() {
    if (!SETUP) {
        debug_file(dbgOut, 2, "Setting up console.\n");

        setupConsole();
        _makeProcessorPatterns();

        debug_file(dbgOut, 2, "Console setup.\n");
        SETUP = TRUE;
    }

    blink_ticks++;
    if (blink_ticks >= MAX_BLINK_TICKS) {
        blink = !blink;
        blink_ticks = 0;
    }
}

void handle_ConsoleMenu_keybinds(int key) {
    switch(key) {
        case KEY_UP:
            if (
                console_output_ind > console_win[0] - 5 
                && console_output_control[3] < console_output_ind - 1
            ) console_output_control[3]++;
            break;
        case KEY_DOWN:
            if (
                console_output_ind > console_win[0] - 5 
                && console_output_control[3] > 0
            ) console_output_control[3]--;
            break;
        case KEY_LEFT:
            if (console_input_cursor > 0) {
                console_input_cursor--;
            }
            break;
        case KEY_RIGHT:
            if (console_input_cursor < console_input_ind) {
                console_input_cursor++;
            }
            break;
        case 10: case 13: {
            char inp[MAX_CONSOLE_INPUT + 3] = { 0 };
            strcpy(inp, "> \0");
            strcat(inp, console_input);
            addMessage(inp);

            if (!equal_strings(inp, history[0])) addCommandToHistory(console_input);
            
            _processCommand();

            if (console_input_skip_tick) {
                console_input_skip_tick = FALSE;
            } else {
                memset(console_input, 0, MAX_CONSOLE_INPUT * sizeof(char));
                console_input_ind = 0;
                console_input_cursor = 0;
            }
            break;
        }
        case KEY_BACKSPACE:
        case 127:
            // if (console_input_ind != 0) {
            //     console_input[--console_input_ind] = '\0';
            // }
            if (console_input_cursor > 0) {
                for (int i = console_input_cursor; i <= console_input_ind; i++) {
                    console_input[i - 1] = console_input[i];
                }
                console_input_cursor--;
                console_input_ind--;
            }
            break;
        default:
            debug_file(dbgOut, 2, "Recieved Console Keypress: '%c' (%d) | %d\n", key, key, console_input_ind);

            if (isprint(key) && console_input_ind < MAX_CONSOLE_INPUT) {
                for (int i = console_input_ind; i >= console_input_cursor; i--) {
                    console_input[i + 1] = console_input[i];
                }
                // Insert the new character
                console_input[console_input_cursor] = key;
                console_input_cursor++;
                console_input_ind++;
            }

            // console_input[console_input_ind++] = key;
            break;
    }
}

void cleanup_ConsoleMenu() {
    memset(console_input, 0, MAX_CONSOLE_INPUT * sizeof(char));
    console_input_ind = 0;
    console_output_control[3] = 0;
}
#pragma endregion

#pragma region CONSOLE HELPERS
void logMessage(const char* format, ...) {
    if (consoleOut == NULL) return;

    va_list valist;

    va_start(valist, format);
    vfprintf(consoleOut, format, valist);
    va_end(valist);

    fflush(consoleOut);
}

void addMessage(const char* msg) {
    for (int i = console_output_control[0] - 1; i > 0; i--) {
        strcpy(console_output[i], console_output[i - 1]);
    }

    strncpy(console_output[0], msg, console_output_control[1] - 1);
    console_output[0][console_output_control[1]] = '\0';
    console_output_ind++;

    char* logMsg = (char*)malloc(console_output_control[1] * sizeof(char));
    strncpy(logMsg, msg, console_output_control[1] - 1);
    strcat(logMsg, "\n");

    logMessage(logMsg);
}

// void addCommandToHistory(const char* command) {
//     if (strlen(command) == 0) return;

//     for (int i = MAX_HISTORY_COMMANDS - 1; i > 0; i--) {
//         strcpy(history[i], history[i - 1]);
//     }

//     strcpy(history[0], command);

//     if (history_len <= MAX_HISTORY_COMMANDS) history_len++;
// }

void addCommandToHistory(const char* command) {
    if (strlen(command) == 0) return;

    if (history_len >= MAX_HISTORY_COMMANDS) free(history[history_len]);
    else history_len++;
    
    for (int i = MAX_HISTORY_COMMANDS - 1; i > 0; i--) {
        if (history[i] == NULL) break;
        strcpy(history[i], history[i - 1]);
    }

    strcpy(history[0], command);
}


void removeCommandFromHistory(int index) {
    if (index < 0 || index >= history_len) return;

    // free(history[index]);

    for (int i = index; i < history_len - 1; i++) {
        // history[i] = history[i + 1];
        strcpy(history[i], history[i + 1]);
    }

    memset(history[history_len - 1], 0, MAX_CONSOLE_INPUT * sizeof(char));

    // history[history_len - 1] = "";
    // history[history_len - 1] = malloc(MAX_CONSOLE_INPUT * sizeof(char));
    // strcpy(history[history_len - 1], "");

    history_len--;
}


void createSessionFile() {
    char out_path[PATH_MAX];
    snprintf(out_path, sizeof(out_path), "%.*s/logs/console-", BIN_PATH_LEN, BIN_PATH);

	time_t now = time(NULL);
    struct tm* tmInfo = localtime(&now);
    size_t dateFormatSize = 20;
    // size_t new_out_len = BIN_PATH_LEN + sizeof("/logs/console-") - 1 + dateFormatSize;
    size_t new_out_len = BIN_PATH_LEN + strlen("/logs/console-") - 1 + dateFormatSize;

    char* new_out_path = malloc(new_out_len * sizeof(char));
    if (new_out_path == NULL) {
        debug_file(dbgOut, 1, "Unable to initialize console: Failed to generate log path.\n");
        closeMenu(MENU_CONSOLE);
        return;
    }
    strcpy(new_out_path, out_path);
    strftime(new_out_path + new_out_len - dateFormatSize, dateFormatSize, "%Y-%m-%dT%H:%M:%S", tmInfo);
    strcat(new_out_path, ".log");

	if (createParentFolder(new_out_path) != 0) {
        debug_file(dbgOut, 1, "Unable to initialize console: Unable to create log folder.\n");
        closeMenu(MENU_CONSOLE);
        return;
    }

    consoleOut = fopen(new_out_path, "a");
    if (consoleOut == NULL) {
        debug_file(dbgOut, 1, "Unable to initialize console: Unable to open log file.\n");
        closeMenu(MENU_CONSOLE);
        return;
    }
}

void setupCommandHistory() {
    for (int i = 0; i < MAX_HISTORY_COMMANDS; i++) {
        history[i] = malloc(MAX_CONSOLE_INPUT * sizeof(char));
        strcpy(history[i], "");
    }
}

void setupConsole() {
    console_output_control[0] = 2 * g_renderstate->nrows;
    console_output_control[1] = g_renderstate->ncols - 8;

    console_output = (char**)malloc(console_output_control[0] * sizeof(char*));
    for (int i = 0; i < console_output_control[0]; i++) {
        console_output[i] = (char*)malloc(console_output_control[1] * sizeof(char));
        console_output[i][0] = '\0';
    }

    setupCommandHistory();

    createSessionFile();
}

void flushOutput() {
    for (int i = 0; i < console_output_control[0]; i++) {
        console_output[i][0] = '\0';
    }
    console_output_ind = 0;
}

void nukeConsoleMenu() {
    for (int i = 0; i < console_output_control[0]; i++) {
        free(console_output[i]);
    }
    free(console_output);

    cleanup_ConsoleMenu();

    fclose(consoleOut);

    setupConsole();
}

#pragma endregion

#pragma region COMMAND PROCESSING

// Patterns
struct pattern {
    char* regex;
    char* cmdName;
    int cmd;
};

#define PATTERNS 19
struct pattern shadow_patterns[PATTERNS];
struct pattern patterns[PATTERNS];

struct pattern _makeProcessorPattern(char* regex, char* name, int cmd) {
    struct pattern node = {
        regex,
        name,
        cmd
    };

    return node;
}

void _makeProcessorPatterns() {
    patterns[0] = _makeProcessorPattern("^exit ?[a-zA-Z]*$", "exit", 1);
    shadow_patterns[0] = _makeProcessorPattern("^exit", "exit", 1);

    patterns[1] = _makeProcessorPattern("^help ?[a-zA-Z]*$", "help", 2);
    shadow_patterns[1] = _makeProcessorPattern("^help", "help", 2);

    patterns[2] = _makeProcessorPattern("^godmode ?[01]*$", "godmode", 3);
    shadow_patterns[2] = _makeProcessorPattern("^godmode", "godmode", 3);

    patterns[3] = _makeProcessorPattern("^clear$", "clear", 4);
    shadow_patterns[3] = _makeProcessorPattern("^clear", "clear", 4);

    patterns[4] = _makeProcessorPattern("^healPlayer [0-9]+$", "healPlayer", 5);
    shadow_patterns[4] = _makeProcessorPattern("^healPlayer", "healPlayer", 5);

    patterns[5] = _makeProcessorPattern("^damagePlayer [0-9]+$", "damagePlayer", 6);
    shadow_patterns[5] = _makeProcessorPattern("^damagePlayer", "damagePlayer", 6);

    patterns[6] = _makeProcessorPattern("^q$", "q", 7);
    shadow_patterns[6] = _makeProcessorPattern("^q", "q", 7);

    patterns[7] = _makeProcessorPattern("^reset$", "reset", 8);
    shadow_patterns[7] = _makeProcessorPattern("^reset", "reset", 8);

    patterns[8] = _makeProcessorPattern("^getitembyname [a-zA-Z_]+$", "getitembyname", 9);
    shadow_patterns[8] = _makeProcessorPattern("^getitembyname", "getitembyname", 9);

    patterns[9] = _makeProcessorPattern("^getitembyid [0-9]+$", "getitembyid", 10);
    shadow_patterns[9] = _makeProcessorPattern("^getitembyid", "getitembyid", 10);

    patterns[10] = _makeProcessorPattern("^gettranslation [a-zA-Z_]+ [a-zA-Z0-9_\\.]+$", "gettranslation", 11);
    shadow_patterns[10] = _makeProcessorPattern("^gettranslation", "gettranslation", 11);

    patterns[11] = _makeProcessorPattern("^getwindows$", "getwindows", 12);
    shadow_patterns[11] = _makeProcessorPattern("^getwindows", "getwindows", 12);

    patterns[12] = _makeProcessorPattern("^maxfov ?[01]*$", "maxfov", 13);
    shadow_patterns[12] = _makeProcessorPattern("^maxfov", "maxfov", 13);

    patterns[13] = _makeProcessorPattern("^kill$", "kill", 14);
    shadow_patterns[13] = _makeProcessorPattern("^kill", "kill", 14);

    patterns[14] = _makeProcessorPattern("^getplayerprop ?[a-zA-Z0-9_\\.]*$", "getplayerprop", 15);
    shadow_patterns[14] = _makeProcessorPattern("^getplayerprop", "getplayerprop", 15);

    patterns[15] = _makeProcessorPattern("^history ?[0-9]?$", "history", 16);
    shadow_patterns[15] = _makeProcessorPattern("^history", "history", 16);

    patterns[16] = _makeProcessorPattern("^addclock [0-9]+$", "addclock", 17);
    shadow_patterns[16] = _makeProcessorPattern("^addclock", "addclock", 17);

    patterns[17] = _makeProcessorPattern("^removeclock 0x[0-9a-zA-Z]+$", "removeclock", 18);
    shadow_patterns[17] = _makeProcessorPattern("^removeclock", "removeclock", 18);

    patterns[18] = _makeProcessorPattern("^viewclock 0x[0-9a-zA-Z]+$", "viewclock", 19);
    shadow_patterns[18] = _makeProcessorPattern("^viewclock", "viewclock", 19);

}

void _executeCommand(int cmd, void* override) {
    int len;
    re_t regex;

    if (cmd == 1) {
        regex = re_compile("^exit$");

        debug_file(dbgOut, 1, "- Parsing 'class' argument\n");
        if (re_matchp(regex, console_input, &len) != -1) {
            debug_file(dbgOut, 1, "-- No arguments\n");

            closeMenu(MENU_CONSOLE);
            debug_file(dbgOut, 0, "Console closed.\n");
            return;
        } else {
            debug_file(dbgOut, 1, "-- With arguments\n");

            char param[50];
            if (!sscanf(console_input, "exit %s", param)) {
                closeMenu(MENU_CONSOLE);
                debug_file(dbgOut, 0, "Console closed.\n");
                return;
            }

            debug_file(dbgOut, 1, "-- Arguments: %d\n", param);

            if (strcmp(param, "all") == 0) {
                EXIT = TRUE;
                return;
            } else if (strcmp(param, "console") == 0) {
                closeMenu(MENU_CONSOLE);
                debug_file(dbgOut, 0, "Console closed.\n");
                return;
            } else {
                addMessage("Usage: exit [class]");
                addMessage("- class: all, console");
            }
        }
    } else if (cmd == 2) {
        regex = re_compile("^help$");

        debug_file(dbgOut, 1, "- Parsing 'command' argument\n");
        if (re_matchp(regex, console_input, &len) != -1) {
            debug_file(dbgOut, 1, "-- No arguments\n");

            addMessage("DLM Console, version 2.5.1");
            addMessage("These commands are defined internally. Type 'help' to see this list.");
            addMessage("Type 'help' [command] to see more info about a command.");
            addMessage("Arguments in the form [arg] are required and arguments in the form <arg> are optional.");
            addMessage("");
            addMessage("addclock       | Adds a new clock and returns it's address.");
            addMessage("clear          | Clears the console output.");
            addMessage("damagePlayer   | Damages the player.");
            addMessage("exit           | Closes the console terminal.");
            addMessage("getitembyid    | Fetches an item through it's ID.");
            addMessage("getitembyname  | Fetches an item through it's name.");
            addMessage("getplayerprop  | Fetches a property stored on the player.");
            addMessage("gettranslation | Fetches a translation through it's translation key.");
            addMessage("getwindows     | Fetches the number of windows created.");
            addMessage("godmode        | (De)activates the GodMode.");
            addMessage("healPlayer     | Heals the player.");
            addMessage("history        | Views or restores a previous command.");  
            addMessage("help           | Displays this message.");  
            addMessage("kill           | Kills the player.");
            addMessage("removeclock    | Removes a clock by it's address.");
            addMessage("reset          | [CHEAT] Resets the console.");
            addMessage("maxfov         | [CHEAT] (De)activates the Max FOV.");
            addMessage("viewclock      | Fetches the data of a clock by it's address.");  
            return;
        } else {
            debug_file(dbgOut, 1, "-- With arguments\n");

            char command[50];

            if (override != NULL) {
                debug_file(dbgOut, 1, "-- Arguments overriden\n");

                strncpy(command, (char*)override, 50);
            } else {
                if (!sscanf(console_input, "help %s", command)) {
                    debug_file(dbgOut, 1, "-- Failed to parse argument\n");
                    addMessage("Invalid command.");
                    return;
                } 
            }

            debug_file(dbgOut, 1, "-- Arguments: %d\n", command);

            if (strcmp(command, "clear") == 0) {
                addMessage("clear: clear");
                addMessage("    clears the console output.");
                return;
            } else if (strcmp(command, "damagePlayer") == 0) {
                addMessage("damagePlayer: damagePlayer [amount]");
                addMessage("    damages the player by AMOUNT.");
                addMessage("");
                addMessage("Arguments:");
                addMessage("    AMOUNT: an integer");
                return;
            } else if (strcmp(command, "exit") == 0) {
                addMessage("exit: exit <class>");
                addMessage("    exits either the game or the console.");
                addMessage("");
                addMessage("Arguments:");
                addMessage("    CLASS: all, console");
                addMessage("");
                addMessage("Aliases:");
                addMessage("    exit console: q");
                return;
            } else if (strcmp(command, "godmode") == 0) {
                addMessage("godmode: godmode <flag>");
                addMessage("    toggles the godmode.");
                addMessage("");
                addMessage("Arguments:");
                addMessage("    FLAG: 1, 0");
                return;
            } else if (strcmp(command, "healPlayer") == 0) {
                addMessage("healPlayer: healPlayer [amount]");
                addMessage("    heals the player by AMOUNT.");
                addMessage("");
                addMessage("Arguments:");
                addMessage("    AMOUNT: an integer");
                return;
            } else if (strcmp(command, "help") == 0) {
                addMessage("help: help <command>");
                addMessage("    displays information about a command or all commands.");
                addMessage("");
                addMessage("Arguments:");
                addMessage("    COMMAND: exit, godmode, help");
                return;
            } else if (strcmp(command, "q") == 0) {
                // _executeCommand(2, "exit", 4);
                _executeCommand(2, "exit");
                return;
            } else if (strcmp(command, "reset") == 0) {
                addMessage("reset: reset");
                addMessage("    resets the console.");
                return;
            } else if (strcmp(command, "getitembyid") == 0) {
                addMessage("getitembyid: getitembyid [ID]");
                addMessage("    fetches the information about an item through it's ID.");
                addMessage("");
                addMessage("Arguments:");
                addMessage("    ID: a numeric sequence representing an Item Id.");
                return;
            } else if (strcmp(command, "getitembyname") == 0) {
                addMessage("getitembyname: getitembyname [NAME]");
                addMessage("    fetches the information about an item through it's NAME.");
                addMessage("");
                addMessage("Arguments:");
                addMessage("    NAME: an alphanumeric sequence representing an item name.");
                return;
            } else if (strcmp(command, "getplayerprop") == 0) {
                addMessage("getitembyid: getplayerprop [PROP]");
                addMessage("    fetches the value of a PROPerty stored on the player.");
                addMessage("");
                addMessage("Arguments:");
                addMessage("    PROP: an alphanumeric sequence representing an existing property on the player.");
                return;
            } else if (strcmp(command, "gettranslation") == 0) {
                addMessage("gettranslation: gettranslation [KEY]");
                addMessage("    fetches the translation of a given translation KEY.");
                addMessage("");
                addMessage("Arguments:");
                addMessage("    KEY: an alphanumeric sequence representing a translation key.");
                return;
            } else if (strcmp(command, "getwindows") == 0) {
                addMessage("getwindows: getwindows");
                addMessage("    fetches the number of windows created up to the invocation time.");
                return;
            } else if (strcmp(command, "maxfov") == 0) {
                addMessage("maxfov: maxfov <flag>");
                addMessage("    toggles the max FOV.");
                addMessage("");
                addMessage("Arguments:");
                addMessage("    FLAG: 1, 0");
                return;
            } else if (strcmp(command, "kill") == 0) {
                addMessage("kill: kill");
                addMessage("    kills the player.");
                return;
            } else if (strcmp(command, "history") == 0) {
                addMessage("history: history <CMD>");
                addMessage("    views the command history or restores a command if the argument CMD is present.");
                addMessage("Arguments:");
                addMessage("    CMD: A number between 0 to 9 (inclusive).");
                return;
            } else if (strcmp(command, "addclock") == 0) {
                addMessage("addclock: addclock [MAX]");
                addMessage("    adds a clock with maxTicks set to MAX, and returns it's address.");
                addMessage("Arguments:");
                addMessage("    MAX: an integer.");
                return;
            } else if (strcmp(command, "removeclock") == 0) {
                addMessage("removeclock: removeclock [ADDR]");
                addMessage("    removes a clock through it's address.");
                addMessage("Arguments:");
                addMessage("    ADDR: a clock address.");
                return;
            } else if (strcmp(command, "viewclock") == 0) {
                addMessage("viewclock: viewclock [ADDR]");
                addMessage("    fetches the data of a clock through it's address.");
                addMessage("Arguments:");
                addMessage("    ADDR: a clock address.");
                return;
            } else {
                char out[MAX_CONSOLE_INPUT];
                sprintf(out, "%s: no help for command found", command);

                addMessage(out);
            }
        }
    } else if (cmd == 3) {
        regex = re_compile("^godmode$");

        debug_file(dbgOut, 1, "- Parsing 'toggle' argument\n");
        if (re_matchp(regex, console_input, &len) != -1) {
            debug_file(dbgOut, 1, "-- No arguments\n");
            g_gamestate->player->cheats->godmode = (g_gamestate->player->cheats->godmode == 0) ? 1 : 0;
            godmode_Health();
            addMessage("GodMode toggled.");
                    
            return;
        } else {
            debug_file(dbgOut, 1, "-- With arguments\n");

            char flag[1];
            if (!sscanf(console_input, "godmode %s", flag)) {
                debug_file(dbgOut, 1, "-- Failed to parse argument\n");
                addMessage("Invalid flag.");
                return;
            }

            debug_file(dbgOut, 1, "-- Arguments: %d\n", flag);

            if (strcmp(flag, "1") == 0) {
                g_gamestate->player->cheats->godmode = 1;
                godmode_Health();
                addMessage("GodMode activated.");
                return;
            } else if (strcmp(flag, "0") == 0) {
                g_gamestate->player->cheats->godmode = 0;
                godmode_Health();
                addMessage("GodMode deactivated.");
                return;
            } else {
                addMessage("Invalid syntax. Type 'help godmode' for more info.");
            }
        }
    } else if (cmd == 4) {
        debug_file(dbgOut, 1, "- Clearing terminal\n");
        flushOutput();
        addMessage("Console cleared.");
    } else if (cmd == 5) {
        int param;
        if (!sscanf(console_input, "healPlayer %d", &param)) {
            addMessage("Invalid value.");
            return;
        }

        debug_file(dbgOut, 1, "-- Arguments: %d\n", param);

        int prevHp = g_gamestate->player->entity->health;
        healEntity(g_gamestate->player->entity, param);

        char out[MAX_CONSOLE_INPUT];
        snprintf(
            out, MAX_CONSOLE_INPUT, 
            "Healed player with %d HP (%llu effective).", 
            param, g_gamestate->player->entity->health - prevHp
        );

        addMessage(out);
    } else if (cmd == 6) {
        int param;
        if (!sscanf(console_input, "damagePlayer %d", &param)) {
            addMessage("Invalid value.");
            return;
        }

        debug_file(dbgOut, 1, "-- Arguments: %d\n", param);

        int prevHp = g_gamestate->player->entity->health;
        damageEntity(g_gamestate->player->entity, param);

        char out[MAX_CONSOLE_INPUT];
        snprintf(
            out, MAX_CONSOLE_INPUT, 
            "Damaged player with %d HP (%llu effective).", 
            param, prevHp - g_gamestate->player->entity->health
        );

        addMessage(out);
    } else if (cmd == 7) {
        // _executeCommand(1, NULL, 0);
        _executeCommand(1, NULL);
    } else if (cmd == 8) {
        nukeConsoleMenu();
        addMessage("Terminal nuked");
    } else if (cmd == 9) {
        char param[50];
        if (!sscanf(console_input, "getitembyname %s", param)) {
            addMessage("Invalid value.");
            return;
        }

        debug_file(dbgOut, 1, "-- Arguments: %d\n", param);

        char out[MAX_CONSOLE_INPUT];
        DataItemNode item = get_item_by_name(param);
        if (item == NULL) {
            snprintf(
                out, MAX_CONSOLE_INPUT, 
                "<undefined>"
            );
        } else {
            snprintf(
                out, MAX_CONSOLE_INPUT, 
                "%s | Name: '%s' | Damage: %d | Armor: %d ",
                item->id, item->name, item->damage, item->armor
            );
        }

        addMessage(out);
    } else if (cmd == 10) {
        char param[50];
        if (!sscanf(console_input, "getitembyid %s", param)) {
            addMessage("Invalid value.");
            return;
        }

        debug_file(dbgOut, 1, "-- Arguments: %d\n", param);

        char out[MAX_CONSOLE_INPUT];
        DataItemNode item = get_item_by_id(param);
        if (item == NULL) {
            snprintf(
                out, MAX_CONSOLE_INPUT, 
                "<undefined>"
            );
        } else {
            snprintf(
                out, MAX_CONSOLE_INPUT, 
                "%s | Name: '%s' | Damage: %d | Armor: %d ",
                item->id, item->name, item->damage, item->armor
            );
        }

        addMessage(out);
    } else if (cmd == 11) {
        int param_len = MAX_CONSOLE_INPUT - 5;
        char param[param_len];
        char locale[6];
        if (!sscanf(console_input, "gettranslation %5s %s", locale, param)) {
            addMessage("Invalid value.");
            return;
        }

        debug_file(dbgOut, 1, "-- Arguments: %d\n", param);

        DataLocale loc = get_locale_id(locale);

        char out[MAX_CONSOLE_INPUT];
        char* transl = get_localized_string(loc, param);

        if (transl == NULL || strcmp(transl, param) == 0) {
            snprintf(
                out, MAX_CONSOLE_INPUT, 
                "<untranslated>"
            );

            addMessage(out);
        } else {
            int lineCount = 0, longest = 0;
            get_multiline_stats(transl, &lineCount, &longest);

            char* lines[lineCount];
            for (int i = 0; i < lineCount; i++) {
                lines[i] = (char*)malloc(longest * sizeof(char));
            }

            split_by(transl, "\n", lines);

            snprintf(
                out, MAX_CONSOLE_INPUT, 
                "%s: '''",
                param
            );

            addMessage(out);
            for (int i = 0; i < lineCount; i++) {
                addMessage(lines[i]);
            }
            addMessage("'''");
        }
    } else if (cmd == 12) {
        char out[MAX_CONSOLE_INPUT];

        snprintf(
            out, MAX_CONSOLE_INPUT, 
            "Currently created window count: %d",
            menu_wins
        );

        addMessage(out);
    } else if (cmd == 13) {
        regex = re_compile("^maxfov$");

        debug_file(dbgOut, 1, "- Parsing 'toggle' argument\n");
        if (re_matchp(regex, console_input, &len) != -1) {
            debug_file(dbgOut, 1, "-- No arguments\n");
            g_gamestate->player->cheats->vision = (g_gamestate->player->cheats->vision == 0) ? 1 : 0;
            vision_Radius();
            addMessage("Max FOV toggled.");
                    
            return;
        } else {
            debug_file(dbgOut, 1, "-- With arguments\n");

            char flag[1];
            if (!sscanf(console_input, "maxfov %s", flag)) {
                debug_file(dbgOut, 1, "-- Failed to parse argument\n");
                addMessage("Invalid flag.");
                return;
            }

            debug_file(dbgOut, 1, "-- Arguments: %d\n", flag);

            if (strcmp(flag, "1") == 0) {
                g_gamestate->player->cheats->vision = 1;
                vision_Radius();
                addMessage("Max FOV activated.");
                return;
            } else if (strcmp(flag, "0") == 0) {
                g_gamestate->player->cheats->vision = 0;
                vision_Radius();
                addMessage("Max FOV deactivated.");
                return;
            } else {
                addMessage("Invalid syntax. Type 'help maxfov' for more info.");
            }
        }
    } else if (cmd == 14) {
        g_gamestate->player->entity->armor = 0;
        g_gamestate->player->entity->health = 0;
        
        addMessage("Killed player");
    } else if (cmd == 15) {
        char out[MAX_CONSOLE_INPUT] = { 0 };
        Player player = g_gamestate->player;

        regex = re_compile("^getplayerprop$");

        debug_file(dbgOut, 1, "- Parsing 'prop' argument\n");
        if (re_matchp(regex, console_input, &len) != -1) {
            debug_file(dbgOut, 1, "-- No arguments\n");

            snprintf(
                out, MAX_CONSOLE_INPUT, 
                "Player { entity=<Entity>; item=<Item>; cheats=<Cheats>; level=%d; kills=%d; xp=%d; last_direction=%d; "
                    "class=%s; radius=%d; sanity=%d; candle_light=%d; hasUltimate=%d }",
                player->level, player->kills, player->xp, player->last_direction, 
                    stringify_class(player->class), player->radius, player->sanity, player->candle_fuel, player->hasUltimate
            );

            addMessage(out);       
            return;
        }

        debug_file(dbgOut, 1, "-- With arguments\n");

        int param_len = 100;
        char param[param_len];
        if (!sscanf(console_input, "getplayerprop %s", param)) {
            addMessage("Invalid value.");
            return;
        }

        #pragma region Entity
        if (equal_strings(param, "entity")) {
            snprintf(
                out, MAX_CONSOLE_INPUT, 
                "Entity { coords=<Coords>; maxHealth=%u; health=%llu; armor=%d; basedamage=%d }",
                player->entity->maxHealth, player->entity->health, player->entity->armor, player->entity->basedamage
            );
        } else if (equal_strings(param, "entity.coords")) {
            snprintf(
                out, MAX_CONSOLE_INPUT, 
                "Coords { x=%d; y=%d }",
                player->entity->coords->x, player->entity->coords->y
            );
        } else if (equal_strings(param, "entity.coords.x")) {
            snprintf(
                out, MAX_CONSOLE_INPUT, 
                "%d",
                player->entity->coords->x
            );
        } else if (equal_strings(param, "entity.coords.y")) {
            snprintf(
                out, MAX_CONSOLE_INPUT, 
                "%d",
                player->entity->coords->y
            );
        } else if (equal_strings(param, "entity.maxHealth")) {
            snprintf(
                out, MAX_CONSOLE_INPUT, 
                "%u",
                player->entity->maxHealth
            );
        } else if (equal_strings(param, "entity.health")) {
            snprintf(
                out, MAX_CONSOLE_INPUT, 
                "%llu",
                player->entity->health
            );
        } else if (equal_strings(param, "entity.armor")) {
            snprintf(
                out, MAX_CONSOLE_INPUT, 
                "%d",
                player->entity->armor
            );
        } else if (equal_strings(param, "entity.basedamage")) {
            snprintf(
                out, MAX_CONSOLE_INPUT, 
                "%d",
                player->entity->basedamage
            );
        }
        #pragma endregion

        #pragma region Item
        else if (equal_strings(param, "item")) {
            snprintf(
                out, MAX_CONSOLE_INPUT, 
                "getitembyid %s",
                player->item->id
            );
            _executeCommand(10, out);
            return;
        } else if (equal_strings(param, "item.id")) {
            snprintf(
                out, MAX_CONSOLE_INPUT, 
                "%s",
                player->item->id
            );
        } else if (equal_strings(param, "item.name")) {
            snprintf(
                out, MAX_CONSOLE_INPUT, 
                "%s",
                player->item->name
            );
        } else if (equal_strings(param, "item.damage")) {
            snprintf(
                out, MAX_CONSOLE_INPUT, 
                "%d",
                player->item->damage
            );
        } else if (equal_strings(param, "item.armor")) {
            snprintf(
                out, MAX_CONSOLE_INPUT, 
                "%d",
                player->item->armor
            );
        }
        #pragma endregion

        #pragma region Cheats
        else if (equal_strings(param, "cheats")) {
            snprintf(
                out, MAX_CONSOLE_INPUT, 
                "Cheats { godmode=%d; vision=%d }",
                player->cheats->godmode, player->cheats->vision
            );
        } else if (equal_strings(param, "cheats.godmode")) {
            snprintf(
                out, MAX_CONSOLE_INPUT, 
                "%d",
                player->cheats->godmode
            );
        } else if (equal_strings(param, "cheats.vision")) {
            snprintf(
                out, MAX_CONSOLE_INPUT, 
                "%d",
                player->cheats->vision
            );
        }
        #pragma endregion

        else if (equal_strings(param, "level")) {
            snprintf(
                out, MAX_CONSOLE_INPUT, 
                "%d",
                player->level
            );
        } else if (equal_strings(param, "kills")) {
            snprintf(
                out, MAX_CONSOLE_INPUT, 
                "%d",
                player->kills
            );
        } else if (equal_strings(param, "xp")) {
            snprintf(
                out, MAX_CONSOLE_INPUT, 
                "%d",
                player->xp
            );
        } else if (equal_strings(param, "last_direction")) {
            snprintf(
                out, MAX_CONSOLE_INPUT, 
                "%d",
                player->last_direction
            );
        } else if (equal_strings(param, "class")) {
            snprintf(
                out, MAX_CONSOLE_INPUT, 
                "%s",
                stringify_class(player->class)
            ); 
        } else if (equal_strings(param, "radius")) {
            snprintf(
                out, MAX_CONSOLE_INPUT, 
                "%d",
                player->radius
            );
        } else if (equal_strings(param, "sanity")) {
            snprintf(
                out, MAX_CONSOLE_INPUT, 
                "%d",
                player->sanity
            );
        } else if (equal_strings(param, "candle_fuel")) {
            snprintf(
                out, MAX_CONSOLE_INPUT, 
                "%d",
                player->candle_fuel
            );
        } else if (equal_strings(param, "hasUltimate")) {
            snprintf(
                out, MAX_CONSOLE_INPUT, 
                "%d",
                player->hasUltimate
            );
        }

        addMessage(out);
    } else if (cmd == 16) {
        regex = re_compile("^history$");

        removeCommandFromHistory(0);

        debug_file(dbgOut, 1, "- Parsing 'cmd' argument\n");
        if (re_matchp(regex, console_input, &len) != -1) {
            debug_file(dbgOut, 1, "-- No arguments\n");

            char buffer[MAX_CONSOLE_INPUT] = { 0 };
            addMessage("Command history:");
            for (int i = 0; i < MAX_HISTORY_COMMANDS - 1; i++) {
                snprintf(buffer, MAX_CONSOLE_INPUT, "%d: %s", i, history[i]);
                addMessage(buffer);
            }
                    
            return;
        } else {
            debug_file(dbgOut, 1, "-- With arguments\n");

            int ind = -1;
            if (!sscanf(console_input, "history %d", &ind)) {
                debug_file(dbgOut, 1, "-- Failed to parse argument\n");
                addMessage("Invalid index.");
                return;
            }

            if (ind < 0 || ind > 9) {
                addMessage("Invalid index.");
                return; 
            }

            strcpy(console_input, history[ind]);
            console_input_cursor = strlen(history[ind]);
            console_input_ind = strlen(history[ind]);
            console_input_skip_tick = TRUE;

            addMessage("Restored command.");
        }
    } else if (cmd == 17) {
        int max_ticks = 0;
        if (!sscanf(console_input, "addclock %d", &max_ticks)) {
            addMessage("Invalid value.");
            return;
        }

        Clock clock = defaultClock();
        clock->maxTicks = max_ticks;
        addClock(clock);

        char out[MAX_CONSOLE_INPUT] = { 0 };
        snprintf(
            out, MAX_CONSOLE_INPUT, 
            "Added clock: %p",
            (void*)clock
        );

        addMessage(out);
    } else if (cmd == 18) {
        Clock clock;
        if (!sscanf(console_input, "removeclock %p", (void**)&clock)) {
            addMessage("Invalid value.");
            return;
        }

        if (clock != NULL) {
            removeClock(clock);

            addMessage("Removed clock.");
            return;
        }

        addMessage("No clock found.");
    } else if (cmd == 19) {
        Clock clock;
        if (!sscanf(console_input, "viewclock %p", (void**)&clock)) {
            addMessage("Invalid value.");
            return;
        }

        if (clock != NULL) {
            char out[MAX_CONSOLE_INPUT];

            snprintf(
                out, MAX_CONSOLE_INPUT, 
                "Ticks: %d | MaxTicks: %d | Blocked: %d",
                clock->ticks, clock->maxTicks, clock->blocked
            );

            addMessage(out);
            return;
        }

        addMessage("No clock found.");
    }
}

// Processor
void _processCommand() {
    trim(console_input);

    debug_file(dbgOut, 1, "Evaluating console command: '%s'\n", console_input);

    for (int i = 0; i < PATTERNS; i++) {
        debug_file(dbgOut, 1, "- Matching pattern #%d\n", i);

        re_t regex = re_compile(shadow_patterns[i].regex);

        int len = 0;
        int match_idx = re_matchp(regex, console_input, &len);

        if (match_idx != -1) {
            debug_file(dbgOut, 1, "- Matched command: '%d'\n-- Asserting syntax\n", patterns[i].cmd);

            regex = re_compile(patterns[i].regex);
            match_idx = re_matchp(regex, console_input, &len);

            if (match_idx != -1) {
                // _executeCommand(patterns[i].cmd, NULL, 0);
                _executeCommand(patterns[i].cmd, NULL);
            } else {
                debug_file(dbgOut, 1, "-- Invalid syntax.\n");

                char out[MAX_CONSOLE_INPUT];
                snprintf(out, MAX_CONSOLE_INPUT, "Invalid syntax. Type 'help %s' for more info.", patterns[i].cmdName);
                addMessage(out);
            }

            return;
        }
    }

    debug_file(dbgOut, 1, "- No commands found.\n");
    
    char cmd[MAX_CONSOLE_INPUT - 25] = { 0 };
    char* spacePos = strchr(console_input, ' ');
    if (spacePos != NULL) {
        size_t length = spacePos - console_input;
        strncpy(cmd, console_input, length);
        cmd[length] = '\0';
    } else {
        strcpy(cmd, console_input);
    }

    char out[MAX_CONSOLE_INPUT] = { 0 };
    snprintf(out, MAX_CONSOLE_INPUT, "Command '%s' not found.", cmd);

    addMessage(out);
}
#pragma endregion
