#include "mcc_generated_files/mcc.h"
#include <stdint.h>
#include <stdbool.h>

#define OBSTACLE_DISTANCE_CM 20
#define MOTION_TIMEOUT_MS 10000
#define COMMAND_WAIT_TIMEOUT_MS 1000

// === LED Pin Definitions ===
#define LED_RED_LAT     LATCbits.LATC4
#define LED_YELLOW_LAT  LATCbits.LATC5

#define LED_RED_ON()    (LED_RED_LAT = 1)
#define LED_RED_OFF()   (LED_RED_LAT = 0)
#define LED_YELLOW_ON() (LED_YELLOW_LAT = 1)
#define LED_YELLOW_OFF()(LED_YELLOW_LAT = 0)

// === Buzzer Pin Definition ===
//#define BUZZER_LAT     LATBbits.LATB2
//#define BUZZER_ON()    (BUZZER_LAT = 1)
//#define BUZZER_OFF()   (BUZZER_LAT = 0)


void stop_all_motors(void);
void move_forward(void);
void move_backward(void);
uint16_t measure_distance_cm(uint8_t trigPin, uint8_t echoPin);
bool is_obstacle_detected(void);

void trigger_ultrasonic(uint8_t trigPin) {
    LATC |= (1 << trigPin);       // Set TRIG high
    __delay_us(10);
    LATC &= ~(1 << trigPin);      // Set TRIG low
}

uint16_t measure_distance_cm(uint8_t trigPin, uint8_t echoPin) {
    uint16_t timeout = 0;

    trigger_ultrasonic(trigPin);

    while (!(PORTC & (1 << echoPin)) && timeout++ < 60000);
    if (!(PORTC & (1 << echoPin))) return 999;

    TMR1H = 0; TMR1L = 0;
    T1CONbits.TMR1ON = 1;
    timeout = 0;

    while ((PORTC & (1 << echoPin)) && timeout++ < 60000);
    T1CONbits.TMR1ON = 0;

    uint16_t time = (TMR1H << 8) | TMR1L;
    return time / 145;
}

bool is_obstacle_detected() {
    uint16_t dist1 = measure_distance_cm(0, 1);
    uint16_t dist2 = measure_distance_cm(2, 3);
    return (dist1 < OBSTACLE_DISTANCE_CM || dist2 < OBSTACLE_DISTANCE_CM);
}

void move_forward() {
    LATD = 0b01010101;
}

void move_backward() {
    LATD = 0b10101010;
}

void stop_all_motors() {
    LATD = 0x00;
    LED_RED_OFF();
    //BUZZER_OFF();
}

void wait_ms_with_obstacle_check(uint16_t duration_ms) {
    uint16_t elapsed = 0;
    while (elapsed < duration_ms) {
        if (is_obstacle_detected()) {
            EUSART1_Write('\r'); EUSART1_Write('\n');
            EUSART1_Write('O'); EUSART1_Write('b'); EUSART1_Write('s');
            EUSART1_Write('t'); EUSART1_Write('a'); EUSART1_Write('c');
            EUSART1_Write('l'); EUSART1_Write('e'); EUSART1_Write(' ');
            EUSART1_Write('d'); EUSART1_Write('e'); EUSART1_Write('t');
            EUSART1_Write('e'); EUSART1_Write('c'); EUSART1_Write('t');
            EUSART1_Write('e'); EUSART1_Write('d');
            EUSART1_Write('\r'); EUSART1_Write('\n');
            
//            BUZZER_ON();
//            __delay_ms(1000);
//            BUZZER_OFF();

            stop_all_motors();
            return;
        }
        __delay_ms(50);
        elapsed += 50;
    }
}

void main(void)
{
    SYSTEM_Initialize();
    TMR1H = 0;
    TMR1L = 0;
    T1CON = 0x10;  // Timer1 ON, prescaler 1:2
    T1CONbits.TMR1ON = 0;

    stop_all_motors();
    LED_RED_OFF();
    LED_YELLOW_OFF();
//    BUZZER_OFF();

    while (1)
    {
        uint16_t waitTime = 0;
        char cmd = 0;

        while (waitTime < COMMAND_WAIT_TIMEOUT_MS) {
            if (EUSART1_is_rx_ready()) {
                cmd = EUSART1_Read();

                EUSART1_Write('\r'); EUSART1_Write('\n');
                EUSART1_Write('R'); EUSART1_Write('e'); EUSART1_Write('c');
                EUSART1_Write('e'); EUSART1_Write('i'); EUSART1_Write('v');
                EUSART1_Write('e'); EUSART1_Write('d'); EUSART1_Write(':');
                EUSART1_Write(' '); EUSART1_Write(cmd);
                EUSART1_Write('\r'); EUSART1_Write('\n');
                break;
            }
            __delay_ms(100);
            waitTime += 100;
        }

        if (cmd == 'R' || cmd == 'r') {
            LED_YELLOW_ON();  // Prepare to move
            __delay_ms(2000);
            if (!is_obstacle_detected()) {
                LED_YELLOW_OFF();
                LED_RED_ON();
                move_forward();
                wait_ms_with_obstacle_check(MOTION_TIMEOUT_MS);
            } else {
                stop_all_motors();
            }
            LED_YELLOW_OFF();
        }
        else if (cmd == 'L' || cmd == 'l') {
            LED_YELLOW_ON();
            __delay_ms(2000);
            if (!is_obstacle_detected()) {
                LED_YELLOW_OFF();
                LED_RED_ON();
                move_backward();
                wait_ms_with_obstacle_check(MOTION_TIMEOUT_MS);
            } else {
                stop_all_motors();
            }
            LED_YELLOW_OFF();
        } else if (cmd == 'S' || cmd == 's') {
            stop_all_motors();
        } else {
            stop_all_motors();
        }

        LED_RED_OFF();  // Turn off after motion complete
        LED_YELLOW_OFF();
    }
}
