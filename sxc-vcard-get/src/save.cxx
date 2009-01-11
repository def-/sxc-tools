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

#include <save.hxx>

#include <fstream>
#include <string>

/*}}}*/

std::string save(/*{{{*/
  const std::string &namePrefix,
  const std::string &binval,
  const std::string &type)
{
  std::string name = namePrefix;

  int posDelimiter = type.find("/");
  if (std::string::npos != posDelimiter) { // Valid mime type.
    // Add the mime type as ending.
    name += '.' + type.substr(posDelimiter + 1);
  }

  std::ofstream file(name.c_str(), std::ofstream::binary);
  //std::ofstream file(name.c_str(), std::ofstream::binary);
  file.write(binval.c_str(), binval.size());
  file.close();

  return name;
}/*}}}*/

// Use no tabs at all; two spaces indentation; max. eighty chars per line.
// vim: et ts=2 sw=2 sts=2 tw=80 fdm=marker
