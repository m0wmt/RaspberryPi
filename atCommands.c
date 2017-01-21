/* 
 * Test of sending AT commands to USB GSM Modem at ttyACM0 using 
 * wiringPi
 */
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <time.h>

#include <wiringPi.h>
#include <wiringSerial.h>


void waitFor (unsigned int secs) {
    unsigned int retTime = time(0) + secs;   // Get finishing time.
    while (time(0) < retTime);               // Loop until it arrives.
}

#define OK       0
#define NO_INPUT 1
#define TOO_LONG 2
static int getLine (char *prmpt, char *buff, size_t sz) {
    int ch, extra;

    // Get line with buffer overrun protection.
    if (prmpt != NULL) {
        printf ("%s", prmpt);
        fflush (stdout);
    }
    if (fgets (buff, sz, stdin) == NULL)
        return NO_INPUT;

    // If it was too long, there'll be no newline. In that case, we flush
    // to end of line so that excess doesn't affect the next call.
    if (buff[strlen(buff)-1] != '\n') {
        extra = 0;
        while (((ch = getchar()) != '\n') && (ch != EOF))
            extra = 1;
        return (extra == 1) ? TOO_LONG : OK;
    }

    // Otherwise remove newline, and give string back to caller.
    buff[strlen(buff) -1] = '\0';
    return OK;
}
int main ()
{
	int fd;
	int rc;
	int dataAvailable = 0;

	char character;
    char buff[150];
 
	if ((fd = serialOpen ("/dev/ttyACM0", 460800)) < 0) {
		fprintf (stderr, "Unable to open serial device: %s\n", strerror (errno));
		return 1 ;
	}

	if (wiringPiSetup () == -1) {
		fprintf (stdout, "Unable to start wiringPi: %s\n", strerror (errno));
		return 1 ;
	}

	while (1) {
		// Read in from the command line, no input and exit
		rc = getLine ("Enter AT Command > ", buff, sizeof(buff));
		if ((strlen(buff) == 0) || (rc == NO_INPUT) ) {
			printf("No input, exiting...\n");
			break;
		}
		else if (rc == TOO_LONG) {
			printf ("Input too long [%s], exiting...\n", buff);
			break;
		}
		else {
			printf ("OK [%s] %i\n", buff, strlen(buff));

			// Add '\r' at the end and terminate buff
			buff[strlen(buff)] = '\r';
			buff[strlen(buff) + 1] = '\0';

			serialPrintf(fd, buff);
			
			// Wait for a second
			waitFor(1);
			
			dataAvailable = serialDataAvail(fd);
			//printf("Data available: %i\n", dataAvailable);
			while (dataAvailable != 0) {
				character = (char)serialGetchar(fd);
				printf("%c", character);
				dataAvailable = serialDataAvail(fd);
			}
			printf("\n");
		}
	}

	printf("Program finished...\n");
	serialClose(fd);	
	return 0 ;
}
