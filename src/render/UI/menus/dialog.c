#include "dialog.h"

char* g_dialog_text;
char**** g_dialog_page_data;
int g_dialog_control[4] = { 0, 0, 0, 1 }; // max per line, max lines, max page, allow control
int (*g_dialog_keybinds)(int key); // additional keybinds
static int page_control[2] = { 0 }; // max pages, current page

void draw_Dialog(Menu menu) {
    wmove(menu->wnd, 10, 10);
    box(menu->wnd, 0, 0);

    char*** page_data = *g_dialog_page_data;

    // int ln = 1;
    for (int i = 0; i < g_dialog_control[1]; i++) {
        if (page_data[page_control[0]] == NULL) break;
        mvwprintw(menu->wnd, 1 + i, 1, "%s", page_data[page_control[0]][i]);
    }

    if (page_control[0] + 1 >= g_dialog_control[2]) mvwprintw(menu->wnd, g_ui_size[0] - 2, g_ui_size[1] - 3, "%s", "X");
    else mvwprintw(menu->wnd, g_ui_size[0] - 2, g_ui_size[1] - 5, "%s", ">>>");

    // int diag_len = strlen(g_dialog_text);
    // int diag_lines = diag_len / g_dialog_control[0];
    // int diag_pages = diag_lines / g_dialog_control[1];

    // mvwprintw(menu->wnd, 1, 1, "%s", g_dialog_text);
    // refresh();

    refresh();
}

void tick_Dialog() {
    return;
}

void handle_Dialog_keybinds(int key) {
    if (!g_dialog_control[3]) return;

    switch(key) {
        // case 10:
        // case 13:
        //     if (page_control[0] + 1 >= g_dialog_control[2]) {
        //         closeMenu(MENU_DIALOG);
        //         page_control[0] = 0;
        //     } else page_control[0]++;

        //     // closeMenu(MENU_DIALOG);
        //     break;
        // case KEY_UP:
        //     closeMenu(MENU_DIALOG);
        default:
            if (g_dialog_keybinds != NULL) {
                int res = g_dialog_keybinds(key);
                if (!res) break;
            }

            if (page_control[0] + 1 >= g_dialog_control[2]) {
                closeMenu(MENU_DIALOG);
                page_control[0] = 0;
            } else page_control[0]++;
            break;
    }   
}

void cleanup_Dialog() {
    g_dialog_text = NULL;
    g_dialog_page_data;

    for (int i = 0; i < g_dialog_control[2]; i++) {
        for (int j = 0; j < g_dialog_control[1]; j++) {
            free(g_dialog_page_data[i][j]);
        }

        free(g_dialog_page_data[i]);
    }

    free(g_dialog_page_data);
    g_dialog_page_data = NULL;


    g_dialog_control[0] = 0;
    g_dialog_control[1] = 0;
    g_dialog_control[2] = 0;
    g_dialog_control[3] = 1;

    g_dialog_keybinds = NULL;
    page_control[0] = 0;
}

void rectangle(WINDOW* win, int y1, int x1, int y2, int x2) {
    mvwhline(win, y1, x1, 0, x2-x1);
    mvwhline(win, y2, x1, 0, x2-x1);
    mvwvline(win, y1, x1, 0, y2-y1);
    mvwvline(win, y1, x2, 0, y2-y1);
    mvwaddch(win, y1, x1, ACS_ULCORNER);
    mvwaddch(win, y2, x1, ACS_LLCORNER);
    mvwaddch(win, y1, x2, ACS_URCORNER);
    mvwaddch(win, y2, x2, ACS_LRCORNER);
}

/*
 * Calculates the metadata used to generate the pages for the shown dialog.
 * It's dependent on `g_dialog_control`, which specifies the control for the dialog.
 */
int calculate_dialog_metadata(char* text, char**** ret) {
    int length = strlen(text);
    int max_chars_per_line = g_dialog_control[0];
    int max_lines = g_dialog_control[1];

    char linebuf[max_chars_per_line];
    char buf[max_chars_per_line];
    int j = 0;
    int lj = 0;
    
    memset(linebuf, 0, max_chars_per_line * sizeof(char));
    memset(buf, 0, max_chars_per_line * sizeof(char));

    int line_count = 1;
    CharNode* og_root = char_node("");
    CharNode* root = og_root;

    for (int i = 0; i < length; i++) {
        buf[j++] = text[i];

        if (text[i] == '\n') {
            if (lj + j >= max_chars_per_line) {
                root->value = strdup(linebuf);
                root->next = char_node("");
                root = root->next;

                line_count++;

                memset(linebuf, 0, max_chars_per_line * sizeof(char));
                lj = 0;

                memcpy(linebuf, buf, j - 1);
                lj += j;

                memset(buf, 0, max_chars_per_line * sizeof(char));
                j = 0;
            } else {
                memcpy(linebuf + lj, buf, j - 1);

                root->value = strdup(linebuf);
                root->next = char_node("");
                root = root->next;

                line_count++;

                memset(linebuf, 0, max_chars_per_line * sizeof(char));
                memset(buf, 0, max_chars_per_line * sizeof(char));
                lj = 0;
                j = 0;
            }

            continue;
        }

        if (text[i] == ' ') {
            if (lj + j >= max_chars_per_line) {
                root->value = strdup(linebuf);
                root->next = char_node("");
                root = root->next;

                line_count++;

                // linebuf[0] = '\0';
                memset(linebuf, 0, max_chars_per_line * sizeof(char));
                lj = 0;

                memcpy(linebuf, buf, j);
                lj += j;

                // buf[0] = '\0';
                memset(buf, 0, max_chars_per_line * sizeof(char));
                j = 0;
            } else {
                memcpy(linebuf + lj, buf, j);

                lj += j;

                // buf[0] = '\0';
                memset(buf, 0, max_chars_per_line * sizeof(char));
                j = 0;
            }
        } else {
            if (lj == 0 && j >= max_chars_per_line) {
                root->value = strndup(buf, j - 1);
                root->next = char_node("");
                root = root->next;

                line_count++;

                memcpy(buf, buf + j - 1, 1);
                j = 1;
            }

            if (lj + j >= max_chars_per_line) {
                root->value = strdup(linebuf);
                root->next = char_node("");
                root = root->next;

                line_count++;

                // linebuf[0] = '\0';
                memset(linebuf, 0, max_chars_per_line * sizeof(char));
                lj = 0;
            }
        }
    }

    if (lj != 0) {
        if (j != 0) {
            if (lj + j >= max_chars_per_line) {
                root->value = strdup(linebuf);
                root->next = char_node("");
                root = root->next;

                line_count++;

                root->value = strdup(buf);
            } else {
                memcpy(linebuf + lj, buf, j);

                root->value = strdup(linebuf);
            }
        }
    } else if (j != 0) {
        root->value = strdup(buf);
    }

    // Paginate this mf
    root = og_root;

    int page_count = ceil((double)line_count / max_lines);
    char*** page_data = malloc(page_count * sizeof(char**));

    for (int i = 0; i < page_count; i++) {
        if (root == NULL) break;

        page_data[i] = malloc(max_lines * sizeof(char*));

        for (int j = 0; j < max_lines; j++) {
            if (root == NULL) break;

            page_data[i][j] = strdup(root->value);
            root = root->next;
        }
    }

    *ret = page_data;

    return page_count;
}
