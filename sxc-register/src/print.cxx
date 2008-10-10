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

#include <print.hxx>

#include <iostream>

/*}}}*/

//const std::string _outputPrefix;/*{{{*/

/// The text printed before every output using @ref printErr() and @ref printLog().
const std::string _outputPrefix = "sxc-register: ";


/*}}}*/

void printErr(std::string text)/*{{{*/
{
    std::cerr << _outputPrefix << text << std::endl;
}/*}}}*/

void printLog(std::string text)/*{{{*/
{
    std::clog << _outputPrefix << text << std::endl;
}/*}}}*/

// Use no tabs at all; two spaces indentation; max. eighty chars per line.
// vim: et ts=2 sw=2 sts=2 tw=80 fdm=marker
