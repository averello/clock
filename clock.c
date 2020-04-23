/*
 * clock - print time in various formats
 *
 * The MIT License (MIT)

 * Copyright (c) 2012-2020 George Boumis <developer.george.boumis@gmail.com>

 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:

 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.

 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>

extern char *optarg;
extern int optind, opterr, optopt;

enum {
	OptionBinary = (1<<0), // shows in binary format
	OptionHex = (1<<1), // shows in hexadecimal format
	OptionOctal = (1<<2), // shows in octal format
	Option12h = (1<<3), // use 12h
	OptionLongOutput = (1<<4), // use ":" delimiter
};
typedef uint8_t OptionFlags;


void usage(const char *const options, const struct tm *const localtime);
void print_bits(unsigned integer, unsigned bits);
void convertto(const OptionFlags option, const struct tm *const localtime);

int main ( int argc, char *argv[] ) {
	const char *const options = "botxhfl"; /* Possible options */
	OptionFlags flags = Option12h;

	time_t clocktime;
	time(&clocktime); /* get time from Epoch */
	struct tm *loctime = NULL;
	loctime = localtime(&clocktime);
	loctime->tm_year += 1900; /* needed because tm_year equals year - 1900 */

	int option;
	while ((option = getopt(argc, argv, options)) != -1) {
		switch (option) {
			case 'h':
				usage(options, loctime);
				exit(EXIT_SUCCESS);
				break;
			case 'b':
				flags |= OptionBinary;
			case 'x':
				flags |= OptionHex;
				break;
			case 'o':
				flags |= OptionOctal;
				break;
			case 'f':
				flags &= ~Option12h;
				break;
			case 'l':
				flags |= OptionLongOutput;
				break;
			case 't':
				break;
			default:
				exit(EXIT_FAILURE);
		}
	}
	argc -= optind;
	argv += optind;

	if (flags & Option12h) {
		loctime->tm_hour %= 12;
	}

	convertto(flags, loctime);

	return EXIT_SUCCESS;
}

void usage(const char *const options, const struct tm *const localtime) {
	const char *const licence = 
		"clock - print time in various formats\n"
		"\nCopyright (C) 2012-%d Georges Boumis\n"
		"This program comes with ABSOLUTELY NO WARRANTY;\n"
		"This is free software, and you are welcome to redistribute it\n"
		"under certain conditions;";
	const char *const usage = 
		"Usage: clock [-%s]\n"
		"\t-h\tshows this message.\n"
		"\t-t\tdefault. show full date. d/m/yyyy h:m:s\n"
		"\t-f\tuse 24h instead of 12h. does not work with -b\n"
		"\t-b\tbinary clock. hhhhh:mmmmmm:ssssss\n"
		"\t-x\thexadecimal clock. hh:mm:ss\n"
		"\t-o\toctal clock. hh:mm:ss\n"
		"\t-l\tlong format. instead of \":\" use newlines.\n";
	printf(licence, localtime->tm_year);
	puts("\n");
	printf(usage, options);
}

void print_bits(unsigned integer, unsigned bits) {
	assert(bits > 0);
	register int i;
	for (i=(1<<(bits -1)); i>=1; i>>=1) {
		printf("%d", ((integer & i) == i) ? 1 : 0);
	}
}

void convertto(const OptionFlags option, const struct tm *const localtime ) {
	const char *const delimiter = option & OptionLongOutput 
		? "\n"
		: ":";
	if (option & OptionBinary) {
		/* Print hours */
		print_bits(localtime->tm_hour, 4);
		printf("%s", delimiter);

		/* Print minutes */
		print_bits(localtime->tm_min, 5);
		printf("%s", delimiter);

		/* Print seconds */
		print_bits(localtime->tm_sec, 5);
		puts("");
	} else if (option & OptionHex) {
		printf("%x%s%2x%s%2x\n", localtime->tm_hour, delimiter,
				localtime->tm_min, delimiter, localtime->tm_sec);
	} else if (option & OptionOctal) {
		printf("%2o%s%2o%s%2o\n", localtime->tm_hour, delimiter,
				localtime->tm_min, delimiter, localtime->tm_sec);
	} else {
		printf("%d/%d/%d %d:%d:%d\n", localtime->tm_mday,
				localtime->tm_mon, localtime->tm_year, localtime->tm_hour,
				localtime->tm_min, localtime->tm_sec);
	}
}

