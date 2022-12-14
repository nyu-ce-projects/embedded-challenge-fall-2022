#include "drivers/LCD_DISCO_F429ZI.h"
#include "mbed.h"

 
DigitalOut trigger(PA_6);
//DigitalOut myled(LED1); //monitor trigger
//DigitalOut myled2(LED2); //monitor echo
DigitalIn  echo(PA_7);
int distance_new = 0;
int correction = 0;
Timer sonar;

LCD_DISCO_F429ZI lcd;
uint8_t msg[50];

void display_voltage(int value)
{
    lcd.SetTextColor(LCD_COLOR_BLACK);
    lcd.Clear(LCD_COLOR_LIGHTCYAN);
    lcd.SetBackColor(LCD_COLOR_LIGHTCYAN);
    BSP_LCD_DrawHLine(0, LINE(4), 240);
    lcd.DisplayStringAt(0, LINE(1), (uint8_t *)"START INCREASING", CENTER_MODE);
    lcd.DisplayStringAt(0, LINE(2), (uint8_t *)"THE PRESSURE VALUE", CENTER_MODE);
    lcd.DisplayStringAt(0, LINE(3), (uint8_t *)"TILL IT IS 200mmHg", CENTER_MODE);
    sprintf((char *)msg, "%d mmHg", value);
    lcd.DisplayStringAt(0, LINE(10), (uint8_t *)&msg, CENTER_MODE);
    BSP_LCD_DrawRect(40, LINE(8), 160, LINE(5));
}


int main()
{
    sonar.reset();
// measure actual software polling timer delays
// delay used later in time correction
// start timer
    sonar.start();
// min software polling delay to read echo pin
    while (echo==2) {};
    //myled2 = 0;
// stop timer
    sonar.stop();
// read timer
    correction = sonar.read_us();
    printf("Approximate software overhead timer delay is %d uS\n\r",correction);
 
//Loop to read Sonar distance values, scale, and print
    while(1) {
// trigger sonar to send a ping
        trigger = 1;
        //myled = 1;
        //myled2 = 0;
        sonar.reset();
        wait_us(10.0);
        trigger = 0;
        //myled = 0;
//wait for echo high
        while (echo==0) {};
        //myled2=echo;
//echo high, so start timer
        sonar.start();
//wait for echo low
        while (echo==1) {};
//stop timer and read value
        sonar.stop();
//subtract software overhead timer delay and scale to cm
        distance_new = (sonar.read_us()-correction)/5.8;
        //myled2 = 0;
        //printf(" %d cm \n\r", distance_new);
        display_voltage(distance_new);
//wait so that any echo(s) return before sending another ping
        wait_us(500000);
    }
}