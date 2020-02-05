#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "gpio.h"


int gpio_bank_A_Output_netlist[15] = {
					0x1,
					0x2,
					0x8,
					0x10,
					0x20,
					0x40,
					0x80,
					0x100,
					0x200,
					0x400,
					0x800,
					0x1000,
					0x2000,
					0x4000,
					0x8000 };

int gpio_bank_A_Input_netlist[15] = {
					0x80000000,
					0x40000004,
					0x10000000,
					0x8000000,
					0x4000000,
					0x2000000,
					0x1000000,
					0x800000,
					0x400000,
					0x200000,
					0x100000,
					0x80000,
					0x40000,
					0x20000,
					0x10000};


int static _init_gpio_A_pins(){
			 	int outPutListLenght = sizeof(gpio_bank_A_Output_netlit) ;
			 	int inPutListLength = sizeof(gpio_bank_A_Input_netlist);
			 	int i =0, j =0, bit_count =0;
				int func_ret =0,
					//first configure the output pins
				for ( i =0 ; i < outPutListLength ; i++ ) {
					//export the outputs
					for( j =0 ; j < 32 ; j++) {
						if ( ((gpio_bank_A_Output_netlist[i]>>j) & 0x01) ) {//check it the jth bit is set
							func_ret = exportPin(j);
							if ( func_ret > 0) {
								func_ret = set_GPIO_as_output(j);
								if ( func_ret < 0 ) {
									printf("error setting setting output pin as output")
								}
							} else {
								printf("error exporting output pin");
							}
						}
					}
				}
				//now configure the input pins
				for ( i =0 ; i < inPutListLength ; i++ ) {
					//export the outputs
					for( j =0 ; j < 32 ; j++) {
						if ( ((gpio_bank_A_Output_netlist[i]>>j) & 0x01) ) {//check it the jth bit is set
							func_ret = exportPin(j);
							if ( func_ret > 0) {
								func_ret = set_GPIO_as_input(j);
								if ( func_ret < 0 ) {
									printf("error setting setting output pin as output")
								}
							} else {
								printf("error exporting output pin");
							}
						}
					}
				}
}




int main() {
       int ret;
	printf("exporting pin\n");
	exportPin(31);
	set_GPIO_as_output(31);
        ret =read_GPIO_state(31);
        if (ret == 1 ) {
		printf("ret was 1 ");
          	write_GPIO_output_state(31,0);
	} else {
		printf("ret was 0 ");
		write_GPIO_output_state(31,1);
	}
}
