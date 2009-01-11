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

#include <gloox/jid.h>
#include <gloox/client.h>
#include <gloox/vcard.h>
#include <gloox/vcardmanager.h>
#include <gloox/vcardhandler.h>
#include <gloox/connectionlistener.h>
#include <gloox/presence.h>
#include <gloox/disco.h>

#include <libsxc/generateString.hxx>

#include <VCardGetter.hxx>
#include <escape.hxx>
#include <save.hxx>

#include <libsxc/Debug/Logger.hxx>

/*}}}*/

VCardGetter::VCardGetter(/*{{{*/
  std::ostream &ostream,
  const gloox::JID login,
  const std::string pass,
  const gloox::JID jid,
  bool savePhoto,
  bool saveLogo)
: _ostream(ostream)
, _client(login, pass)
, _vCardManager(&_client)
, _jid(jid)
, _received(false)
, _errorText()
, _savePhoto(savePhoto)
, _saveLogo(saveLogo)
{
  _client.disableRoster();
  _client.registerConnectionListener(this);
  _client.disco()->setIdentity("client", "bot", PACKAGE);
  _client.disco()->setVersion(PACKAGE, VERSION);
  // Lowest priority.
  _client.setPresence(gloox::Presence::Unavailable, -128, "");
}/*}}}*/
void VCardGetter::run()/*{{{*/
{
  _client.connect();

  // Something went wrong, VCard not received.
  if (!_received)
    throw _errorText; //libsxc::Exception::Connection(_errorText);
}/*}}}*/

void VCardGetter::onConnect()/*{{{*/
{
  _vCardManager.fetchVCard(_jid, this);
}/*}}}*/
bool VCardGetter::onTLSConnect(const gloox::CertInfo &info)/*{{{*/
{
  return true;
}/*}}}*/
void VCardGetter::onDisconnect(gloox::ConnectionError e)/*{{{*/
{
  _errorText = libsxc::genConnErrorString(
    e,
    _client.streamError(),
    _client.streamErrorText(),
    _client.authError());
}/*}}}*/

