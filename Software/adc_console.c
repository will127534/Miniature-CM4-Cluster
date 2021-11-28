#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"

#define N_SAMPLES 1000
uint16_t sample_buf[N_SAMPLES];

#define RST1 10
#define RST2 14
#define RST3 18
#define RST4 22

#define EN1 13
#define EN2 17
#define EN3 21
#define EN4 25

#define RUN1 12
#define RUN2 16
#define RUN3 20
#define RUN4 24

#define BOOT1 11
#define BOOT2 15
#define BOOT3 19
#define BOOT4 23

uint8_t Reset[4] = {RST1,RST2,RST3,RST4};
uint8_t Enable[4] = {EN1,EN2,EN3,EN4};
uint8_t Run[4] = {RUN1,RUN2,RUN3,RUN4};
uint8_t Boot[4] = {BOOT1,BOOT2,BOOT3,BOOT4};

void printhelp() {
    puts("\nCommands:");
    puts("c0, ...\t: Select CM4 channel n");
    puts("s\t: Sample once");
    puts("r\t: Reset CM4");
    puts("U\t: Assert USB Boot");
    puts("u\t: Deassert USB Boot");
    puts("e\t: Enable power");
    puts("d\t: Disable power");
    puts("a\t: Sample all CM4 current measurements");


}

void __not_in_flash_func(adc_capture)(uint16_t *buf, size_t count) {
    adc_fifo_setup(true, false, 0, false, false);
    adc_run(true);
    for (int i = 0; i < count; i = i + 1)
        buf[i] = adc_fifo_get_blocking();
    adc_run(false);
    adc_fifo_drain();
}

int main(void) {

    // Set all pins to input (as far as SIO is concerned)
    //gpio_set_dir_all_bits(-1);
    for (int i = 0; i < 4; ++i){
        gpio_init(Reset[i]);
        gpio_init(Enable[i]);
        gpio_init(Run[i]);
        gpio_init(Boot[i]);

        gpio_set_dir(Reset[i],GPIO_IN);
        gpio_set_dir(Enable[i],GPIO_OUT);
        gpio_set_dir(Run[i],GPIO_OUT);
        gpio_set_dir(Boot[i],GPIO_OUT);

        //gpio_put(Reset[i],1);
        gpio_put(Enable[i],0);
        gpio_put(Run[i],1);
        gpio_put(Boot[i],1);
    }

    for (int i = 26; i < 30; ++i){
        gpio_init(i);
    }


    stdio_init_all();
    adc_init();
    adc_set_temp_sensor_enabled(true);

    uint8_t channel = 0;
    while (1) {
        char c = getchar();
        printf("%c", c);
        switch (c) {
            case 'c':
                c = getchar(); 
                printf("%c\n", c);
                if (c < '0' || c > '3') {
                    printf("Unknown input channel\n");
                    printhelp();
                } else {
                    adc_select_input(c - '0');
                    printf("Switched to channel %c\n", c);
                    channel = c - '0';
                }
                break;
            case 'r': {
                gpio_put(Run[channel],0);
                sleep_ms(100);
                gpio_put(Run[channel],1);
                printf("Reseting:%d\n", channel);
                break;
            }
            case 'u': {
                gpio_put(Boot[channel],1);
                printf("USB boot deassert:%d\n", channel);
                break;
            }
            case 'U': {
                gpio_put(Boot[channel],0);
                printf("USB boot assert:%d\n", channel);
                break;
            }
            case 'd': {
                gpio_put(Enable[channel],0);
                printf("Disable:%d\n", channel);
                break;
            }
            case 'e': {
                gpio_put(Enable[channel],1);
                printf("Enable:%d\n", channel);
                break;
            }
            case 's': {
                uint32_t result = adc_read();
                const float conversion_factor = 3.3f / (1 << 12);
                printf("\n0x%03x -> %f V\n", result, result * conversion_factor);
                break;
            }
            case 'a': {
                adc_select_input(0);
                adc_capture(sample_buf, N_SAMPLES);
                double result_ch0 = 0;
                for (int i = 0; i < N_SAMPLES; i = i + 1)
                    result_ch0 += sample_buf[i]/1000.0;
                adc_select_input(1);
                adc_capture(sample_buf, N_SAMPLES);
                double result_ch1 = 0;
                for (int i = 0; i < N_SAMPLES; i = i + 1)
                    result_ch1 += sample_buf[i]/1000.0;
                adc_select_input(2);
                adc_capture(sample_buf, N_SAMPLES);
                double result_ch2 = 0;
                for (int i = 0; i < N_SAMPLES; i = i + 1)
                    result_ch2 += sample_buf[i]/1000.0;
                adc_select_input(3);
                adc_capture(sample_buf, N_SAMPLES);
                double result_ch3 = 0;
                for (int i = 0; i < N_SAMPLES; i = i + 1)
                    result_ch3 += sample_buf[i]/1000.0;
                const float conversion_factor = 3.3f / (1 << 12) * 1000.0;
                printf("\n%f mA,%f mA,%f mA,%f mA\n", result_ch0 * conversion_factor, result_ch1 * conversion_factor, result_ch2 * conversion_factor, result_ch3 * conversion_factor);
                adc_select_input(channel);
                break;
            }
            case 'S': {
                printf("\nStarting capture\n");
                adc_capture(sample_buf, N_SAMPLES);
                printf("Done\n");
                for (int i = 0; i < N_SAMPLES; i = i + 1)
                    printf("%03x\n", sample_buf[i]);
                break;
            }
            case '\n':
            case '\r':
                break;
            case 'h':
                printhelp();
                break;
            default:
                printf("\nUnrecognised command: %c\n", c);
                printhelp();
                break;
        }
    }
}
