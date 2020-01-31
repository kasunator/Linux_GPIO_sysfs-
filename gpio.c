#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#define AC_AUTO_LED_PATH  "/sys/class/leds/ac_auto/brightness"

int main() {
	FILE* fp = NULL;
        char state[5] = "";
        char* ret;
	fp = fopen(AC_AUTO_LED_PATH, "rt");
        if (fp == NULL ) {
		printf("No such file\n");
	} else {
		ret = fgets(state,5,fp);
		if (ret == NULL) {
                	printf("the read failed\n");		
		} else {
		 	printf("The file string %s \n", state );
                        printf("read length %d \n", strlen(state) );
 			printf("last ocuurance of 0 %d \n",strrchr(state,'0')-state);
			printf("last ocuurance of 1 %d \n",strrchr(state, '1')-state);
		}
	}
	return 0;
}
