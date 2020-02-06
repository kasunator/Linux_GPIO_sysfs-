#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#define DEBUG
#define AC_AUTO_LED_PATH  "/sys/class/leds/ac_auto/brightness"
#define GPIO_PIN_EXPORT_PATH	"/sys/class/gpio/export"
#define GPIO_PIN_UNEXPORT_PATH "/sys/class/gpio/export/unexport"
#define GPIO_PIN_PATH "/sys/class/gpio/"

#define GPIO_PIN_BANK_A_SUB_STRING "/sys/class/gpio/PA"
#define GPIO_PIN_BANK_B_SUB_STRING "/sys/class/gpio/PB"
#define GPIO_PIN_BANK_C_SUB_STRING "/sys/class/gpio/PC"
#define GPIO_PIN_BANK_D_SUB_STRING "/sys/class/gpio/PD"

 int exportPin(int pin_num);
 int unexportPin(int pin_num);
 int set_GPIO_as_output(int pin_num);
 int set_GPIO_as_input(int pin_num);
 int read_GPIO_state(int pin_num);
 int write_GPIO_output_state(int pin_num, int value);

 void print_error(char func_name[], char failed_file_op[],
 	char failed_file_location[], char file_op_ret_val[],char failed_wrt_val[]);


#ifdef TEST_ERROR_PRINT_FUNCTION
/* test the debug print function */
#define TEST_FUNCTION_NAME "test_exportPin"
#define TEST_FAILED_FILE_OP "fopen"
#define TEST_FAILED_FILE_LOCATION "/sys/class/gpio/export/unexport"
#define TEST_FAILED_OP_RET_VAL   "-1"
#define TEST_FAILED_WRT_VAL  "7"
int main() {
	print_error(TEST_FUNCTION_NAME, TEST_FAILED_FILE_OP, TEST_FAILED_FILE_LOCATION,
  		TEST_FAILED_OP_RET_VAL, TEST_FAILED_WRT_VAL );
}
#endif //#ifdef TEST_ERROR_PRINT_FUNCTION

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

#if 0
int main() {
  	int ret;
	printf("exporting pin\n");
	exportPin(31);
	set_GPIO_as_output(31);//this will aotmatically reset the gpio ouput state to 0
	sleep(1);
  	ret =read_GPIO_state(31);
	printf("ret was: %d\n", ret);
  	if (ret == 1 ) {//so this condition will never be set
    		write_GPIO_output_state(31,0);
	} else {
		write_GPIO_output_state(31,1);
	}
}
#endif
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
#if 0
int main(int argc, char *argv[]) {

	if ( argc  > 0) {
		if( strncmp(argv[1],"write",5) == 0 ) {
			printf("command 1 :write \n");
			if( strncmp(argv[2],"1",1) == 0) {
				printf("parm 1: 1\n");
				write_GPIO_output_state(31,1);
			} else if ( strncmp(argv[2],"1",0) == 0 ) {
				printf("parm 1: 0\n");
				write_GPIO_output_state(31,0);
			}
		} else if ( strncmp(argv[1],"read",4) == 0) {
			printf("command 1 :read \n");
			read_GPIO_state(31);
		} else if ( strncmp(argv[1],"toggle",6) == 0 ) {
			printf("command 1 :toggle \n");
			int ret =read_GPIO_state(31);
			printf("read_GPIO_state returned : %d",ret);
			if (ret == 1 ) {
				printf("ret was 1 ");
				write_GPIO_output_state(31,0);
			} else {
				printf("ret was 0\n");
				write_GPIO_output_state(31,1);
			}
		}
	}
}
#endif

int set_GPIO_as_output(int pin_num) {
	FILE* fp = NULL;
	int ret_value;
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
		ret_value = fprintf(fp,"out"); //On success, the total number of characters written is returned.
#ifdef DEBUG
		printf("ret_value: %d\n",ret_value);
#endif //#ifdef DEBUG
		if ( ret_value > 0 ) {
			fclose(fp);
			return 0;
		} else {
			/*printf("error writing to file\n");*/
			print_error("set_GPIO_as_output","fprintf",
					gpio_pin_file_string, ret_value,0);
			fclose(fp);
			return -1;
		}
	} else {
		/*printf("error faile openning file:[%s]\n",gpio_pin_file_string);*/
		print_error("set_GPIO_as_output","fopen",
				gpio_pin_file_string, 0,0);
		return -1;
	}
}

int set_GPIO_as_input(int pin_num) {
	FILE* fp = NULL;
	int ret_value;
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
		ret_value = fprintf(fp,"in");
		if ( ret_value < 0) {
			fclose(fp);
                        return 0;
		} else {
			/*printf("error writing to file\n");*/
			print_error("set_GPIO_as_input","fprintf",
					gpio_pin_file_string, ret_value,0);
			fclose(fp);
			return -1;
		}

	} else {
		/*printf("faile openning file:[%s]\n",gpio_pin_file_string);*/
		print_error("set_GPIO_as_input","fopen",
				gpio_pin_file_string, 0,0);
		return -1;
	}
}

