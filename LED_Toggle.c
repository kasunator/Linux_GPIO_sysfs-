#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#define AC_AUTO_LED_PATH  "/sys/class/leds/ac_auto/brightness"

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
