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
#include <iostream>

#include <gloox/jid.h>

#include <libsxc/Option/Parser.hxx>
#include <libsxc/Option/Option.hxx>
#include <libsxc/Option/OptionPort.hxx>
#include <libsxc/Exception/Exception.hxx>
#include <libsxc/Exit/Code.hxx>

#include <Registerer.hxx>

#ifdef HAVE_CONFIG_H
# include <config.hxx>
#endif

/*}}}*/

/**
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
  } catch (libsxc::Exception::Exception &e) {
    std::cerr << e.what() << std::endl;
    return e.getExitCode();
  } catch(std::exception &e) {
    std::cerr << e.what() << std::endl;
    return libsxc::Exit::General;
  } catch(...) {
    std::cerr << "Unexpected error parsing options." << std::endl;
    return libsxc::Exit::General;
  }

  if (parser.doShowHelp()) {
    std::cerr << PACKAGE " " VERSION " (C) " COPYRIGHT << std::endl;

    std::vector<std::string> usage = parser.getUsage();
    for(
    std::vector<std::string>::iterator it = usage.begin();
    usage.end() != it;
    ++it) {
      std::cerr << *it << std::endl;
    }
    return libsxc::Exit::NoError;
  }

  if (parser.doShowVersion()) {
    std::cerr << VERSION << std::endl;
    return libsxc::Exit::NoError;
  }

  Registerer registerer(jid.getValue(), port.getValue(), pwOnce.getValue());
  registerer.run();

  return 0;
}/*}}}*/

// Use no tabs at all; two spaces indentation; max. eighty chars per line.
// vim: et ts=2 sw=2 sts=2 tw=80 fdm=marker
