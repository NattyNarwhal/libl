#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

static char *argv0;

void
set_program_name (char *progname)
{
	argv0 = progname;
}

void
print_msg_exit (char *msg, int ret)
{
	fprintf (stderr, "%s: %s\n", argv0, msg);
	exit (ret);
}
