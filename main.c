#include <stdio.h>
#include <stdbool.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

#include "qlichgll.h"
#include "ebcdic.h"
#include "error.h"

/* it needs to be freed, but we'll exec/exit anyways */
static char*
dup_to_libname (char *utf)
{
	char *utf_upper = strdup (utf);
	int utf_len = strlen (utf);
	/*
	 * first, convert the UTF-8 library name to upper case, as XPF needs
	 * library names that way. naive ASCII conversion because XPF won't
	 * need anything more either.
	 */
	for (int i = 0; i < utf_len; i++) {
		utf_upper [i] = (char) toupper (utf [i]);
	}
	char *newname = malloc (11);
	utf2ebcdic (utf_upper, 11, newname);
	/* make sure the last char is EBCDIC space */
	newname [10] = 0x40;
	free (utf_upper);
	return newname;
}

static void
usage (char *argv0)
{
	fprintf (stderr, "usage: %s [-CrRL] [-c curlib] [-p prodlib] [-P prodlib] [-l userlib] -- [argv]\n", argv0);
	exit (1);
}

int
main (int argc, char **argv)
{
	set_program_name (argv [0]);
	ERRC0100 err = { 0 };
	err.bytes_in = sizeof (err);
	char *prodlib1, *prodlib2, *curlib;
	char *userlibs [250]; /* just deal with the max value */
	int userlibs_len = -1;
	/* ...so copy it in instead; do the same later */
	prodlib1 = prodlib2 = curlib = dup_to_libname ("*SAME");
	/* getopt sludge */
	int ch;
	while ((ch = getopt (argc, argv, "c:Cp:P:rRl:L")) != -1) {
		switch (ch) {
		case 'C': /* set no library in current library entry */
			curlib = dup_to_libname ("*CRTDFT");
			break;
		case 'r': /* remove first product library */
			prodlib1 = dup_to_libname ("*NONE");
			break;
		case 'R': /* remove second product library */
			prodlib2 = dup_to_libname ("*NONE");
			break;
		case 'L': /* remove user libraries */
			userlibs_len = 0;
			memset (userlibs, 0, sizeof (char*) * 250);
			break;
		case 'c': /* set current library */
			curlib = dup_to_libname (optarg);
			break;
		case 'p': /* set first product library */
			prodlib1 = dup_to_libname (optarg);
			break;
		case 'P': /* set second product library */
			prodlib1 = dup_to_libname (optarg);
			break;
		case 'l': /* set a user library */
			if (userlibs_len >= 250) {
				print_msg_exit ("too many libs", 3);
			}
			if (userlibs_len == -1) {
				userlibs_len = 0;
			}
			userlibs [userlibs_len++] = dup_to_libname (optarg);
			break;
		default:
			usage (argv [0]);
		}
	}
	if (argc == optind) {
		usage (argv [0]);
	}
	qlichgll (curlib, prodlib1, prodlib2, userlibs, userlibs_len, &err);
	/* XXX: check err */
	if (err.exception_id [0] != '\0') {
		char code [8], msg [64];
		ebcdic2utf (err.exception_id, 7, code);
		code [7] = '\0'; /* truncate */
		sprintf (msg, "QLICHGLL returned exception code %s\n", code);
		print_msg_exit (msg, 2);
	}
	char **newargv = argv + optind;
	execvp (newargv [0], newargv);
	print_msg_exit ("failed to exec", 4);
	return 4; /* not reachable */
}
