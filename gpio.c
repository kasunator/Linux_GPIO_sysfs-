#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#define DEBUG
#define AC_AUTO_LED_PATH  "/sys/class/leds/ac_auto/brightness"
#define GPIO_PIN_EXPORT_PATH	"/sys/class/gpio/export"
#define GPIO_PIN_UNEXPORT_PATH "/sys/class/gpio/export/unexport"
#define GPIO_PIN_PATH "/sys/class/gpio/"

#define GPIO_PIN_BANK_A_SUB_STRING "/sys/class/gpio/PA"
#define GPIO_PIN_BANK_B_SUB_STRING "/sys/class/gpio/PB"
#define GPIO_PIN_BANK_C_SUB_STRING "/sys/class/gpio/PC"
#define GPIO_PIN_BANK_D_SUB_STRING "/sys/class/gpio/PD"

static int exportPin(int pin_num);
static int unexportPin(int pin_num);
static int set_GPIO_as_output(int pin_num);
static int set_GPIO_as_input(int pin_num);
static int read_GPIO_state(int pin_num);
static int write_GPIO_output_state(int pin_num, int value);



#if 0
int main() {
	FILE* fp = NULL;
        char state[5] = "";
        char* ret;
	char write_value;
	fp = fopen(AC_AUTO_LED_PATH, "rt");
        if (fp == NULL ) {
		printf("No such file\n");
	} else {
		/* this function reads 5 characters,just a random long enough number,
		 read the length or till new line or endof file is met, what ever comes first */
		ret = fgets(state,5,fp); 
		if (ret == NULL) {
                	printf("the read failed\n");		
		} else {
		 	printf("The file string %s \n", state );
                        printf("read length %d \n", strlen(state) ); /*this return 2*/
			fp = freopen(AC_AUTO_LED_PATH, "w+",fp);
			if ( fp != NULL) {//to make sure we didnt have a problem re opening the same file
				if ( state[0] == '0' ) { //if the current state is off
					//char write_value ='1';
					write_value='1';
				} else {
					//char write_value ='0';
					write_value='0';
				}
				fprintf(fp,"%c\n", write_value);
				fclose(fp);

			}

		}
	}
	return 0;
}
#endif 

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

static int set_GPIO_as_output(int pin_num) {
	FILE* fp = NULL;
	char gpio_pin_file_string[strlen(GPIO_PIN_BANK_A_SUB_STRING)  + strlen("/direction")+2 ] ;
 
      	if (pin_num < 32) {
  		sprintf(gpio_pin_file_string,GPIO_PIN_BANK_A_SUB_STRING "%d/direction",pin_num);
	} else if(pin_num < 64) {
		sprintf(gpio_pin_file_string,GPIO_PIN_BANK_B_SUB_STRING "%d/direction",pin_num);
	} else if(pin_num < 98) {
		sprintf(gpio_pin_file_string,GPIO_PIN_BANK_C_SUB_STRING "%d/direction",pin_num);
	} else if(pin_num < 128) {
		sprintf(gpio_pin_file_string,GPIO_PIN_BANK_D_SUB_STRING "%d/direction",pin_num);
	}
#ifdef DEBUG
	printf("DEBUG gpio_pin_file_string:[%s]\n",gpio_pin_file_string);

#endif
	fp = fopen(gpio_pin_file_string,"w");
        if (fp != NULL) {
		fprintf(fp,"out");
		return 0;
	} else {
		printf("faile openning file:[%s]\n",gpio_pin_file_string);
		return -1;
	}
}

static int set_GPIO_as_input(int pin_num) {
	FILE* fp = NULL;
	char gpio_pin_file_string[strlen(GPIO_PIN_BANK_A_SUB_STRING) + strlen("/direction")+2 ] ;
 
      	if (pin_num < 32) {
  		sprintf(gpio_pin_file_string,GPIO_PIN_BANK_A_SUB_STRING "%d/direction",pin_num);
	} else if(pin_num < 64) {
		sprintf(gpio_pin_file_string,GPIO_PIN_BANK_B_SUB_STRING "%d/direction",pin_num);
	} else if(pin_num < 98) {
		sprintf(gpio_pin_file_string,GPIO_PIN_BANK_C_SUB_STRING "%d/direction",pin_num);
	} else if(pin_num < 128) {
		sprintf(gpio_pin_file_string,GPIO_PIN_BANK_D_SUB_STRING "%d/direction",pin_num);
	}
#ifdef DEBUG
	printf("DEBUG gpio_pin_file_string:[%s]\n",gpio_pin_file_string);

#endif	
	fp = fopen(gpio_pin_file_string,"w");
        if (fp != NULL) {
		fprintf(fp,"in");
		return 0;
	} else {
		printf("faile openning file:[%s]\n",gpio_pin_file_string);
		return -1;
	}
}

