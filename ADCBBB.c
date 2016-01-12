
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/select.h>
#include <fcntl.h>
#include <math.h>
#include <util/util.h>
#include <errno.h>



#define SYSFS_ADC_DIR "/sys/bus/iio/devices/iio:device0/in_voltage0_raw"
#define SYSFS_ADC_DIR2 "/sys/bus/iio/devices/iio:device0/in_voltage6_raw"
#define MAX_BUFF 64



//check that the character is numeric
 int caracter_valido(const char cad)
 {
     return cad >= '0' && cad <='9';
 }
 

 // converts an string into a integer

int mi_atoi(const char *cad, int tam)
 {
     int i=tam-1, entero=0, pos=1;
     while (i>=0)
     {
         while (caracter_valido(cad[i])){
             entero=entero+(cad[i]-48)*pos;
             pos=pos*10;
             i--;
         }
    pos=1;
    i--;
     }
     return entero;
 }

int main(void) {

    int value;
   int fd;
   int value2;
   int fd2;
  char buf[MAX_BUFF];
  char ch[5];   //Update
  char ch2[5];   //Update
  ch[4] = 0;    //Update
  
    
   int GPIOPin=60, /* GPIO1_28 or pin 12 on the P9 header */
 
    printf("\nStarting GPIO output program\n");
    FILE *myOutputHandle = NULL;
    char setValue[4], GPIOString[4], GPIOValue[64], GPIODirection[64];
    sprintf(GPIOString, "%d", GPIOPin);
    sprintf(GPIOValue, "/sys/class/gpio/gpio%d/value", GPIOPin);
    sprintf(GPIODirection, "/sys/class/gpio/gpio%d/direction", GPIOPin);
    
    // Export the pin
    if ((myOutputHandle = fopen("/sys/class/gpio/export", "ab")) == NULL){
        printf("Unable to export GPIO pin\n");
        return 1;
    }
    
    strcpy(setValue, GPIOString);
    fwrite(&setValue, sizeof(char), 2, myOutputHandle);
    fclose(myOutputHandle);
 
    // Set direction of the pin to an output
     if ((myOutputHandle = fopen(GPIODirection, "rb+")) == NULL){
        printf("Unable to open direction handle\n");
        return 1;
    }
    
    strcpy(setValue,"out");
    fwrite(&setValue, sizeof(char), 3, myOutputHandle);
    fclose(myOutputHandle);
    
    
    // export pin 2
    GPIOPin=50;
    sprintf(GPIOString, "%d", GPIOPin);
    sprintf(GPIOValue, "/sys/class/gpio/gpio%d/value", GPIOPin);
    sprintf(GPIODirection, "/sys/class/gpio/gpio%d/direction", GPIOPin);
    
    // Export the pin
    if ((myOutputHandle = fopen("/sys/class/gpio/export", "ab")) == NULL){
        printf("Unable to export GPIO pin\n");
        return 1;
    }
    
    strcpy(setValue, GPIOString);
    fwrite(&setValue, sizeof(char), 2, myOutputHandle);
    fclose(myOutputHandle);
 
    // Set direction of the pin to an output
     if ((myOutputHandle = fopen(GPIODirection, "rb+")) == NULL){
        printf("Unable to open direction handle\n");
        return 1;
    }
    
    strcpy(setValue,"out");
    fwrite(&setValue, sizeof(char), 3, myOutputHandle);
    fclose(myOutputHandle);
   
   
   

    while(1) {
        
        //read adc ch0
        snprintf(buf, sizeof(buf), SYSFS_ADC_DIR);
      fd = open(buf, O_RDONLY);
      read(fd,ch,4);
      close(fd);
      value= mi_atoi(ch, strlen(ch));
      value= (value*100)/2667;
      sleep(0.1);
      
      //read adc ch 6
        snprintf(buf, sizeof(buf), SYSFS_ADC_DIR2);
      fd2 = open(buf, O_RDONLY);
      read(fd2,ch2,4);
      close(fd2);
      value2= mi_atoi(ch2, strlen(ch));
   
      // if the solar power is high and the current from the motor is low, then shut down the fuel cell and turn the solar charger on, otherwise the fuel cell is always on.
      if (value > 80 && value2 < 1000){
          GPIOPin=60;
        
        sprintf(GPIOString, "%d", GPIOPin);
        sprintf(GPIOValue, "/sys/class/gpio/gpio%d/value", GPIOPin);
        sprintf(GPIODirection, "/sys/class/gpio/gpio%d/direction", GPIOPin);
        
        if ((myOutputHandle = fopen("/sys/class/gpio/export", "ab")) == NULL){
        printf("Unable to export GPIO pin\n");
        return 1;
        }
       
        strcpy(setValue, GPIOString);
        fwrite(&setValue, sizeof(char), 2, myOutputHandle);
        fclose(myOutputHandle);
        
        // Set output to low
        if ((myOutputHandle = fopen(GPIOValue, "rb+")) == NULL){
            printf("Unable to open value handle\n");
            return 1;
        
        }
        strcpy(setValue, "0"); // Set value low
        fwrite(&setValue, sizeof(char), 1, myOutputHandle);
        fclose(myOutputHandle);
        sleep(0.5); // wait for 1 sec
        
        GPIOPin=50;
        
        sprintf(GPIOString, "%d", GPIOPin);
        sprintf(GPIOValue, "/sys/class/gpio/gpio%d/value", GPIOPin);
        sprintf(GPIODirection, "/sys/class/gpio/gpio%d/direction", GPIOPin);
        
        if ((myOutputHandle = fopen("/sys/class/gpio/export", "ab")) == NULL){
        printf("Unable to export GPIO pin\n");
        return 1;
        }
       
        strcpy(setValue, GPIOString);
        fwrite(&setValue, sizeof(char), 2, myOutputHandle);
        fclose(myOutputHandle);
       
        // Set output to high
        if ((myOutputHandle = fopen(GPIOValue, "rb+")) == NULL){
            printf("Unable to open value handle\n");
            return 1;
        }
        
        strcpy(setValue, "1"); // Set value high
        fwrite(&setValue, sizeof(char), 1, myOutputHandle);
        fclose(myOutputHandle);
        sleep(0.5); // wait for 1 sec
    }else{
          GPIOPin=50;
        
        sprintf(GPIOString, "%d", GPIOPin);
        sprintf(GPIOValue, "/sys/class/gpio/gpio%d/value", GPIOPin);
        sprintf(GPIODirection, "/sys/class/gpio/gpio%d/direction", GPIOPin);
        
        if ((myOutputHandle = fopen("/sys/class/gpio/export", "ab")) == NULL){
        printf("Unable to export GPIO pin\n");
        return 1;
        }
       
        strcpy(setValue, GPIOString);
        fwrite(&setValue, sizeof(char), 2, myOutputHandle);
        fclose(myOutputHandle);
        
        // Set output to low
        if ((myOutputHandle = fopen(GPIOValue, "rb+")) == NULL){
            printf("Unable to open value handle\n");
            return 1;
        
        }
        strcpy(setValue, "0"); // Set value low
        fwrite(&setValue, sizeof(char), 1, myOutputHandle);
        fclose(myOutputHandle);
        sleep(0.5); // wait for 1 sec
        
        GPIOPin=60;
        
        sprintf(GPIOString, "%d", GPIOPin);
        sprintf(GPIOValue, "/sys/class/gpio/gpio%d/value", GPIOPin);
        sprintf(GPIODirection, "/sys/class/gpio/gpio%d/direction", GPIOPin);
        
        if ((myOutputHandle = fopen("/sys/class/gpio/export", "ab")) == NULL){
        printf("Unable to export GPIO pin\n");
        return 1;
        }
       
        strcpy(setValue, GPIOString);
        fwrite(&setValue, sizeof(char), 2, myOutputHandle);
        fclose(myOutputHandle);
       
        // Set output to high
        if ((myOutputHandle = fopen(GPIOValue, "rb+")) == NULL){
            printf("Unable to open value handle\n");
            return 1;
        }
        
        strcpy(setValue, "1"); // Set value high
        fwrite(&setValue, sizeof(char), 1, myOutputHandle);
        fclose(myOutputHandle);
        sleep(0.5); // wait for 1 sec
      }
      
                    printf("potencia solar: %d \n",value);
                    printf("corriente del motor %d \n",value2);
           
    }

    return 0;
}
