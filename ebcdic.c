#include <iconv.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <as400_protos.h>

#include "error.h"

iconv_t e2a, a2e;

static void
init_iconv (void)
{
	char *pasecs = ccsidtocs (Qp2paseCCSID ());
	char *jobcs = ccsidtocs (Qp2jobCCSID ());
	/* XXX: This should use the system locales for PASE and XPF */
	e2a = iconv_open(pasecs, jobcs);
	a2e = iconv_open(jobcs, pasecs);
	if (e2a == (iconv_t)(-1) || a2e == (iconv_t)(-1)) {
		print_msg_exit ("iconv failed to init", 5);
	}
}

/*
 * Because EBCDIC strings are usually fixed-length and padded by ASCII, try to
 * cope by copying it to a fixed null-terminated buffer.
 */
size_t
ebcdic2utf (char *ebcdic, int ebcdic_len, char *utf)
{
	size_t inleft, outleft, ret;
	if (e2a == NULL) {
		init_iconv ();
	}
	inleft = outleft = ebcdic_len + 1;
	char *temp;
	temp = malloc (ebcdic_len + 1);
	strncpy (temp, ebcdic, ebcdic_len);
	temp [ebcdic_len] = '\0';
	ret = iconv (e2a, &temp, &inleft, &utf, &outleft);
	free (temp);
	return ret;
}

/* Convert a UTF-8 string to a fixed-length EBCDIC string. */
size_t utf2ebcdic (char *utf, int ebcdic_len, char *ebcdic)
{
	size_t inleft, outleft, ret;
	if (a2e == NULL) {
		init_iconv ();
	}
	inleft = outleft = ebcdic_len + 1;
	char *temp;
	temp = malloc (ebcdic_len + 1);
	sprintf (temp, "%-10s", utf);
	ret = iconv (a2e, &temp, &inleft, &ebcdic, &outleft);
	free (temp);
	return ret;
}

