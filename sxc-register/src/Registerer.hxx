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

#include <gloox/client.h>
#include <gloox/connectionlistener.h>
#include <gloox/registration.h>

/*}}}*/


/**
 * @class Registerer
 * @author Dennis Felsing
 * @brief Main class for registering a new JID.
 *
 * Connects to the server and registers a new JID. Outputs errors to stderr.
 */
class Registerer
    : public gloox::RegistrationHandler, gloox::ConnectionListener
{
    public:
        //void start(gloox::JID jid);/*{{{*/

        /**
         * @brief Connect to the server.
         *
         * This function initializes the connection to the server chosen in the
         * JID and then waits until the connection is closed.
         *
         * @param jid The JID to register.
         */
        void start(gloox::JID jid);

/*}}}*/
        //const std::string enterPassword(bool retype = false);/*{{{*//*{{{*/

        /**
         * @brief Get a password from stdin.
         *
         * This function waits for interaction from the user to get a password
         * without showing the entered characters.
         *
         * @param retype Whether this is the affirmation of the password.
         * @return The entered password.
         */
        const std::string enterPassword(bool retype = false);

/*}}}*//*}}}*/
        //const std::string enterField(std::string text);/*{{{*/

        /**
         * @brief Get an entry from stdin.
         *
         * This function asks the user to input a text.
         *
         * @return The entered text.
         */
        const std::string enterField(std::string text);

/*}}}*/
        //virtual void onConnect();/*{{{*/

        /**
         * @brief Call @ref fetchRegistrationFields().
         *
         * This function gets called after a connection has been established.
         * It then calls @ref fetchRegistrationFields().
         */
        virtual void onConnect();

/*}}}*/
        //virtual void onDisconnect(gloox::ConnectionError e);/*{{{*/

        /**
         * @brief Handle the error.
         *
         * This function output informations about unwanted disconnections.
         *
         * @param e The type of the error that lead to the disconnect.
         */
        virtual void onDisconnect(gloox::ConnectionError e);

/*}}}*/
        //virtual void handleRegistrationFields(/*{{{*/

        /**
         * @brief Create the new account.
         *
         * This function gets called when the server requests the fields. It
         * fills the username from the JID and the password from stdin and then
         * calls @ref gloox::Registration::createAccount().
         *
         * @param from The server or service the registration field came from.
         * @param fields The fields the server requires.
         * @param instructions Additional information from the server.
         */
        virtual void handleRegistrationFields(
            const gloox::JID &from,
            int fields,
            std::string instructions);

/*}}}*/
        //virtual void handleRegistrationResult(/*{{{*/

        /**
         * @brief Print the result of the registration.
         *
         * This function gets called when the registration request gets
         * answered by the server and prints the results to stdout.
         *
         * @param from The server or service the result came from.
         * @param regResult The result of the registration.
         */
        virtual void handleRegistrationResult(
            const gloox::JID &from,
            gloox::RegistrationResult result);
        void print(std::string text);

/*}}}*/

        // Reimplemented, but unused functions./*{{{*/
        Registerer() {}
        virtual ~Registerer() {}
        virtual bool onTLSConnect(const gloox::CertInfo& info) {}
        virtual void handleAlreadyRegistered(const gloox::JID &from) {}
        virtual void handleDataForm(
            const gloox::JID &from,
            const gloox::DataForm &form) {}
        virtual void handleOOB(
            const gloox::JID &from,
            const gloox::OOB &oob) {}/*}}}*/

    private:
        //static std::string prefix;/*{{{*/

        /// The text printed before every output.
        static std::string prefix;

/*}}}*/
        //static std::string registrationPrefix;/*{{{*/

        /// The text printed before every output about the registration.
        static std::string registrationPrefix;

/*}}}*/
        //static std::string connectionPrefix;/*{{{*/

        /// The text printed before every output about the connection.
        static std::string connectionPrefix;

/*}}}*/
        //gloox::JID *jid;/*{{{*/

        /// The JID to extract the server and the username from.
        gloox::JID *jid;

/*}}}*/
        //gloox::Registration *registration;/*{{{*/

        /// The registration class.
        gloox::Registration *registration;
        gloox::Client *client;

/*}}}*/
};
