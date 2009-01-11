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
#include <list>
#include <istream>

#include <gloox/vcard.h>

/*}}}*/


#ifndef PARSER_HXX
#define PARSER_HXX


/**
 * @brief Parse input from one or more sources to a VCard.
 */
class Parser
{
  public:
    //Parser();/*{{{*/

    /**
     */
    Parser();

/*}}}*/

    //void parse(std::istream &istream);/*{{{*/

    /**
     * @brief Parse the input from a stream.
     *
     * This method can be called more than once. Newly set options will
     * overwrite the old ones.
     */
    void parse(std::istream &istream);

/*}}}*/
    //const gloox::VCard &getVCard();/*{{{*/

    /**
     */
    const gloox::VCard &getVCard();

/*}}}*/
  private:
    //const std::string getType(const std::string &name);/*{{{*/

    /**
     */
    const std::string getType(const std::string &name);

/*}}}*/
    //int getAddressType(const std::string &raw);/*{{{*/

    /**
     */
    int getAddressType(const std::string &raw);

/*}}}*/
    //gloox::VCard::VCardClassification getClass(const std::string &raw);/*{{{*/

    /**
     */
    gloox::VCard::VCardClassification getClass(const std::string &raw);

/*}}}*/
    //const std::list<std::string> split(const string &raw, char delim);/*{{{*/

    /**
     */
    const std::list<std::string> split(const std::string &raw, char delim);

/*}}}*/

    struct Email/*{{{*/
    {
      std::string address;
      int type;
    };

/*}}}*/
    struct Address/*{{{*/
    {
      std::string pobox;
      std::string extadd;
      std::string street;
      std::string locality;
      std::string region;
      std::string pcode;
      std::string ctry;
      int type;
    };

/*}}}*/
    struct Label/*{{{*/
    {
      std::list<std::string> lines;
      int type;
    };

/*}}}*/
    struct Phone/*{{{*/
    {
      std::string number;
      int type;
    };

/*}}}*/

    //gloox::VCard _vCard;/*{{{*/

    ///
    gloox::VCard _vCard;

/*}}}*/
};


#endif // PARSER_HXX
// Use no tabs at all; two spaces indentation; max. eighty chars per line.
// vim: et ts=2 sw=2 sts=2 tw=80 fdm=marker
