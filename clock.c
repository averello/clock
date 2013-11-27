/*
 * clock - print time in various formats
 * Copyright (C) 2012 George Boumis <developer.george.boumis@gmail.com>
 *
 */

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef char*string;

extern char *optarg;
extern int optind, opterr, optopt;

static const char *const options = "tboxlh"; /* Possible options */
static const char *const formatoptions = "l"; /* Possible options */
static struct tm *loctime = NULL;
static time_t clocktime;
static const char *const licence = "clock - print time in various formats (12h)\nCopyright (C) 2012 George Boumis\nThis program comes with ABSOLUTELY NO WARRANTY;\nThis is free software, and you are welcome to redistribute it\nunder certain conditions;";


void convertto( const char option, const char format, const struct tm*localtime );


int main ( int argc, char *argv[] ) {
	int option = -1;
	int format = -1;
	if (argc>1)
		option = getopt(argc, argv, options);

	time(&clocktime); /* get time frim Epoch */
	loctime = localtime(&clocktime);
	loctime->tm_year += 1900; /* Needed because tm_year equals year - 1900 */
	loctime->tm_hour %= 12;

	format = getopt(argc, argv, formatoptions);
#ifdef DEBUG_FLAG
	printf("option1 : %d\noption2 : %d\n", option, format);
#endif
	if ( option == 'h' )
		puts(licence);
	else
		convertto(option, format, loctime);

	return EXIT_SUCCESS;
}


void convertto( const char option, const char format, const struct tm*localtime ) {
	register int i;
	switch (option) {
		default:
		case 'b': /* bonary */
			{
				/* Print hours */
				printf(" ");
				for (i=(1<<4); i>=1; i>>=1)
					printf("%d", ((localtime->tm_hour & i) == i) ? 1 : 0);
				if (format=='l' || option=='l') puts("");
				else printf(":");

				/* Print minutes */
				for (i=(1<<5); i>=1; i>>=1)
					printf("%d", ((localtime->tm_min & i) == i) ? 1 : 0);
				if (format=='l' || option=='l') puts("\n");
				else printf(":");

				/* Print seconds */
				for (i=(1<<5); i>=1; i>>=1)
					printf("%d", ((localtime->tm_sec& i) == i) ? 1 : 0);
				puts("");
				break; 
			}
		case 'x': /* hex */
			if (format=='l')
				printf("%x\n%2x\n%2x\n", localtime->tm_hour, localtime->tm_min, localtime->tm_sec);
			else
				printf("%x:%2x:%2x\n", localtime->tm_hour, localtime->tm_min, localtime->tm_sec);
			break;
		case 'o': /* octal */
			if (format=='l')
				printf("%2o\n%2o\n%2o\n", localtime->tm_hour, localtime->tm_min, localtime->tm_sec);
			else
				printf("%2o:%2o:%2o\n", localtime->tm_hour, localtime->tm_min, localtime->tm_sec);
			break;
		case 't': /* */
			printf("%d/%d/%d %d:%d:%d\n", localtime->tm_mday, localtime->tm_mon, localtime->tm_year, localtime->tm_hour, localtime->tm_min, localtime->tm_sec);
			break;
	}
}




