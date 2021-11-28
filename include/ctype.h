#ifndef CTYPE_H
#define CTYPE_H

/*
 * NOTE! This ctype does not handle EOF like the standard C
 * library is required to.
 */

#define _U	0x01	//!< upper.
#define _L	0x02	//!< lower.
#define _D	0x04	//!< digit.
#define _C	0x08	//!< cntrl.
#define _P	0x10	//!< punct.
#define _S	0x20	//!< white space (space/lf/tab).
#define _X	0x40	//!< hex digit.
#define _SP	0x80	//!< hard space (0x20).

extern unsigned char _ctype[];

#define __ismask(x) (_ctype[(int)(unsigned char)(x)])

//! Return a value != 0 only if \p c is an alphanumeric character.
#define isalnum(c)	((__ismask(c)&(_U|_L|_D)) != 0)
//! Return a value != 0 only if \p c is an alphabetic character.
#define isalpha(c)	((__ismask(c)&(_U|_L)) != 0)
//! Return a value != 0 only if \p c is a control character.
#define iscntrl(c)	((__ismask(c)&(_C)) != 0)
//! Return a value != 0 only if \p c is a digit character.
#define isdigit(c)	((__ismask(c)&(_D)) != 0)
//! Return a value != 0 only if \p c is a printable (not a space)
//! character.
#define isgraph(c)	((__ismask(c)&(_P|_U|_L|_D)) != 0)
//! Return a value != 0 only if \p c is a lower case character.
#define islower(c)	((__ismask(c)&(_L)) != 0)
//! Return a value != 0 only if \p c is a printable character.
#define isprint(c)	((__ismask(c)&(_P|_U|_L|_D|_SP)) != 0)
//! Return a value != 0 only if \p c is a punctuation-mark.
#define ispunct(c)	((__ismask(c)&(_P)) != 0)
//! Return a value != 0 only if \p c is an empty space
//! like TAB, CR, LF...
#define isspace(c)	((__ismask(c)&(_S)) != 0)
//! Return a value != 0 only if \p c is an upper case character.
#define isupper(c)	((__ismask(c)&(_U)) != 0)
//! Return a value != 0 only if \p c is a hexadecimal digit.
#define isxdigit(c)	((__ismask(c)&(_D|_X)) != 0)
//! Return a value != 0 only if \p c is an ASCII character <= 127.
#define isascii(c) (((unsigned char)(c))<=0x7f)
//! Return only the ASCII lower 7 bits of the \p c character.
#define toascii(c) (((unsigned char)(c))&0x7f)

//! \brief Convert a character into the lower case.
//! \param c The character to convert.
//! \return The lower case of \p c.
static inline unsigned char __tolower(unsigned char c)
{
	if (isupper(c))
		c -= 'A'-'a';
	return c;
}

//! \brief Convert a character into the upper case.
//! \param c The character to convert.
//! \return The upper case of \p c.
static inline unsigned char __toupper(unsigned char c)
{
	if (islower(c))
		c -= 'a'-'A';
	return c;
}

//! Macro equivalent to __tolower(c).
#define tolower(c) __tolower(c)

//! Macro equivalent to __toupper(c).
#define toupper(c) __toupper(c)

#endif
