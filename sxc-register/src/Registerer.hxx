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

#include <gloox/client.h>
#include <gloox/connectionlistener.h>
#include <gloox/registration.h>

/*}}}*/


/**
 * @class Registerer
 * @brief Main class for registering a new JID.
 *
 * Connects to the server and registers a new JID. Outputs errors to stderr.
 */
class Registerer
  : public gloox::RegistrationHandler, gloox::ConnectionListener
{
  public:
    //Registerer(gloox::JID jid, int port = -1, bool pwOnce = false);/*{{{*/

    /**
     * @brief Connect to the server.
     *
     * The constructor initializes the connection to the server chosen in
     * the JID and then waits until the connection is closed.
     *
     * @param jid The JID to register.
     */
    Registerer(gloox::JID jid, int port = -1, bool pwOnce = false);

/*}}}*/
    //~Registerer();/*{{{*/

    /**
     * @brief Clean up.
     */
    ~Registerer();

/*}}}*/

    //void run();/*{{{*/

    /**
     * @brief Run the registration process.
     *
     * Connect to the server and initialize the registration. This method
     * blocks until it is disconnected.
     */
    void run();

/*}}}*/
    //const std::string enterPassword(bool retype = false);/*{{{*//*{{{*/

    /**
     * @brief Get a password from stdin.
     *
     * This function waits for interaction from the user to get a password
     * without showing the entered characters. If stdin is not connected to
     * a terminal, it sets @ref _pwOnce to true, so the password only needs
     * to be entered once.
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
    const std::string enterField(const std::string &text) const;

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
    //bool onTLSConnect(const gloox::CertInfo& info);/*{{{*/

    /**
     * @brief Whether to accept the TLS certificate.
     *
     * Always accept.
     *
     * @param info The Certificate and additional information about it.
     * @return 'true' to accept, 'false' to cancel connection.
     */
    bool onTLSConnect(const gloox::CertInfo& info);

/*}}}*/
    //void handleAlreadyRegistered(const gloox::JID &from);/*{{{*/

    /**
     * @brief The wanted jid is already in use.
     */
    void handleAlreadyRegistered(const gloox::JID &from);

/*}}}*/
    //void handleDataForm(const JID &from, const DataForm form);/*{{{*/

    /**
     */
    void handleDataForm(
      const gloox::JID &from,
      const gloox::DataForm &form);

/*}}}*/
    //void handleOOB(const jid &from, const OOB &oob);/*{{{*/

    /**
     */
    void handleOOB(
      const gloox::JID &from,
      const gloox::OOB &oob);

/*}}}*/

  private:
    //bool _pwOnce;/*{{{*/

    /// Whether to disable the retype of the password.
    bool _pwOnce;

/*}}}*/
    //bool _isTerm;/*{{{*/

    /// Whether the input is a terminal.
    bool _isTerm;

/*}}}*/
    //const gloox::JID _jid;/*{{{*/

    /// The JID to extract the server and the username from.
    const gloox::JID _jid;

/*}}}*/
    //gloox::Client _client;/*{{{*/

    /// The client.
    gloox::Client _client;

/*}}}*/
    //gloox::Registration _registration;/*{{{*/

    /// The registration class.
    gloox::Registration *_registration;

/*}}}*/
};

// Use no tabs at all; two spaces indentation; max. eighty chars per line.
// vim: et ts=2 sw=2 sts=2 tw=80 fdm=marker
