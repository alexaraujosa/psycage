#include "console.h"

extern int EXIT;

static int SETUP = 0;

#define MAX_CONSOLE_INPUT 255
static char console_input[255];
static int console_input_ind = 0;
static int console_input_cursor = 0;

static char** console_output;
static int console_output_ind = 0;
static int console_output_control[4] = { 0, 0, 0, 0 }; // max lines, max line length, latest line, scroll offset

#define MAX_BLINK_TICKS 10
static int blink_ticks = 0;
static int blink = 0;

static int console_win[2] = { 0, 0 }; // mrows, mcols

static FILE* consoleOut;

void setupConsole();
void _processCommand();
void _makeProcessorPatterns();

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
        mvwprintw(menu->wnd, curLine--, 2, console_output[i]);
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
};

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
        case 10: case 13:
            char inp[MAX_CONSOLE_INPUT + 3] = { 0 };
            strcpy(inp, "> \0");
            strcat(inp, console_input);
            addMessage(inp);
            
            _processCommand();
            memset(console_input, 0, MAX_CONSOLE_INPUT * sizeof(char));
            console_input_ind = 0;
            console_input_cursor = 0;
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

void createSessionFile() {
    char out_path[PATH_MAX];
    snprintf(out_path, sizeof(out_path), "%.*s/logs/console-", BIN_PATH_LEN, BIN_PATH);

	time_t now = time(NULL);
    struct tm* tmInfo = localtime(&now);
    size_t dateFormatSize = 20;
    size_t new_out_len = BIN_PATH_LEN + sizeof("/logs/console-") - 1 + dateFormatSize;

    char* new_out_path = malloc(new_out_len);
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

void setupConsole() {
    console_output_control[0] = 2 * g_renderstate->nrows;
    console_output_control[1] = g_renderstate->ncols - 8;

    console_output = (char**)malloc(console_output_control[0] * sizeof(char*));
    for (int i = 0; i < console_output_control[0]; i++) {
        console_output[i] = (char*)malloc(console_output_control[1] * sizeof(char));
        console_output[i][0] = '\0';
    }

    createSessionFile();

    // addMessage("Hello world");
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

#define PATTERNS 13
struct pattern shadow_patterns[PATTERNS];
struct pattern patterns[PATTERNS];

struct pattern _makeProcessorPattern(char* regex, char* name, int cmd) {
    struct pattern node = {
        regex: regex,
        cmdName: name,
        cmd: cmd
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

    patterns[8] = _makeProcessorPattern("^getitembyname [a-zA-Z]+$", "getitembyname", 9);
    shadow_patterns[8] = _makeProcessorPattern("^getitembyname", "getitembyname", 9);

    patterns[9] = _makeProcessorPattern("^getitembyid [0-9]+$", "getitembyid", 10);
    shadow_patterns[9] = _makeProcessorPattern("^getitembyid", "getitembyid", 10);

    patterns[10] = _makeProcessorPattern("^gettranslation [a-zA-Z_]+ [a-zA-Z0-9_.]+$", "gettranslation", 11);
    shadow_patterns[10] = _makeProcessorPattern("^gettranslation", "gettranslation", 11);

    patterns[11] = _makeProcessorPattern("^getwindows$", "getwindows", 12);
    shadow_patterns[11] = _makeProcessorPattern("^getwindows", "getwindows", 12);

    patterns[12] = _makeProcessorPattern("^maxfov ?[01]*$", "maxfov", 13);
    shadow_patterns[12] = _makeProcessorPattern("^maxfov", "maxfov", 13);

}

void _executeCommand(int cmd, void* override, int overrideN) {
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

            addMessage("DLM Console, version 1.4.0");
            addMessage("These commands are defined internally. Type 'help' to see this list.");
            addMessage("Type 'help' [command] to see more info about a command.");
            addMessage("Arguments in the form [arg] are required and arguments in the form <arg> are optional.");
            addMessage("");
            addMessage("clear          | Clears the console output.");
            addMessage("damagePlayer   | Damages the player.");
            addMessage("exit           | Closes the console terminal.");
            addMessage("getitembyid    | Gets an item through it's ID.");
            addMessage("getitembyname  | Gets an item through it's name.");
            addMessage("gettranslation | Gets a translation through it's translation key.");
            addMessage("getwindows     | Gets the number of windows created.");
            addMessage("godmode        | (De)activates the GodMode.");
            addMessage("healPlayer     | Heals the player.");
            addMessage("help           | Displays this message.");   
            addMessage("reset          | [CHEAT] Resets the console.");  
            addMessage("maxfov         | [CHEAT] (De)activates the Max FOV.");    
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
                _executeCommand(2, "exit", 4);
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
            "Healed player with %d HP (%d effective).", 
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
            "Damaged player with %d HP (%d effective).", 
            param, prevHp - g_gamestate->player->entity->health
        );

        addMessage(out);
    } else if (cmd == 7) {
        _executeCommand(1, NULL, 0);
    } else if (cmd == 8) {
        nukeConsoleMenu();
        addMessage("Terminal nuked");
    } else if (cmd == 9) {
        char param[50];
        if (!sscanf(console_input, "getitembyname %s", &param)) {
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
        if (!sscanf(console_input, "getitembyid %s", &param)) {
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
        char param[MAX_CONSOLE_INPUT];
        char locale[6];
        if (!sscanf(console_input, "gettranslation %5s %s", &locale, &param)) {
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
            debug_file(dbgOut, 1, "- Matched command: '%d'\n-- Asserting syntax", patterns[i].cmd);

            regex = re_compile(patterns[i].regex);
            match_idx = re_matchp(regex, console_input, &len);

            if (match_idx != -1) {
                _executeCommand(patterns[i].cmd, NULL, 0);
            } else {
                debug_file(dbgOut, 1, "-- Invalid syntax.");

                char out[MAX_CONSOLE_INPUT];
                snprintf(out, MAX_CONSOLE_INPUT, "Invalid syntax. Type 'help %s' for more info.", patterns[i].cmdName);
                addMessage(out);
            }

            return;
        }
    }

    debug_file(dbgOut, 1, "- No commands found.\n");

    char cmd[MAX_CONSOLE_INPUT] = { 0 };
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
