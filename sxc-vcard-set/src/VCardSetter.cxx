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

#ifdef HAVE_CONFIG_H
# include <config.hxx>
#endif

#include <iostream>
#include <stdexcept>
#include <vector>
#include <map>
#include <list>
#include <sstream>

#include <gloox/jid.h>
#include <gloox/client.h>
#include <gloox/vcard.h>
#include <gloox/vcardmanager.h>
#include <gloox/vcardhandler.h>
#include <gloox/connectionlistener.h>
#include <gloox/presence.h>
#include <gloox/disco.h>
#include <gloox/loghandler.h>

#include <libsxc/generateString.hxx>
#include <libsxc/Debug/Logger.hxx>

#include <VCardSetter.hxx>
#include <Parser.hxx>
#include <load.hxx>

/*}}}*/

void LogHandler::handleLog(
  gloox::LogLevel level,
  gloox::LogArea area,
  const std::string &message)
{
  LOG(message);
}

VCardSetter::VCardSetter(/*{{{*/
  std::istream &istream,
  const gloox::JID jid,
  const std::string pass,
  bool savePhoto,
  bool saveLogo)
: _istream(istream)
, _client(jid, pass)
, _vCardManager(&_client)
, _received(false)
, _errorText()
, _savePhoto(savePhoto)
, _saveLogo(saveLogo)
, _logHandler()
{
  _client.logInstance().registerLogHandler(
    gloox::LogLevelDebug,
    gloox::LogAreaAll,
    &_logHandler);

  _client.disableRoster();
  _client.registerConnectionListener(this);
  _client.disco()->setIdentity("client", "bot", PACKAGE);
  _client.disco()->setVersion(PACKAGE, VERSION);
  // Lowest priority.
  _client.setPresence(gloox::Presence::Unavailable, -128, "");
}/*}}}*/

void VCardSetter::set()/*{{{*/
{
  _client.connect();

  // Something went wrong, server didn't signalize to have received the VCard.
  if (!_received) {
    throw _errorText; //FIXME: libsxc::Exception::Connection(_errorText);
  }
}/*}}}*/

void VCardSetter::onConnect()/*{{{*/
{
  LOG("Connection established.");

  Parser parser;

  parser.parse(_istream);
  LOG(parser.getVCard().nickname());
  _vCardManager.storeVCard(new gloox::VCard(parser.getVCard()), this);
}/*}}}*/
bool VCardSetter::onTLSConnect(const gloox::CertInfo &info)/*{{{*/
{
  return true;
}/*}}}*/
void VCardSetter::onDisconnect(gloox::ConnectionError e)/*{{{*/
{
  _errorText = libsxc::genConnErrorString(
    e,
    _client.streamError(),
    _client.streamErrorText(),
    _client.authError());
}/*}}}*/

void VCardSetter::handleVCard(/*{{{*/
  const gloox::JID &jid,
  const gloox::VCard *vcard)
{
  LOG("Unexpectedly received a VCard. Doing nothing.");

  // Silently ignore incoming VCards, as this may not be our fault.
  delete vcard;
}/*}}}*/
void VCardSetter::handleVCardResult(/*{{{*/
  gloox::VCardHandler::VCardContext context,
  const gloox::JID &jid,
  gloox::StanzaError se)
{
  if (gloox::VCardHandler::FetchVCard == context)
    return;

  if (gloox::StanzaErrorUndefined != se) {
    _errorText = libsxc::genStanzaErrorString(se);
  } else {
    _received = true;
  }
  _client.disconnect();
}/*}}}*/

// Use no tabs at all; two spaces indentation; max. eighty chars per line.
// vim: et ts=2 sw=2 sts=2 tw=80 fdm=marker
