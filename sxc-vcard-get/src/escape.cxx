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

#include <escape.hxx>

#include <string>
#include <list>

/*}}}*/

std::string escape(std::string text)/*{{{*/
{
  int pos = 0;

  while (true) {
    pos = text.find('\n', pos);

    if (std::string::npos == pos) // Not found.
      return text;

    text.insert(pos, "\\");
    pos += 2; // Skip two characaters (\\ and \n).
  }
}/*}}}*/
std::string escape(const std::list<std::string> lines)/*{{{*/
{
  std::string text = "";
  int pos = 0;

  for (
  std::list<std::string>::const_iterator line = lines.begin();
  lines.end() != line;)
  {
    if (lines.end() == ++line) // End of vector.
      return text;

    text += escape(*line);
    text += "\\\n";
  }
}/*}}}*/

// Use no tabs at all; two spaces indentation; max. eighty chars per line.
// vim: et ts=2 sw=2 sts=2 tw=80 fdm=marker
