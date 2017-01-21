/* 
 * Test of configuring GPIO pins using wiringPi
 */
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <ctype.h>

#include <wiringPi.h>

#define MAX_STR_LEN (50)

int main ()
{
    char str[MAX_STR_LEN] = {0};
    int i, gpio, setting, mode;

    printf("Enter GPIO, mode (0=OUTPUT, 1=INPUT) and setting (0=LOW, 1=HIGH) > ");
    fgets(str, MAX_STR_LEN, stdin);

    for(i = 0; i < MAX_STR_LEN; i++)
    {
        if(!isdigit(str[i]) && (!isspace(str[i])) && (str[i] != '\0'))
        {
            if((i != 0) && (str[i - 1] != ' ') && ((str[i] != '+') || (str[i] != '-')))
            {
                printf("'%c' is bogus, bye!\n", str[i]);
                return -1;
            }
        }

        if((str[i] == '\n') || (str[i] == '\0'))
            break;
    }

    sscanf(str, "%d%d%d", &gpio, &setting, &mode);
    printf("Set GPIO %d (%s) to %s.\n", gpio, (setting == 0 ? "LOW" : "HIGH"), (mode == 0 ? "OUTPUT" : "INPUT")); 
    
	wiringPiSetup ();
	pinMode (gpio,  (mode == 0 ? OUTPUT : INPUT));
	{
		digitalWrite (0, (setting == 0 ? LOW : HIGH));
	}

	printf("Program finished...\n");
	return 0 ;
}

