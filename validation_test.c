#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "gpio.h"
#include<stdint.h>
#include<time.h>
#include<errno.h>

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
	0x0,//0x4, edited for testing this is UART
	0x0,//0x8, edited for testing this is UART
	0x10,
	0x20,
	0x40};

unsigned int   gpio_bank_D_Input_netlist = {
	0x10000,
	0x8000,
	0x0,//0x4000, edited for testing this is UART
	0x0,//0x2000, edited for testing this is UART
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

static int init_all_pins();
static int _init_pins(enum GPIO_bank bank ,
	unsigned int netlist[],int list_len, enum enum_pin_direction direction);

static int _set_Output_net(enum GPIO_bank bank ,
		unsigned int out_net, int pin_level);

static int _get_input_net(enum GPIO_bank bank,
		unsigned int in_net, unsigned int* in_net_reading);



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
/* this function is used to set a network of outputs belonging to a particular bank*/
static int _set_Output_net(enum GPIO_bank bank ,
	unsigned int out_net, int pin_level  ){

	int i =0, pin_number, ret =0;
	/* the out_net  is a 32 bit bit field  we have to
	check which bits are set enabled in the net
	by iterating through each bit */
	for (i = 0 ; i < 32 ; i++) {

		if ( out_net>>i & 0x01 ) {
			pin_number = i+(bank*32); /*the pin number is a value between 0 and 127*/
			ret = write_GPIO_output_state(pin_num, pin_level );
			if (ret<0) {
				printf("Error Setting out net\n");
				return -1;
			} else {
				return 0;
			}
		}
	}

}

static int _get_input_net(enum GPIO_bank bank,
	unsigned int in_net, unsigned int* in_net_reading  ){

	int i , pint_number,reading;
	/* the in_net  is a 32 bit bit field  we have to
	check which bits are set enabled in the net
	by iterating through each bit */
	for (i = 0 ; i < 32 ; i++) {

		if ( in_net>>i & 0x01 ) {
			pin_number = i+(bank*32); /*the pin number is a value between 0 and 127*/
			ret = read_GPIO_state(pin_num, &reading );
			if (ret < 0 ) {
				printf("Error Setting out net\n");
				return -1;
			} else {
				*in_net_reading |= (unsigned int)(*reading )<<i;
			}
		}
	}

       return *in_net_reading;

}

/*msec should be given in milliseconds */
static int _msleep(long msec)
{
    struct timespec ts;
    int res;

    if (msec < 0)
    {
        errno = EINVAL;
        return -1;
    }

    ts.tv_sec = msec / 1000;
    ts.tv_nsec = (msec % 1000) * 1000000;

    do {
        res = nanosleep(&ts, &ts);
    } while (res && errno == EINTR);

    return res;
}





enum gpio_net_test_state {
	TEST_STANDBY,
	SET_OUT_NET_HIGH=0,
	WAIT_FOR_HIGH_SETTLING_TIME,
	READ_IN_NET_HIGH,
	SET_OUT_NET_LOW,
	WAIT_FOR_LOW_SETTLING_TIME,
	READ_IN_NET_LOW,
	END_TEST
}

static enum gpio_net_test_state test_state= TEST_STANDBY;
static unsigned int test_index;
static unsigned int *out_net, *in_net;
static GPIO_bank bank_under_test;

typedef struct {
	uint8_t  start_test:1;
	uint8_t  state_entering:1;
	uint8_t  test_in_progress:1;
	uint8_t  test_complete:1;
}t_test_cntrl_flags;

static t_test_cntrl_flags test_cntrl_flags = {
	.start_test =0;
	.state_entering =0;
	.test_in_progress =0;
	.test_complete =0;
}
unsigned int input_high_reslt =0;
unsigned int input_low_reslt =0;
unsigned int test_result =0;

static int _set_test_params(enum GPIO_bank bank, unsigned int netlist_index) {

	bank_under_test = bank;
	test_index = netlist_index;
}

static int _start_test(){
	test_cntrl_flags.start_test =1;
}

static int _check_if_test_complet(){
	return test_cntrl_flags.test_complete;
}

static int _check_if_test_in_progress(){
	return test_cntrl_flags.test_complete;
}

//static int _test_gpio_net_task(enum GPIO_bank bank, unsigned int netlist_index) {
static int _test_gpio_net_task() {

	switch(test_state){
	case TEST_STANDBY:
		if ( test_cntrl_flags.start_test ==1 ) {
			test_cntrl_flags.start_test =0;
			test_cntrl_flags.test_complete =0;
			test_cntrl_flags.test_in_progress=1;
			input_high_reslt =0;
			input_low_reslt =0;
			test_result =0;
			if (bank_under_test == BANK_A){
				out_net = gpio_bank_A_Output_netlist[test_index] ;
				in_net = gpio_bank_A_Input_netlist[test_index];
			} else if (bank_under_test == BANK_B) {
				out_net = gpio_bank_A_Output_netlist[test_index] ;
				in_net = gpio_bank_A_Input_netlist[test_index];
			} else if (bank_under_test == BANK_C) {
				out_net = gpio_bank_A_Output_netlist[test_index] ;
				in_net = gpio_bank_A_Input_netlist[test_index];
			} else if (bank_under_test == BANK_D) {
				out_net = gpio_bank_A_Output_netlist[test_index] ;
				in_net = gpio_bank_A_Input_netlist[test_index];
			}
			test_cntrl_flags.state_entering = 1;
			test_state = SET_OUT_NET_HIGH;
		}
		break;
	case SET_OUT_NET_HIGH:
		if(test_cntrl_flags.state_entering ==1) {
			test_cntrl_flags.state_entering = 0;
			/*static int _set_Output_net(enum GPIO_bank bank ,
				unsigned int out_net, int pin_level  )*/
			_set_Output_net(bank_under_test, out_net, 1);
			test_cntrl_flags.state_entering = 1;
			test_state = WAIT_FOR_HIGH_SETTLING_TIME;
		}

		break;
	case WAIT_FOR_HIGH_SETTLING_TIME:
		if (test_cntrl_flags.state_entering == 1) {
			test_cntrl_flags.state_entering =0;
			_msleep(100); // go to sleep for 100milli seconds
			test_cntrl_flags.state_entering=1;
			test_state = READ_IN_NET_HIGH;
		}
		break;
	case READ_IN_NET_HIGH:
		if(test_cntrl_flags.state_entering == 1){
			test_cntrl_flags.state_entering =0;
		 	_get_input_net(bank_under_test,
					in_net, &input_high_reslt);
			test_cntrl_flags.state_entering =1;
			test_state = SET_OUT_NET_LOW;
		}
		break;
	case SET_OUT_NET_LOW:
		if(test_cntrl_flags.state_entering == 1) {
			test_cntrl_flags.state_entering =0;
			_set_Output_net(bank_under_test, out_net, 0);
			test_cntrl_flags.state_entering = 1;
			test_state = WAIT_FOR_LOW_SETTLING_TIME;
		}
		break;
	case WAIT_FOR_LOW_SETTLING_TIME;
		if (test_cntrl_flags.state_entering == 1) {
			test_cntrl_flags.state_entering =0;
			_msleep(100); // go to sleep for 100milli seconds
			test_cntrl_flags.state_entering=1;
			test_state = READ_IN_NET_LOW;
		}
		break;
	case READ_IN_NET_LOW:
		if(test_cntrl_flags.state_entering == 1){
			test_cntrl_flags.state_entering =0;
			_get_input_net(bank_under_test,
					in_net, &input_low_reslt);
			test_cntrl_flags.state_entering =1;
			test_state = END_TEST;
		}
		break;
	case END_TEST:
		if(test_cntrl_flags.state_entering ==1 ){
			test_cntrl_flags.state_entering=0;
			if (input_high_reslt == in_net && input_low_reslt == 0 ) {
				test_result=0;
			} else {
				test_result=-1;
			}
			test_cntrl_flags.test_in_progres=0;
			test_cntrl_flags.test_complete = 1;
			test_state= TEST_STANDBY;
		}
		break;
	default:
		breaak;
	}



}

static int test_gpios(uinsinged int test_start_index, unsigned int test_end_index){
	//check if everything is ready
}


static int init_all_pins(){
	//init bank A inputs
	_init_pins(BANK_A, gpio_bank_A_Input_netlist, sizeof(gpio_bank_A_Input_netlist), DIRECTION_INPUT );
	//init bank A outputs
	_init_pins(BANK_A, gpio_bank_A_Output_netlist, sizeof(gpio_bank_A_Output_netlist), DIRECTION_OUTPUT );

//#if 0 //we test port A only for now
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
//#endif //#if 0
}

static int set_all_outputs_high() {

}


/*
    int main(int argc, char *argv[]) {  }
    argc (ARGument Count) is int and stores number of command-line arguments passed by the user including the name of the program.
		So if we pass a value to a program, value of argc would be 2 (one for argument and one for program name)
    The value of argc should be non negative.
    argv(ARGument Vector) is array of character pointers listing all the arguments.
    If argc is greater than zero,the array elements from argv[0] to argv[argc-1] will contain pointers to strings.
    Argv[0] is the name of the program , After that till argv[argc-1] every element is command -line arguments.

		we can call the ./executable_name arg1 arg2 arg3

*/


int main(int argc, char *argv[]) {

	if ( argc  > 0) {
		if( strncmp(argv[1],"inti",4) == 0 ) {
			printf("arg[1] :init \n");
			init_all_pins();
		} else if (strncmp(argv[1],"high",4) == 0 ) {
			printf("arg[1] :high \n");

		} else if (strncmp(argv[1],"low",4) == 0 ) {
			printf("arg[1] :low \n");

		} else if (strncmp(argv[1],"read",4) == 0 ) {
			printf("arg[1] :read \n");

		} else  {
			printf("arg[1] :unknown command\n");

		}
	}
}
