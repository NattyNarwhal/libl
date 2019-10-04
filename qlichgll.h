/* These probably don't match their names in XPF headers */
typedef struct {
	int bytes_in;
	int bytes_avail;
	char exception_id[7];
	char reserved;
} ERRC0100;

/*
 * Please read the manual for the program for structure contents:
 * https://www.ibm.com/support/knowledgecenter/ssw_ibm_i_71/apis/qlichgll.htm
 */

void qlichgll(char*, char*, char*, char**, int, ERRC0100*);
