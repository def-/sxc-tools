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

#include <string>
#include <vector>
#include <map>
#include <list>
#include <sstream>

#include <Parser.hxx>
#include <load.hxx>

#include <libsxc/Debug/Logger.hxx>

/*}}}*/

Parser::Parser()/*{{{*/
: _vCard()
{
}/*}}}*/

void Parser::parse(std::istream &istream)/*{{{*/
{
  std::string nameFamily;
  std::string nameGiven;
  std::string nameMiddle;
  std::string namePrefix;
  std::string nameSuffix;

  std::string latitude;
  std::string longitude;

  std::string orgName;
  std::list<std::string> orgUnits;

  std::map<int, Email> emails;
  std::map<int, Address> addresses;
  std::map<int, Label> labels;
  std::map<int, Phone> phones;

  std::string line;
  std::string entry;
  std::string value;
  // Multi-purpose: Position of colon in line, position of beginning of value,
  // and position of "\n"s to replace with newlines
  int pos;

  while (!istream.eof()) {
  //while (istream) {
    std::getline(istream, line);

    if (std::string::npos == line.find_first_not_of(" \t")) {
      // Ignore empty line.
      continue;
    }

    pos = line.find(':');
    if (0 == line.length() || '#' == line.at(0)) {
      continue; // Ignore the comment line.
    } else if (std::string::npos == pos) {
      // Not a valid entry.
      throw "FIXME for better exception: Invalid line (no entry): \""
      + line + "\".";
    }

    entry = line.substr(0, pos);

    // Find the beginning of the value.
    pos = line.find_first_not_of(' ', pos + 1);
    if (std::string::npos == pos) {
      value = "";
    } else {
      // Value found.
      value = line.substr(pos);
    }

    // Add all following lines, if the preceding one ends with an \.
    while (0 < value.length() && '\\' == value.at(value.length() - 1)) {
      value.erase(value.size() - 1);
      std::getline(istream, line);
      value.append("\n" + line);
    }


    // Replace all "\n" with real newlines.
    while (true) { // Break inside.
      pos = value.find("\\\n", pos);
      if (std::string::npos == pos)
        break;

      value.replace(pos, 2, "\n");
      ++pos; // Skip the newline just created in the next search.
    }

    LOG(
      "Parsing: (entry: \"" + entry + "\", value: \"" + value + "\").");

    // Set value. /*{{{*/
    if ("jid" == entry) {
      _vCard.setJabberid(value);
    } else if ("nick" == entry) {
      _vCard.setNickname(value);
    } else if ("name" == entry) {
      _vCard.setFormattedname(value);
    } else if ("family name" == entry) {
      nameFamily = value;
    } else if ("given name" == entry) {
      nameGiven = value;
    } else if ("middle name" == entry) {
      nameMiddle = value;
    } else if ("name prefix" == entry) {
      namePrefix = value;
    } else if ("name suffix" == entry) {
      nameSuffix = value;
    } else if ("url" == entry) {
      _vCard.setUrl(value);
    } else if ("birthday" == entry) {
      _vCard.setBday(value);
    } else if ("title" == entry) {
      _vCard.setTitle(value);
    } else if ("role" == entry) {
      _vCard.setRole(value);
    } else if ("note" == entry) {
      _vCard.setNote(value);
    } else if ("desc" == entry) {
      _vCard.setDesc(value);
    } else if ("mailer" == entry) {
      _vCard.setMailer(value);
    } else if ("last revision" == entry) {
      _vCard.setRev(value);
    } else if ("unique identifier" == entry) {
      _vCard.setUid(value);
    } else if ("timezone offset" == entry) {
      _vCard.setTz(value);
    } else if ("product identifier" == entry) {
      _vCard.setProdid(value);
    } else if ("sort string" == entry) {
      _vCard.setSortstring(value);
    } else if ("photo url" == entry) {
      _vCard.setPhoto(value);
    } else if ("photo file" == entry) {
      _vCard.setPhoto(getType(value), load(value));
    } else if ("logo url" == entry) {
      _vCard.setLogo(value);
    } else if ("logo file" == entry) {
      _vCard.setLogo(getType(value), load(value));
    } else if ("latitude" == entry) {
      latitude = value;
    } else if ("longitude" == entry) {
      longitude = value;
    } else if ("organization name" == entry) {
      orgName = value;
    } else if ("classification" == entry) {
      _vCard.setClass(getClass(value));
    } else { // Handle dynamic entries.
      // These can't be filled in directly, as they are composed of multiple
      // fields that first need to be put together.

      std::string prefix;
      unsigned int nr;
      std::string suffix;
      std::istringstream ss(value);

      ss >> prefix >> nr >> suffix;
      // FIXME: Whether is empty and good. (Enable exceptions instead?)
      if ("email" == prefix) {/*{{{*/
        std::map<int, Email>::iterator i = emails.find(nr);
        if (emails.end() == i) {
          // First add a new empty entry. Values will be filled in later (when
          // found).
          Email email;
          emails.insert(std::make_pair(nr, email));

          i = emails.find(nr); // Try to find it again
          if (emails.end() == i) {
            throw "FIXME for better exception: Can't add new email.";
          }
        }

        if ("address" == suffix) {
          i->second.address = value;
        } else if ("type" == suffix) {
          i->second.type = getAddressType(value);
        }
/*}}}*/
      } else if ("address" == prefix) {/*{{{*/
        std::map<int, Address>::iterator i = addresses.find(nr);
        if (addresses.end() == i) {
          // First add a new empty entry. Values will be filled in later (when
          // found).
          Address address;
          addresses.insert(std::make_pair(nr, address));

          i = addresses.find(nr); // Try to find it again
          if (addresses.end() == i) {
            throw "FIXME for better exception: Can't add new address.";
          }
        }

        if ("pobox" == suffix) {
          i->second.pobox = value;
        } else if ("extra" == suffix) {
          i->second.extadd = value;
        } else if ("street" == suffix) {
          i->second.street = value;
        } else if ("city" == suffix) {
          i->second.locality = value;
        } else if ("region" == suffix) {
          i->second.region = value;
        } else if ("pcode" == suffix) {
          i->second.pcode = value;
        } else if ("country" == suffix) {
          i->second.ctry = value;
        } else if ("type" == suffix) {
          i->second.type = getAddressType(value);
        }
/*}}}*/
      } else if ("label" == prefix) {/*{{{*/
        std::map<int, Label>::iterator i = labels.find(nr);
        if (labels.end() == i) {
          // First add a new empty entry. Values will be filled in later (when
          // found).
          Label label;
          labels.insert(std::make_pair(nr, label));

          i = labels.find(nr); // Try to find it again
          if (labels.end() == i) {
            throw "FIXME for better exception: Can't add new label.";
          }
        }

        if ("label" == suffix) {
          i->second.lines = split(value,'\n');
        } else if ("type" == suffix) {
          i->second.type = getAddressType(value);
        }
/*}}}*/
      } else if ("phone" == prefix) {/*{{{*/
        std::map<int, Phone>::iterator i = phones.find(nr);
        if (phones.end() == i) {
          // First add a new empty entry. Values will be filled in later (when
          // found).
          Phone phone;
          phones.insert(std::make_pair(nr, phone));

          i = phones.find(nr); // Try to find it again
          if (phones.end() == i) {
            throw "FIXME for better exception: Can't add new phone.";
          }
        }

        if ("number" == suffix) {
          i->second.number = value;
        } else if ("type" == suffix) {
          i->second.type = getAddressType(value);
        }
      }/*}}}*/

    }/*}}}*/
  }

  // Set values compound of multiple entries.
  _vCard.setName(nameFamily, nameGiven, nameMiddle, namePrefix, nameSuffix);
  _vCard.setGeo(latitude, longitude);
  _vCard.setOrganization(orgName, orgUnits);

  // FIXME!!!1
}/*}}}*/

