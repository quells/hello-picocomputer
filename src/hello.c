#include <rp6502.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

// XRAM
#define CANVAS_DATA 0x0000
#define RULER_DATA 0x1000
#define CANVAS_CONFIG 0xFF00
#define RULER_CONFIG 0xFF10
#define KEYBOARD_INPUT 0xFF20
#define DEFAULT_PALETTE 0xFFFF

void draw_ruler() {
    unsigned i = 17*17;
    RIA.addr0 = RULER_DATA;
    RIA.step0 = 1;
    for (; i; i--)
        RIA.rw0 = 0x00;

    RIA.addr0 = RULER_DATA + 1;
    RIA.step0 = 1;
    for (i = 16; i; i--)
        RIA.rw0 = i - 1;

    RIA.addr0 = RULER_DATA + 17;
    RIA.step0 = 17;
    for (i = 16; i; i--)
        RIA.rw0 = i - 1;
}

void loop() {
    unsigned char i = 1;
    unsigned char c;
    unsigned char m = 0x80;
    RIA.addr0 = KEYBOARD_INPUT;
    RIA.step0 = 1;
    RIA.addr1 = CANVAS_DATA;
    RIA.step1 = 1;

    c = RIA.rw0;
    do {
        RIA.rw1 = (c & m) ? 0xFF : 0x00;
    } while (m >>= 1);

    m = 0x80;
    do {
        c = RIA.rw0;
        do {
            RIA.rw1 = (c & m) ? 0xFF : 0x00;
        } while (m >>= 1);
        m = 0x80;
        i++;
    } while (i < 32);
}

void main() {
    xreg_vga_canvas(2);
    xram0_struct_set(CANVAS_CONFIG, vga_mode3_config_t, x_wrap, false);
    xram0_struct_set(CANVAS_CONFIG, vga_mode3_config_t, y_wrap, false);
    xram0_struct_set(CANVAS_CONFIG, vga_mode3_config_t, x_pos_px, 1);
    xram0_struct_set(CANVAS_CONFIG, vga_mode3_config_t, y_pos_px, 1);
    xram0_struct_set(CANVAS_CONFIG, vga_mode3_config_t, width_px, 16);
    xram0_struct_set(CANVAS_CONFIG, vga_mode3_config_t, height_px, 16);
    xram0_struct_set(CANVAS_CONFIG, vga_mode3_config_t, xram_data_ptr, CANVAS_DATA);
    xram0_struct_set(CANVAS_CONFIG, vga_mode3_config_t, xram_palette_ptr, DEFAULT_PALETTE);

    xreg_vga_canvas(1);
    xram0_struct_set(RULER_CONFIG, vga_mode3_config_t, x_wrap, false);
    xram0_struct_set(RULER_CONFIG, vga_mode3_config_t, y_wrap, false);
    xram0_struct_set(RULER_CONFIG, vga_mode3_config_t, x_pos_px, 0);
    xram0_struct_set(RULER_CONFIG, vga_mode3_config_t, y_pos_px, 0);
    xram0_struct_set(RULER_CONFIG, vga_mode3_config_t, width_px, 17);
    xram0_struct_set(RULER_CONFIG, vga_mode3_config_t, height_px, 17);
    xram0_struct_set(RULER_CONFIG, vga_mode3_config_t, xram_data_ptr, RULER_DATA);
    xram0_struct_set(RULER_CONFIG, vga_mode3_config_t, xram_palette_ptr, DEFAULT_PALETTE);
    draw_ruler();

    xreg_vga_mode(3, 2, RULER_CONFIG, 0);
    xreg_vga_mode(3, 2, CANVAS_CONFIG, 1);

    xreg_ria_keyboard(KEYBOARD_INPUT);
    while (1)
        loop();
}
