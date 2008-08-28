/**
 * TODO:
 * - Make the registration work with additional information (stdin or
 *   parameters)
 * - How about ['sexy] exceptions?
 */

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

/* $Id$ */

// INCLUDE/*{{{*/

#include <gloox/jid.h>

#include <libsxc/Option/Parser.hxx>
#include <libsxc/Option/Option.hxx>
#include <libsxc/Option/OptionPort.hxx>
#include <libsxc/Exception/Exception.hxx>

#include <Registerer.hxx>
#include <print.hxx>

/*}}}*/


/**
 * @file main.cpp
 * @author Dennis Felsing
 * @brief The starting point of sxc-register.
 *
 * Create a JID object from the parameter and start the registration process by
 * running @ref Registerer.
 */

int main(int argc, char *argv[])/*{{{*/
{
    libsxc::Option::Parser parser;
    libsxc::Option::OptionPort port(
        &parser, 'p', "port", "port", "0 - 65535, -1 for default");
    libsxc::Option::Option<gloox::JID> jid(
        &parser, ' ', "", "jid", "user@domain[/resource]");

    try {
        parser.parse(argv);
    } catch (libsxc::Exception::Exception &e) {
        printErr(e.getDescription());

        std::vector<std::string> usage = parser.getUsage();
        for_each(usage.begin(), usage.end(), printErr);

        return e.getType();
    }

    Registerer registrator;
    registrator.start(jid.getValue());

    return 0;
}/*}}}*/