const gloox::VCard &Parser::getVCard()/*{{{*/
{
  return _vCard;
}/*}}}*/

const std::string Parser::getType(const std::string &name)/*{{{*/
{
  int pos = name.rfind('.');
  if (std::string::npos == pos)
    return "";
  return name.substr(pos + 1);
}/*}}}*/

int Parser::getAddressType(const std::string &raw)/*{{{*/
{
  std::istringstream ss(raw);
  std::string word;
  int pos1;
  int pos2;
  int type;

  while (!ss.eof()) {
    std::getline(ss, word, ',');
    pos1 = word.find_first_not_of(" \t");
    pos2 = word.find_last_not_of(" \t");
    word = word.substr(pos1, pos2);

    // Adding to type. /*{{{*/
    if ("home" == word) {
      type |= gloox::VCard::AddrTypeHome;
    } else if ("work" == word) {
      type |= gloox::VCard::AddrTypeWork;
    } else if ("pref" == word) {
      type |= gloox::VCard::AddrTypePref;
    } else if ("x400" == word) {
      type |= gloox::VCard::AddrTypeX400;
    } else if ("internet" == word) {
      type |= gloox::VCard::AddrTypeInet;
    } else if ("parcel" == word) {
      type |= gloox::VCard::AddrTypeParcel;
    } else if ("postal" == word) {
      type |= gloox::VCard::AddrTypePostal;
    } else if ("domestic" == word) {
      type |= gloox::VCard::AddrTypeDom;
    } else if ("international" == word) {
      type |= gloox::VCard::AddrTypeIntl;
    } else if ("voice" == word) {
      type |= gloox::VCard::AddrTypeVoice;
    } else if ("fax" == word) {
      type |= gloox::VCard::AddrTypeFax;
    } else if ("pager" == word) {
      type |= gloox::VCard::AddrTypePager;
    } else if ("msg" == word) {
      type |= gloox::VCard::AddrTypeMsg;
    } else if ("cell" == word) {
      type |= gloox::VCard::AddrTypeCell;
    } else if ("video" == word) {
      type |= gloox::VCard::AddrTypeVideo;
    } else if ("bbs" == word) {
      type |= gloox::VCard::AddrTypeBbs;
    } else if ("modem" == word) {
      type |= gloox::VCard::AddrTypeModem;
    } else if ("isdn" == word) {
      type |= gloox::VCard::AddrTypeIsdn;
    } else if ("pcs" == word) {
      type |= gloox::VCard::AddrTypePcs;
    } else {
      throw "FIXME for better exception: Invalid address type: \""
      + word + "\"";
    }/*}}}*/
  }
}/*}}}*/

gloox::VCard::VCardClassification Parser::getClass(/*{{{*/
  const std::string &raw)
{
  if ("none" == raw) {
    return gloox::VCard::ClassNone;
  } else if ("public" == raw) {
    return gloox::VCard::ClassPublic;
  } else if ("private" == raw) {
    return gloox::VCard::ClassPrivate;
  } else if ("confidential" == raw) {
    return gloox::VCard::ClassConfidential;
  } else {
    throw "FIXME for better exception: Invalid classification: \"" + raw + "\"";
  }
}/*}}}*/

const std::list<std::string> Parser::split(const std::string &raw, char delim)/*{{{*/
{
  std::list<std::string> l;
  int posStart = 0;
  int posDelim;

  while (true) {
    posDelim = raw.find(delim, posStart);
    if (std::string::npos == posDelim)
      return l;

    l.push_back(raw.substr(posStart, posDelim - 1));
    posStart = posDelim + 1;
  }
}/*}}}*/


// Use no tabs at all; two spaces indentation; max. eighty chars per line.
// vim: et ts=2 sw=2 sts=2 tw=80 fdm=marker
