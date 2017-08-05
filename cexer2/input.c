#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <termios.h>

static struct termios stored_settings;

void set_keypress(void) {
	struct termios new_settings;
	tcgetattr(0,&stored_settings);
	new_settings = stored_settings;
	new_settings.c_lflag &= (~ICANON & ~ECHO);
	new_settings.c_cc[VTIME] = 0;
	new_settings.c_cc[VMIN] = 1;
	tcsetattr(0,TCSANOW,&new_settings);
	return;
}

void reset_keypress(void) {
	tcsetattr(0,TCSANOW,&stored_settings);
	return;
}

int main(void) {
	fd_set rfds;
	struct timeval tv;
	int retval;
	
	set_keypress();

    printf("[Start]\n");
	while(1) {
		FD_ZERO(&rfds);
		FD_SET(0, &rfds);
		
		tv.tv_sec = 0;
		tv.tv_usec = 0;	
		retval = select(2, &rfds, NULL, NULL, &tv);
		if (retval) {
			//printf("Data is available now.\n");
			char c = getc(stdin);
            printf("[%c]", c);
            fflush(0);
		} else {
		//	printf("No data available.\n");
		}
		usleep(100000);
	}
	reset_keypress();
	exit(0);
}
