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
#include <list>

/*}}}*/


#ifndef ESCAPE_HXX
#define ESCAPE_HXX


//std::string escape(std::string text);/*{{{*/

/**
 * @brief Escape a text, so it can be printed.
 *
 * Before every newline (\n) a backslash (\) is inserted. Hence you can
 * identify that the next line belongs to the same entry and is not a new one.
 *
 * @param text The raw text.
 * @return The escaped text.
 */
std::string escape(std::string text);

/*}}}*/
//std::string escape(const std::list<std::string> lines);/*{{{*/

/**
 * @brief Escape a text in the form of a list of strings
 *
 * @param lines The text.
 * @return The escaped text.
 */
std::string escape(const std::list<std::string> lines);

/*}}}*/


#endif // ESCAPE_HXX
// Use no tabs at all; two spaces indentation; max. eighty chars per line.
// vim: et ts=2 sw=2 sts=2 tw=80 fdm=marker
