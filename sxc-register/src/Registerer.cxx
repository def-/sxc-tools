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

#include <gloox/client.h>
#include <gloox/connectionlistener.h>
#include <gloox/registration.h>

#include <libsxc/generateString.hxx>
#include <libsxc/Exception/Type.hxx>

#include <iostream>
#include <string>
#include <cstdio> // stdin
#include <stdexcept> // std::runtime_error

#include <termios.h> // struct termios
#include <stdio.h> // fileno()
#include <string.h> // strerror()
#include <errno.h> // errno

#include <print.hxx>
#include <Registerer.hxx>

#ifdef HAVE_CONFIG_H
#   include <config.hxx>
#endif

/*}}}*/


Registerer::Registerer(gloox::JID jid, bool pwOnce)/*{{{*/
: _pwOnce(pwOnce),
  _isTerm(true),
  _jid(jid),
  _client(jid.server())
{
    _client.disableRoster();
    _client.registerConnectionListener(this);

    _registration = new gloox::Registration(&_client);
    _registration->registerRegistrationHandler(this);
}/*}}}*/
Registerer::~Registerer()/*{{{*/
{
    _client.disconnect();

    delete _registration;
}/*}}}*/

void Registerer::run()/*{{{*/
{
    _client.connect(); // Blocking connection.
}/*}}}*/
const std::string Registerer::enterPassword(bool retype)/*{{{*/
{
    struct termios savedTermState;
    std::string password;

    try {
        // Save a copy of the console state.
        if (tcgetattr(fileno(stdin), &savedTermState)) // Cin must track stdin.
            throw std::runtime_error(std::string("Get: ") + strerror(errno));

        // Suppress echo so password is not logged.
        struct termios newTermState = savedTermState;
        newTermState.c_lflag &= ~ECHO;

        if (tcsetattr(fileno(stdin), TCSAFLUSH, &newTermState))
            throw std::runtime_error(std::string("Set: ") + strerror(errno));

        // Verify that echo suppression is supported.
        if (newTermState.c_lflag & ECHO)
            throw std::runtime_error("Verify: unable to suppress echo");
    } catch (...) {
#       ifdef DEBUG
            printLog("Securing the terminal failed, assuming no terminal.");
#       endif

        _pwOnce = true; // Don't ask to retype.
        _isTerm = false;

        getline(std::cin, password);
        return password;
    }

    // Prompt the user for a password.
    std::cerr << "sxc-register: "
              << (retype ? "Retype Password: " : "Password: ") << std::flush;
    getline(std::cin, password);

    // Restore the terminal state.
    tcsetattr(fileno(stdin), TCSANOW, &savedTermState);
    std::cerr << std::endl;

    return password;
}/*}}}*/
void Registerer::handleRegistrationFields(/*{{{*/
    const gloox::JID &from,
    int fields,
    std::string instructions)
{
    gloox::RegistrationFields values;

    if (fields & gloox::Registration::FieldUsername)
        values.username = _jid.username();

    if (fields & gloox::Registration::FieldPassword) {
        while (true) {
            values.password = enterPassword();

            if (values.password == "") {
                printErr("Empty password not allowed.");
                if (!_isTerm)
                    exit(libsxc::Exception::Registration);
            } else if (_pwOnce || enterPassword(true) == values.password) {
                break;
            } else {
                printErr("Mismatch, try again.");
            }
        }
    }

    _registration->createAccount(fields, values);
}/*}}}*/
void Registerer::handleRegistrationResult(/*{{{*/
    const gloox::JID &from,
    gloox::RegistrationResult result)
{
    std::string text;

    switch (result) {
    case gloox::RegistrationSuccess:
        text = "Succeeded.";
        break;
    case gloox::RegistrationNotAcceptable:
        text = "Not all necassary information provided.";
        break;
    case gloox::RegistrationConflict:
        text = "Username already exists.";
        break;
    case gloox::RegistrationNotAuthorized:
        text = "Not authorized.";
        break;
    case gloox::RegistrationBadRequest:
        text = "Bad request.";
        break;
    case gloox::RegistrationForbidden:
        text = "Forbidden.";
        break;
    case gloox::RegistrationUnexpectedRequest:
        text = "Unexpected request.";
        break;
    default:
        text = "Unknown error.";
        break;
    }

    if (!text.empty())
        printErr("Registration: " + text);

    _client.disconnect();
    exit(result); // Exit the program.
}/*}}}*/

void Registerer::onConnect()/*{{{*/
{
    // Request the registration fields the server requires.
    _registration->fetchRegistrationFields();
}/*}}}*/
void Registerer::onDisconnect(gloox::ConnectionError e)/*{{{*/
{
    std::string text = libsxc::genConnErrorString(
        e,
        _client.streamError(),
        _client.streamErrorText(),
        _client.authError());
    if (!text.empty())
        printErr("Disconnected: " + text);
}/*}}}*/
    bool Registerer::onTLSConnect(const gloox::CertInfo& info)/*{{{*/
    {
        return true;
    }/*}}}*/
    void Registerer::handleAlreadyRegistered(const gloox::JID &from)/*{{{*/
    {
    }/*}}}*/
    void Registerer::handleDataForm(/*{{{*/
        const gloox::JID &from,
        const gloox::DataForm &form)
    {
    }/*}}}*/
    void Registerer::handleOOB(/*{{{*/
        const gloox::JID &from,
        const gloox::OOB &oob)
    {
    }/*}}}*/

// Use no tabs at all; two spaces indentation; max. eighty chars per line.
// vim: et ts=2 sw=2 sts=2 tw=80 fdm=marker
