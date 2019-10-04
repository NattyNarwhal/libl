# `libl`

This program sets the library list when invoking a PASE program. It provides
a thin wrapper around the [`qlichgll`][qlichgll] API. While PASE programs may
not necessarily care about the library list, calls into native IBM i programs
might; this provides a way for PASE-centric programs and scripts to cope with
this.

## Usage

Please consult the `qlichgll` documentation if anything seems unclear.

`libl` takes some flags; the remainder is used to invoke a program with the
new library list. (This is because changing the library list only does it
for the program it was invoked it; if run without flags; `libl` would change
the list in itself and exit, which wouldn't be terribly useful.)

By default, flags not set will not alter that part of the library list.

* `-l`: Add a library to replace the user part of the library list.
* `-L`: Remove the user part of the library list.
* `-c`: Change the current library.
* `-C`: Remove the current library; essentially defaults to `QGPL`.
* `-p`: Set the first product library.
* `-P`: Set the second product library.
* `-r`: Remove the first product library.
* `-R`: Remove the second product library.

After these flags, the argument vector of the program to call is taken. While
the `--` to begin the argument vector of the program to call is not required,
it clarifies that `libl` will no longer look for flags after it.

In the event that an error occurs, `libl` will print to `stderr` and set the
return code.

## Example

```
$ libl -c calvin -- system "dsplib lib(*curlib)"
 Library . . . . . . . . . . . . . . . . :   CALVIN
[...]
$ system "dsplib lib(*curlib)"
 Library . . . . . . . . . . . . . . . . :   QGPL
[...]
$ libl -c dummy -- system "dsplib lib(*curlib)"
libl: QLICHGLL returned exception code CPF2110
```

## Building

The source is plain C99. It will need GCC and the system libiconv. To build,
simply run `make`. It can be installed by sticking the created `libl` program
somewhere on your `$PATH`.

[qlichgll]: https://www.ibm.com/support/knowledgecenter/ssw_ibm_i_71/apis/qlichgll.htm "QLICHGLL API"
