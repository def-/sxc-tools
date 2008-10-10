// LICENSE/*{{{*/
/*
  sxc-tools
  Copyright (C) 2008 Dennis Felsing, Andreas Waidler

  Permission to use, copy, modify, and/or distribute this software for any
  purpose with or without fee is hereby granted, provided that the above
  copyright notice and this permission notice appear in all copies.

  THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
  WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
  MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
  ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
  WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
  ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
  OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */
/*}}}*/


// INCLUDE/*{{{*/

#include <string>

/*}}}*/


#ifndef PRINT_HXX
#define PRINT_HXX


/**
 */

//void printErr(std::string text);/*{{{*/

/**
 * @brief Print a text to stderr.
 *
 * Format a raw text and print it to stderr.
 *
 * @note As sxc normally communicates using its own files, this
 *       method should only be used on critical errors, for example
 *       when you can't write to the output file or before the
 *       output file has been initialized.
 *
 * @param text The raw text to print.
 */
void printErr(std::string text);

/*}}}*/
//void printLog(std::string text);/*{{{*/

/**
 * @brief Print a logging text.
 *
 * Format a raw text and print it using clog. This usually goes to
 * stderr.
 *
 * @param text The raw text to print.
 */
void printLog(std::string text);

/*}}}*/


#endif // PRINT_HXX
// Use no tabs at all; two spaces indentation; max. eighty chars per line.
// vim: et ts=2 sw=2 sts=2 tw=80 fdm=marker
