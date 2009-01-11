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

#include <fstream>

/*}}}*/


#ifndef SAVE_HXX
#define SAVE_HXX


//std::string save(string &namePrefix, string &binval, string &type);/*{{{*/

/**
 * @brief Save a photo to a file.
 *
 * @param namePrefix The base of the name of the file.
 * @param binval The raw photo.
 * @param type The mime type that will be used when saving the image as the
 *             file ending.
 * @return The final filename.
 */
std::string save(
  const std::string &namePrefix,
  const std::string &binval,
  const std::string &type);

/*}}}*/


#endif // SAVE_HXX
// Use no tabs at all; two spaces indentation; max. eighty chars per line.
// vim: et ts=2 sw=2 sts=2 tw=80 fdm=marker
