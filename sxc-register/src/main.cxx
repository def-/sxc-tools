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

#include <string>
#include <iostream>

#include <gloox/jid.h>

#include <libsxc/Option/Parser.hxx>
#include <libsxc/Option/Option.hxx>
#include <libsxc/Option/OptionPort.hxx>
#include <libsxc/Exception/Exception.hxx>

#include <Registerer.hxx>
#include <print.hxx>

/*}}}*/


/**
 * @author Dennis Felsing
 * @brief The starting point of sxc-register.
 *
 * Create a JID object from the parameter and start the registration process by
 * running @ref Registerer.
 */
int main(int argc, char *argv[])/*{{{*/
{
    libsxc::Option::Parser parser;
    libsxc::Option::Option<bool> defHelp(
        &parser, 'h', "help", "Show help and exit");
    libsxc::Option::Option<bool> defVersion(
        &parser, 'V', "version", "Show version and exit");
    libsxc::Option::OptionPort port(
        &parser, 'p', "port", "port", "0 - 65535, -1 for default");
    libsxc::Option::Option<bool> pwOnce(
        &parser, 'o', "once", "Type password once (disable retype)");
    libsxc::Option::Option<gloox::JID> jid(
        &parser, ' ', "", "jid", "user@domain[/resource]");

    try {
        parser.parse(argv);
    } catch (libsxc::Exception::OptionException &e) {
        if (libsxc::Exception::ShowUsage == e.getType()) {
            std::cerr << PACKAGE << " " << VERSION << " (C) " << COPYRIGHT
                      << std::endl;
        } else if (libsxc::Exception::ShowVersion == e.getType()) {
            std::cerr << VERSION << std::endl;
            return libsxc::Exception::NoError;
        } else {
            printErr(e.getDescription());
        }

        std::vector<std::string> usage = parser.getUsage();
        for(
        std::vector<std::string>::iterator line = usage.begin();
        line != usage.end();
        ++line)
            std::cerr << *line << std::endl;

        if (e.getType() < 0) // No error. (ShowUsage, ShowVersion)
            return libsxc::Exception::NoError;
        return e.getType();
    } catch (libsxc::Exception::Exception &e) {
        printErr(e.getDescription());
        return e.getType();
    }

    Registerer registerer(jid.getValue(), pwOnce.getValue());
    registerer.run();

    return 0;
}/*}}}*/

// Use no tabs at all; four spaces indentation; max. eighty chars per line.
// vim: et ts=4 sw=4 tw=80 fo+=c fdm=marker
