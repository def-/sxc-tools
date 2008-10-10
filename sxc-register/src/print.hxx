// LICENSE/*{{{*/
/*
  sxc - Simple Xmpp Client
  Copyright (C) 2008 Dennis Felsing, Andreas Waidler

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
/*}}}*/


// INCLUDE/*{{{*/

#include <string>

/*}}}*/


#ifndef PRINT_HXX
#define PRINT_HXX


/**
 * @author Dennis Felsing
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
// Use no tabs at all; four spaces indentation; max. eighty chars per line.
// vim: et ts=4 sw=4 tw=80 fo+=c fdm=marker
