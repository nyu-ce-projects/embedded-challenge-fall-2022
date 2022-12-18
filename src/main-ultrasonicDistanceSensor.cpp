#include "drivers/LCD_DISCO_F429ZI.h"
#include "mbed.h"

 
DigitalOut trigger(PA_6);
DigitalOut dout(PG_13);
//DigitalOut myled(LED1); //monitor trigger
//DigitalOut myled2(LED2); //monitor echo
DigitalIn  echo(PA_7);
int distance_new = 0;
int distance_old = -1;
int correction = 0;
int delta = 0;
int counter = 0;
bool breathe_flag = true;
Timer sonar;

LCD_DISCO_F429ZI lcd;
uint8_t msg1[50];
uint8_t msg2[50];

void display_status(int value)
{
    lcd.SetTextColor(LCD_COLOR_BLACK);
    lcd.Clear(LCD_COLOR_LIGHTCYAN);
    lcd.SetBackColor(LCD_COLOR_LIGHTCYAN);
    BSP_LCD_DrawHLine(0, LINE(4), 240);
    lcd.DisplayStringAt(0, LINE(1), (uint8_t *)"Just Breathe", CENTER_MODE);
    lcd.DisplayStringAt(0, LINE(2), (uint8_t *)"KEEP THE SENSOR", CENTER_MODE);
    lcd.DisplayStringAt(0, LINE(3), (uint8_t *)"IN A RANGE OF ~300 MM", CENTER_MODE);
    if(breathe_flag)
    {
        sprintf((char *)msg1, "|| Breathing ||");
        dout=0;
    }
    else
    {
        sprintf((char *)msg1, "!! Not Breathing !!");
        dout=1;
    }
    sprintf((char *)msg2, "Distance: %d mm", value);
    lcd.DisplayStringAt(5, LINE(10), (uint8_t *)&msg1, CENTER_MODE);
    lcd.DisplayStringAt(5, LINE(12), (uint8_t *)&msg2, CENTER_MODE);
    //BSP_LCD_DrawRect(5, LINE(8), 200, LINE(5));
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
        breathe_flag = true;
        trigger = 1;
        sonar.reset();
        wait_us(10.0);
        trigger = 0;
//wait for echo high
        while (echo==0) {};
//echo high, so start timer
        sonar.start();
//wait for echo low
        while (echo==1) {};
//stop timer and read value
        sonar.stop();
//subtract software overhead timer delay and scale to cm
        distance_new = (sonar.read_us()-correction)/5.8;
        //printf(" %d cm \n\r", distance_new);
        delta = abs(distance_old - distance_new);
        distance_old = distance_new;
        if(delta <= 3)
        {  if(counter < 10)
              counter++ ; //start counter
        }
        else
        {
            counter = 0; //reset counter
        }
        
        if(counter >= 10)
        {
          breathe_flag = false;
          display_status(distance_new);
          break;
        }
        display_status(distance_new);


        
//wait so that any echo(s) return before sending another ping
        wait_us(1000000);
    }
}