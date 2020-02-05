#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "gpio.h"


unsigned int gpio_bank_A_Output_netlist[15] = {
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

unsigned int gpio_bank_A_Input_netlist[15] = {
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

unsigned int  gpio_bank_B_Output_netlist = {
	0x1,
	0x2,
	0x4,
	0x8,
	0x10,
	0x800,
	0x1000,
	0x2000,
	0x4000,
	0x8000,
	0x10000,
	0x20000,
	0x40000};

unsigned int  gpio_bank_B_Input_netlist = {
	0x80000000,
	0x40000000,
	0x20000000,
	0x10000000,
	0x8000000,
	0x4000000,
	0x2000000,
	0x1000000,
	0x800000,
	0x400000,
	0x200000,
	0x100000,
	0x80000};

unsigned int  gpio_bank_C_Output_netlist = {
	0x1,
	0x2,
	0x4,
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
	0x8000};

unsigned int  gpio_bank_C_Input_netlist = {
	0x80000000,
	0x40000000,
	0x20000000,
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

unsigned int  gpio_bank_D_Output_netlist = {
	0x1,
	0x2,
	0x4,
	0x8,
	0x10,
	0x20,
	0x40};

unsigned int   gpio_bank_D_Input_netlist = {
	0x10000,
	0x8000,
	0x4000,
	0x2000,
	0x1000,
	0x100,
	0x80};

enum enum_pin_direction{
	DIRECTION_INPUT=0,
	DIRECTION_OUTPUT
	};
enum GPIO_bank{
	BANK_A =0,
	BANK_B,
	BANK_C,
	BANL_D
};

static int _init_pins(enum GPIO_bank bank ,
	unsigned int netlist[],int list_len, enum enum_pin_direction direction  ){

	int i =0, j =0,func_ret =0, pin_number;


	for ( i =0 ; i < list_len ; i++ ) {
		//export netlist
		for( j =0 ; j < 32 ; j++) {
			if ( (netlist[i]>>j) & 0x01 ) {//check it the jth bit is set
				pin_number = j+(bank*32);
				func_ret = exportPin(pin_number);
				if ( func_ret > 0) {
					if (direction == DIRECTION_INPUT)
						func_ret = set_GPIO_as_input(pin_number);
				 	else if (direction == DIRECTION_OUTPUT)
						func_ret = set_GPIO_as_output(pin_number);
					if ( func_ret < 0 )
						printf("error setting setting output pin as output")
						return -1;

				} else {
					printf("error exporting output pin");
					return -1;
				}
			}
		}
	}

}

static int init_all_pins(){
	//init bank A inputs
	_init_pins(BANK_A, gpio_bank_A_Input_netlist, sizeof(gpio_bank_A_Input_netlist), DIRECTION_INPUT );
	//init bank A outputs
	_init_pins(BANK_A, gpio_bank_A_Output_netlist, sizeof(gpio_bank_A_Output_netlist), DIRECTION_OUTPUT );

	//init bank B inputs
	_init_pins(BANK_B, gpio_bank_B_Input_netlist, sizeof(gpio_bank_B_Input_netlist), DIRECTION_INPUT );
	//init bank B outputs
	_init_pins(BANK_B, gpio_bank_B_Output_netlist, sizeof(gpio_bank_B_Output_netlist), DIRECTION_OUTPUT );

	//init bank C inputs
	_init_pins(BANK_C, gpio_bank_C_Input_netlist, sizeof(gpio_bank_C_Input_netlist), DIRECTION_INPUT );
	//init bank C outputs
	_init_pins(BANK_C, gpio_bank_C_Output_netlist, sizeof(gpio_bank_C_Output_netlist), DIRECTION_OUTPUT );

	//init bank D inputs
	_init_pins(BANK_D, gpio_bank_D_Input_netlist, sizeof(gpio_bank_D_Input_netlist), DIRECTION_INPUT );
	//init bank D outputs
	_init_pins(BANK_D, gpio_bank_D_Output_netlist, sizeof(gpio_bank_D_Output_netlist), DIRECTION_OUTPUT );

}




/*

int static _init_gpio_A_pins(){

 	int outPutListLenght = sizeof(gpio_bank_A_Output_netlit) ;
 	int inPutListLength = sizeof(gpio_bank_A_Input_netlist);
 	int i =0, j =0, bit_count =0,func_ret =0, pin_number;

		//first configure the output pins
	for ( i =0 ; i < outPutListLength ; i++ ) {
		//export the outputs
		for( j =0 ; j < 32 ; j++) {
			if ( (gpio_bank_A_Output_netlist[i]>>j) & 0x01 ) {//check it the jth bit is set
				pin_number = j;
				func_ret = exportPin(pin_number);
				if ( func_ret > 0) {
					func_ret = set_GPIO_as_output(pin_number);
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
*/



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
