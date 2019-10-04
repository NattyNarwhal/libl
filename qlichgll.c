#include "qlichgll.h"

#include <as400_protos.h>
#include <stdbool.h>

ILEpointer pgm __attribute__ ((aligned (16)));
bool initialized = false;

static bool
init_pgm (void)
{
	if (0 != _RSLOBJ2(&pgm, RSLOBJ_TS_PGM, "QLICHGLL", "QSYS")) {
		return false;
	}
	initialized = true;
	return true;
}

/* pass by value is what we expect, but PGMCALL wants pass by ref because */

void
qlichgll (char *cur, char* prodFirst, char *prodSecond, char **userLibs, int userLibCount, ERRC0100 *error)
{
	if (!initialized) {
		if (!init_pgm()) {
			/* XXX: fail */
		}
	}
	/* Assume caller passes in EBCDIC */
	void *pgm_argv[] = {
		cur,
		prodFirst,
		prodSecond,
		userLibs,
		&userLibCount,
		error,
		NULL
	};
	if (0 != _PGMCALL(&pgm, pgm_argv, 0)) {
		/* XXX: fail */
	}
	/* check error */
}
