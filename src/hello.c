#include <rp6502.h>
#include <stdio.h>
#include <stdbool.h>

char get_char() {
    char c;
    while (1) {
        if (!(RIA.ready & RIA_READY_RX_BIT))
            continue;
        c = RIA.rx;
        return c;
    }
}

char is_ctrl(char c) {
    /*
    backspace == 'ctrl-h'
    enter == 'ctrl-m'
    tab == 'ctrl-i'
    escape = 'ctrl-['
    delete == 'ctrl-[ 0x7f'
    left == 'ctrl-[ [ D'
    right == 'ctrl-[ [ C'
    */
    if (c < 0x20) 
        return 0x60 + c; // 0x01 == 'ctrl-a'
    return 0;
}

int main() {
    char c, ctrl;
    while (1) {
        c = get_char();
        if (ctrl = is_ctrl(c)) {
            switch (ctrl) {
            case 'c':
                return 0;
            default:
                printf("%02x (%02x)\n", c, ctrl);
                continue;
            }
        } else {
            printf("%02x ('%c')\n", c, c);
        }
    }
}