void VCardGetter::handleVCard(/*{{{*/
  const gloox::JID &jid,
  const gloox::VCard *vcardConst)
{
  int i; // Internal counter.
  gloox::VCard *vcard = new gloox::VCard(*vcardConst);

  _ostream << "jid: "                << escape(vcard->jabberid()) << std::endl;
  _ostream << "nick: "               << escape(vcard->nickname()) << std::endl;
  _ostream << "name: "               << escape(vcard->formattedname())
           << std::endl;
  _ostream << "family name: "        << escape(vcard->name().family)
           << std::endl;
  _ostream << "given name: "         << escape(vcard->name().given)
           << std::endl;
  _ostream << "middle name: "        << escape(vcard->name().middle)
           << std::endl;
  _ostream << "name prefix: "        << escape(vcard->name().prefix)
           << std::endl;
  _ostream << "name suffix: "        << escape(vcard->name().suffix)
           << std::endl;
  _ostream << "url: "                << escape(vcard->url()) << std::endl;
  _ostream << "birthday: "           << escape(vcard->bday()) << std::endl;
  _ostream << "title: "              << escape(vcard->title()) << std::endl;
  _ostream << "role: "               << escape(vcard->role()) << std::endl;
  _ostream << "note: "               << escape(vcard->note()) << std::endl;
  _ostream << "desc: "               << escape(vcard->desc()) << std::endl;
  _ostream << "mailer: "             << escape(vcard->mailer()) << std::endl;
  _ostream << "last revision: "      << escape(vcard->rev()) << std::endl;
  _ostream << "unique identifier: "  << escape(vcard->uid()) << std::endl;
  _ostream << "timezone offset: "    << escape(vcard->tz()) << std::endl;
  vcard->setProdid(PACKAGE " " VERSION);
  _ostream << "product identifier: " << escape(vcard->prodid()) << std::endl;
  _ostream << "sort string: "        << escape(vcard->sortstring())
           << std::endl;

  const gloox::VCard::Photo &photo = vcard->photo();/*{{{*/

  if ("" != photo.extval) {
    // Photo is available at an external place. Only print the url.
    _ostream << "photo url: " << photo.extval << std::endl;
  } else if (_savePhoto && 0 != photo.binval.size()) {
    _ostream << "photo file: " 
             << save(jid.bare() + ".photo", photo.binval, photo.type)
             << std::endl;
  }/*}}}*/

  const gloox::VCard::Photo &logo = vcard->logo();/*{{{*/

  if ("" != logo.extval) {
    _ostream << "logo url: " << logo.extval << std::endl;
  } else if (_saveLogo && 0 != logo.binval.size()) {
    _ostream << "logo file: " 
             << save(jid.bare() + ".logo", logo.binval, logo.type)
             << std::endl;
  }/*}}}*/

  gloox::VCard::EmailList &eAddresses = vcard->emailAddresses();/*{{{*/
  i = 0;
  for (
  gloox::VCard::EmailList::iterator email = eAddresses.begin();
  eAddresses.end() != email;
  ++email, ++i)
  {
    _ostream << "email " << i << " address: " << escape(email->userid) 
             << std::endl;
    _ostream << "email " << i << " type: ";
    if (email->home)
      _ostream << "home, ";
    if (email->work)
      _ostream << "work, ";
    if (email->internet)
      _ostream << "internet, ";
    if (email->pref)
      _ostream << "preferred, ";
    if (email->x400)
      _ostream << "x400, ";
    _ostream << std::endl;
  }/*}}}*/

  gloox::VCard::AddressList &addresses = vcard->addresses();/*{{{*/
  i = 0;
  for (
  gloox::VCard::AddressList::iterator address = addresses.begin();
  addresses.end() != address;
  ++address, ++i)
  {
    _ostream << "address " << i << " pobox: " << escape(address->pobox)
              << std::endl;
    _ostream << "address " << i << " extra: " << escape(address->extadd)
              << std::endl;
    _ostream << "address " << i << " street: " << escape(address->street)
              << std::endl;
    _ostream << "address " << i << " city: " << escape(address->locality)
              << std::endl;
    _ostream << "address " << i << " region: " << escape(address->region)
              << std::endl;
    _ostream << "address " << i << " pcode: " << escape(address->pcode)
              << std::endl;
    _ostream << "address " << i << " country: " << escape(address->ctry)
              << std::endl;
    _ostream << "address " << i << " type: ";

    if (address->home)
      _ostream << "home, ";
    if (address->work)
      _ostream << "work, ";
    if (address->postal)
      _ostream << "postal, ";
    if (address->parcel)
      _ostream << "parcel, ";
    if (address->pref)
      _ostream << "preferred, ";
    if (address->dom)
      _ostream << "domestic, ";
    if (address->intl)
      _ostream << "international, ";

    _ostream << std::endl;
  }/*}}}*/

  gloox::VCard::LabelList &labels = vcard->labels();/*{{{*/
  i = 0;
  for (
  gloox::VCard::LabelList::iterator label = labels.begin();
  labels.end() != label;
  ++label, ++i)
  {
    _ostream << "label " << i << " label: ";
    _ostream << escape(label->lines) << std::endl;

    _ostream << "label " << i << " type: ";

    if (label->home)
      _ostream << "home, ";
    if (label->work)
      _ostream << "work, ";
    if (label->postal)
      _ostream << "postal, ";
    if (label->parcel)
      _ostream << "parcel, ";
    if (label->pref)
      _ostream << "preferred, ";
    if (label->dom)
      _ostream << "domestic, ";
    if (label->intl)
      _ostream << "international, ";

    _ostream << std::endl;
  }/*}}}*/

  gloox::VCard::TelephoneList &phones = vcard->telephone();/*{{{*/
  i = 0;
  for (
  gloox::VCard::TelephoneList::iterator phone = phones.begin();
  phones.end() != phone;
  ++phone, ++i)
  {
    _ostream << "phone " << i << " number: " << escape(phone->number)
             << std::endl;

    _ostream << "phone " << i << " type: ";

    if (phone->home)
      _ostream << "home, ";
    if (phone->work)
      _ostream << "work, ";
    if (phone->voice)
      _ostream << "voice, ";
    if (phone->fax)
      _ostream << "fax, ";
    if (phone->pager)
      _ostream << "pager, ";
    if (phone->msg)
      _ostream << "msg, ";
    if (phone->cell)
      _ostream << "cell, ";
    if (phone->video)
      _ostream << "video, ";
    if (phone->bbs)
      _ostream << "bbs, ";
    if (phone->modem)
      _ostream << "modem, ";
    if (phone->isdn)
      _ostream << "isdn, ";
    if (phone->pcs)
      _ostream << "pcs, ";
    if (phone->pref)
      _ostream << "preferred, ";

    _ostream << std::endl;
  }/*}}}*/

  _ostream << "latitude: "           << escape(vcard->geo().latitude)
           << std::endl;
  _ostream << "longitude: "          << escape(vcard->geo().longitude)
           << std::endl;
  _ostream << "organization name: "  << escape(vcard->org().name) << std::endl;

  std::list<std::string> orgUnits = vcard->org().units;/*{{{*/
  for (
  std::list<std::string>::iterator unit = orgUnits.begin();
  orgUnits.end() != unit;
  ++unit)
  {
    _ostream << "organization unit: " << escape(*unit) << std::endl;
  }/*}}}*/

  _ostream << "classification: ";/*{{{*/

  switch (vcard->classification()) {
  case gloox::VCard::ClassNone:
    _ostream << "none";
    break;
  case gloox::VCard::ClassPublic:
    _ostream << "public";
    break;
  case gloox::VCard::ClassPrivate:
    _ostream << "private";
    break;
  case gloox::VCard::ClassConfidential:
    _ostream << "confidential";
    break;
  }

  _ostream << std::endl;/*}}}*/

  delete vcardConst;
  _received = true;
  _client.disconnect();
}/*}}}*/
void VCardGetter::handleVCardResult(/*{{{*/
  gloox::VCardHandler::VCardContext context,
  const gloox::JID &jid,
  gloox::StanzaError se)
{
  if (gloox::VCardHandler::StoreVCard == context)
    throw std::logic_error(
      "handleVCardResult() should never be called to handle a store action.");

  // gloox::VCardHandler::FetchVCard
  if (gloox::StanzaErrorUndefined == se) {
    // No error occured. This function shouldn't even be called then...
    return;
  }
  // An error occured.
  std::cerr << libsxc::genStanzaErrorString(se) << std::endl;
  _client.disconnect();
}/*}}}*/

// Use no tabs at all; two spaces indentation; max. eighty chars per line.
// vim: et ts=2 sw=2 sts=2 tw=80 fdm=marker
