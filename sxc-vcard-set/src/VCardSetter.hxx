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

#include <gloox/vcardhandler.h>
#include <gloox/vcardmanager.h>
#include <gloox/connectionlistener.h>
#include <gloox/client.h>
#include <gloox/jid.h>

/*}}}*/


#ifndef VCARDSETTER_HXX
#define VCARDSETTER_HXX


class LogHandler : public gloox::LogHandler
{
  void handleLog(
    gloox::LogLevel level,
    gloox::LogArea area,
    const std::string &message);
};


/**
 * @brief Get a VCard and print it.
 *
 * This class logs in to a server and then sends a VCard fetch request. The
 * VCard is then printed to stdout.
 */
class VCardSetter : public gloox::VCardHandler, gloox::ConnectionListener
{
  public:
    //VCardSetter(istream, jid, pass, savePhoto, saveLogo);/*{{{*/

    /**
     * @brief Connect to the server.
     *
     * @param istream The input stream to get the VCard from.
     * @param login The JID to login to.
     * @param pass The password for the JID.
     * @param jid The JID from which the VCard gets fetched.
     * @param savePhoto Whether the save the photo to a file named $jid.photo.
     * @param saveLogo Whether the save the logo to a file named $jid.logo.
     */
    VCardSetter(
      std::istream &istream,
      const gloox::JID jid,
      const std::string pass,
      bool savePhoto = true,
      bool saveLogo = true);

/*}}}*/
    //void set();/*{{{*/

    /**
     */
    void set();

/*}}}*/

    // Reimplemented from gloox::ConnectionListener
    //virtual void onConnect();/*{{{*/

    /**
     * @brief This function is called when connected to the server.
     *
     * It initializes the asynchronous fetch of the VCard. See @ref handleVCard
     * for the result of the fetch request.
     */
    virtual void onConnect();

/*}}}*/
    //virtual void onTLSConnect(const CertInfo& info);/*{{{*/

    /**
     * @brief This function is called when a TLS connection is established.
     *
     * It always returns bool, so the certificate is always accepted.
     *
     * @return Whether to accept the certificate.
     */
    virtual bool onTLSConnect(const gloox::CertInfo &info);

/*}}}*/
    //virtual void onDisconnect(gloox::ConnectionError e);/*{{{*/

    /**
     * @brief This function is called on a disconnect from the server.
     */
    virtual void onDisconnect(gloox::ConnectionError e);

/*}}}*/

    // Reimplemented from gloox::VCardHandler
    //virtual void handleVCard(const gloox::JID &jid, VCard *vcard);/*{{{*/

    /**
     * @brief Handle an incomming VCard
     *
     * This function prints a received VCard and disconnects from the server.
     *
     * @param jid The JID of the owner of the VCard.
     * @param vcard The VCard.
     */
    virtual void handleVCard(const gloox::JID &jid, const gloox::VCard *vcard);

/*}}}*/
    //void handleVCardResult(context, &jid, StanzaError se);/*{{{*/

    /**
     * @brief Handle the result of a store of a VCard.
     *
     * This function does nothing.
     */
    void handleVCardResult(
      gloox::VCardHandler::VCardContext context,
      const gloox::JID &jid,
      gloox::StanzaError se);

/*}}}*/
  private:
    //std::istream &_istream;/*{{{*/

    /// The input stream to get the VCard from.
    std::istream &_istream;

/*}}}*/
    //gloox::Client _client;/*{{{*/

    /// The client.
    gloox::Client _client;

/*}}}*/
    //gloox::VCardManager _vCardManager;/*{{{*/

    /// The VCard manager.
    gloox::VCardManager _vCardManager;

/*}}}*/
    //bool _received;/*{{{*/

    /// Whether the VCard was received. If not, something went wrong.
    bool _received;

/*}}}*/
    //std::string _errorText;/*{{{*/

    /// An error text.
    std::string _errorText;

/*}}}*/
    //bool _savePhoto;/*{{{*/

    /// Whether to save the photo to a file named $jid.photo
    bool _savePhoto;

/*}}}*/
    //bool _saveLogo;/*{{{*/

    /// Whether to save the logo to a file named $jid.logo
    bool _saveLogo;

/*}}}*/
    LogHandler _logHandler;
};


#endif // VCARDSETTER_HXX
// Use no tabs at all; two spaces indentation; max. eighty chars per line.
// vim: et ts=2 sw=2 sts=2 tw=80 fdm=marker