static int read_GPIO_state(int pin_num) {
	FILE* fp = NULL;
	char state[2] = {0};
	char gpio_pin_file_string[strlen(GPIO_PIN_BANK_A_SUB_STRING)+ strlen("/value") +2] ;
        char* ret;
      	if (pin_num < 32) {
  		sprintf(gpio_pin_file_string,GPIO_PIN_BANK_A_SUB_STRING "%d/value",pin_num);
	} else if (pin_num < 64) {
		sprintf(gpio_pin_file_string,GPIO_PIN_BANK_B_SUB_STRING "%d/value",pin_num);
	} else if (pin_num < 98) {
		sprintf(gpio_pin_file_string,GPIO_PIN_BANK_C_SUB_STRING "%d/value",pin_num);
	} else if (pin_num < 128) {
		sprintf(gpio_pin_file_string,GPIO_PIN_BANK_D_SUB_STRING "%d/value",pin_num);
	}
#ifdef DEBUG
	printf("DEBUG gpio_pin_file_string :[%s]\n",gpio_pin_file_string);

#endif	

	fp = fopen(gpio_pin_file_string,"r");
        if (fp != NULL) {
		ret = fgets(state,2,fp);
                if (ret != NULL ) {
			return state[0];      
		} else { 
			return -1;
		}
	} else {
		printf("faile openning file:[%s]\n",gpio_pin_file_string);
		return -1;
	}

}

static int write_GPIO_output_state(int pin_num, int value){
	FILE* fp = NULL;
	char state[2] = {0};
	char gpio_pin_file_string[strlen(GPIO_PIN_BANK_A_SUB_STRING)+ strlen("/value") +2];
        char write_value;
	int ret;
      	if (pin_num < 32) {
  		sprintf(gpio_pin_file_string,GPIO_PIN_BANK_A_SUB_STRING "%d/value",pin_num);
	} else if (pin_num < 64) {
		sprintf(gpio_pin_file_string,GPIO_PIN_BANK_B_SUB_STRING "%d/value",pin_num);
	} else if (pin_num < 98) {
		sprintf(gpio_pin_file_string,GPIO_PIN_BANK_C_SUB_STRING "%d/value",pin_num);
	} else if (pin_num < 128) {
		sprintf(gpio_pin_file_string,GPIO_PIN_BANK_D_SUB_STRING "%d/value",pin_num);
	}
#ifdef DEBUG
	printf("DEBUG gpio_pin_file_string :[%s]\n",gpio_pin_file_string);

#endif	

	fp = fopen(gpio_pin_file_string,"w");
        if (fp != NULL) {
		if(value ==0 )
			write_value = '0';
		else
			write_value = '1';	
		ret = fprintf(fp,"%c\n",write_value);
		return ret;
	} else {
		printf("faile openning file:[%s]\n",gpio_pin_file_string);
		return -1;
	}
}


static int exportPin(int pin_num) {
	FILE* fp = NULL;
	
         
        fp = fopen(GPIO_PIN_EXPORT_PATH,"w");
	if ( fp != NULL) {//to make sure we didnt have a problem re opening the same file
		fprintf(fp,"%d\n", pin_num);
		fclose(fp);
		return 0;
	} else {
		printf("fopen failed\n");
		return -1;
	}
}

static int unexportPin(int pin_num) {
	FILE* fp = NULL;
	
         
        fp = fopen(GPIO_PIN_UNEXPORT_PATH,"w");
	if ( fp != NULL) {//to make sure we didnt have a problem re opening the same file
		fprintf(fp,"%d\n", pin_num);
		fclose(fp);
		return 0;
	} else {
		printf("fopen failed\n");
		return -1;
	}
}