int read_GPIO_state(int pin_num, int* read_value) {
	FILE* fp = NULL;
	char state[3] = {0};
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
		ret = fgets(state,3,fp);
#ifdef DEBUG
		printf("read state: %s \n",state);
#endif //#ifdef DEBUG
    		if (ret != NULL ) {
			fclose(fp);
			if(state[0] == '1') {
                                *read_value =1;
				return 1;
			} else if ( state[0] == '0') {
                                *read_value =0;
				return 0;
			} else {
                                *read_value =state[0];
				return state[0];
			}
		} else {
			/*printf("Error reading from file\n");*/
			fclose(fp);
			print_error("read_GPIO_state","fgets",
					gpio_pin_file_string, ret,0);
			return -1;
		}
	} else {
		/*printf("faile openning file:[%s]\n",gpio_pin_file_string);*/
		print_error("read_GPIO_state","fopen",
				gpio_pin_file_string, 0,0);
		return -1;
	}

}

int write_GPIO_output_state(int pin_num, int value){
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

	fp = fopen(gpio_pin_file_string,"w+");
  	if (fp != NULL) {
		if(value ==0 )
			write_value = '0';
		else
			write_value = '1';
		//printf("write value = %c \n", write_value);
		ret = fprintf(fp,"%c\n",write_value);
		if ( ret < 0 ) {
			//printf("Error failed writing: [%c] \n", write_value );
			print_error("write_GPIO_output_state","fprintf",
					gpio_pin_file_string, ret,write_value);
			fclsoe(fp);
			return -1;
		} else {
			fclose(fp);
			return ret;
		}
	} else {
		/*printf("Error failed openning file:[%s]\n",gpio_pin_file_string);*/
		print_error("write_GPIO_output_state","fopen",
				gpio_pin_file_string, 0,0);
		return -1;
	}
}


int exportPin(int pin_num) {
	FILE* fp = NULL;
	int ret;
        fp = fopen(GPIO_PIN_EXPORT_PATH,"w");
	if ( fp != NULL) {//to make sure we didnt have a problem re opening the same file
		ret = fprintf(fp,"%d\n", pin_num);
		if ( ret < 0 ){
			print_error("unexportPin","fprintf",
					GPIO_PIN_EXPORT_PATH, ret,pin_num);
			fclose(fp);
			return -1;
		} else {
			fclose(fp);
			return 0;
		}

	} else {
		/*printf("Error file open failed\n");*/
		print_error("exportPin","fopen",
				GPIO_PIN_EXPORT_PATH, 0,0);
		return -1;
	}
}


int unexportPin(int pin_num) {
	FILE* fp = NULL;
	int ret;

  	fp = fopen(GPIO_PIN_UNEXPORT_PATH,"w");
	if ( fp != NULL) {//to make sure we didnt have a problem re opening the same file
		ret = fprintf(fp,"%d\n", pin_num);
		if ( ret < 0 ) {
			/*print_error(char func_name[], char failed_file_op[],
				char failed_file_location[], char file_op_ret_val[],char failed_wrt_val[])*/
			print_error("unexportPin","fprintf",
					GPIO_PIN_UNEXPORT_PATH, ret,pin_num);
			fclose(fp);
			return -1;
		} else {
			fclose(fp);
			return ret;
		}

	} else {
		/*printf("Error file open failed\n");*/
		print_error("unexportPin","fopen,
				GPIO_PIN_UNEXPORT_PATH, 0,0);
		return -1;
	}
}

/* error reporting JSON structure
""Error" : { 	"function name": "(string)my_function name",
		"failed file operation": " (string)my_file operation function",
		"failed file location": "(string)my_failed file location",
		"failed file operation return value": "(string)my_file oepration return value";
                "failed write value" : "(string) XX"
	   }

*/
void print_error(char func_name[], char failed_file_op[],
	char failed_file_location[], int file_op_ret_val,int failed_wrt_val){
	//char final_string[] ={};
        //strncat(final_string," \"",)
	char* _str_ret[11];
	sprintf(_str_ret,"%d",file_op_ret_val);

	char* _str_wrt[11];
	sprintf(_str_wrt,"%d",failed_wrt_val);

	printf( " \"Error\" : \"function name\" : \"%s\" , \"failed file operation\" : \"%s\" , \"failed file location\" : \"%s\" , \"failed file operation return value\" : \"%s\", \"failed write value\" : \"%s\" \n" ,
	func_name, failed_file_op, failed_file_location , _str_ret, _str_wrt);
	/*printf( " \"function name\"\: \"%s\" ,\"failed file operation\"\: \"%s\"
	        ,\"failed file location\"\: \"%s\", \"failed file operation return value\"\: \"%s\", \"failed write value\"\: \"%s\" "
		,func_name, failed_file_op , failed_file_location, file_op_ret_val, failed_wrt_val );*/



}
