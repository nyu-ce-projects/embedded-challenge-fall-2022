#include <mbed.h>

AnalogIn   ain(PA_0, 3);
DigitalOut dout(PG_13);
uint32_t counter=0; 

int mainStretchSensor() {
    
  while (1) {
        // test the voltage on the initialized analog pin
        //  and if greater than 0.3 * VCC set the digital pin
        //  to a logic 1 otherwise a logic 0
        printf("normal %f", ain.read_voltage());
        volatile float voltage = ain.read_voltage();
        if(voltage > 0.90) {
            printf("no breathing\n");
            counter++;
            if (counter >= 10){
              dout=1; 
              //wait(2);
            }
        } else {
            printf("breathing\n");
            dout=0;
            counter=0;
        }

        //display_voltage(voltage);
        
        // print the percentage and 16 bit normalized values
        //printf("\npercentage: %.3f\n", voltage*100.0f);
        //printf("normalized: 0x%04X \n", ain.read_u16());
        thread_sleep_for(1000);
  }
  return 0;
}
