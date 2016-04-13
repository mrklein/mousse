// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "error.hpp"
#include "token.hpp"
#include "iostreams.hpp"
#include "scalar.hpp"


// Constructors 
mousse::token::token(Istream& is)
:
  type_{UNDEFINED}
{
  is.read(*this);
}


// IOstream operators 
mousse::Istream& mousse::operator>>(Istream& is, token& t)
{
  t.clear();
  return is.read(t);
}


mousse::Ostream& mousse::operator<<(Ostream& os, const token& t)
{
  switch (t.type_) {
    case token::UNDEFINED:
      os << "UNDEFINED";
      WARNING_IN("Ostream& operator<<(Ostream&, const token&)")
        << "Undefined token" << endl;
    break;
    case token::PUNCTUATION:
      os << t.punctuationToken_;
    break;
    case token::WORD:
      os << *t.wordTokenPtr_;
    break;
    case token::STRING:
    case token::VERBATIMSTRING:
      os << *t.stringTokenPtr_;
    break;
    case token::VARIABLE:
      // Behaviour differs according to stream type
      os.write(t);
    break;
    case token::LABEL:
      os << t.labelToken_;
    break;
    case token::FLOAT_SCALAR:
      os << t.floatScalarToken_;
    break;
    case token::DOUBLE_SCALAR:
      os << t.doubleScalarToken_;
    break;
    case token::COMPOUND:
      os << *t.compoundTokenPtr_;
    break;
    case token::ERROR:
      os << "ERROR";
      WARNING_IN("Ostream& operator<<(Ostream&, const token&)")
        << "Error token" << endl;
    break;
    default:
      os << "UNKNOWN";
      SERIOUS_ERROR_IN("Ostream& operator<<(Ostream&, const token&)")
        << "Unknown token"
        << endl;
  }
  // Check state of stream
  os.check("Ostream& operator<<(Ostream&, const token&)");
  return os;
}


ostream& mousse::operator<<(ostream& os, const token::punctuationToken& pt)
{
  return os << char(pt);
}


mousse::Ostream& mousse::operator<<(Ostream& os, const token::punctuationToken& pt)
{
  return os << char(pt);
}


mousse::Ostream& mousse::operator<<(Ostream& os, const token::compound& ct)
{
  os << ct.type() << token::SPACE;
  ct.write(os);
  return os;
}


ostream& mousse::operator<<(ostream& os, const InfoProxy<token>& ip)
{
  const token& t = ip.t_;
  os  << "on line " << t.lineNumber();
  switch (t.type()) {
    case token::UNDEFINED:
      os  << " an undefined token";
    break;
    case token::PUNCTUATION:
      os  << " the punctuation token " << '\'' << t.pToken() << '\'';
    break;
    case token::WORD:
      os  << " the word " << '\'' << t.wordToken() << '\'';
    break;
    case token::STRING:
      os  << " the string " << t.stringToken();
    break;
    case token::VARIABLE:
      os  << " the variable " << t.stringToken();
    break;
    case token::VERBATIMSTRING:
      os  << " the verbatim string " << t.stringToken();
    break;
    case token::LABEL:
      os  << " the label " << t.labelToken();
    break;
    case token::FLOAT_SCALAR:
      os  << " the floatScalar " << t.floatScalarToken();
    break;
    case token::DOUBLE_SCALAR:
      os  << " the doubleScalar " << t.doubleScalarToken();
    break;
    case token::COMPOUND: {
      if (t.compoundToken().empty()) {
        os << " the empty compound of type "
          << t.compoundToken().type();
      } else {
        os << " the compound of type "
          << t.compoundToken().type();
      }
    }
    break;
    case token::ERROR:
      os  << " an error";
    break;
    default:
      os  << " an unknown token type " << '\'' << int(t.type()) << '\'';
  }
  return os;
}


template<>
mousse::Ostream& mousse::operator<<(Ostream& os, const InfoProxy<token>& ip)
{
  const token& t = ip.t_;
  os  << "on line " << t.lineNumber();
  switch (t.type()) {
    case token::UNDEFINED:
      os  << " an undefined token";
    break;
    case token::PUNCTUATION:
      os  << " the punctuation token " << '\'' << t.pToken() << '\'';
    break;
    case token::WORD:
      os  << " the word " << '\'' << t.wordToken() << '\'';
    break;
    case token::STRING:
      os  << " the string " << t.stringToken();
    break;
    case token::VARIABLE:
      os  << " the variable " << t.stringToken();
    break;
    case token::VERBATIMSTRING:
      os  << " the verbatim string " << t.stringToken();
    break;
    case token::LABEL:
      os  << " the label " << t.labelToken();
    break;
    case token::FLOAT_SCALAR:
      os  << " the floatScalar " << t.floatScalarToken();
    break;
    case token::DOUBLE_SCALAR:
      os  << " the doubleScalar " << t.doubleScalarToken();
    break;
    case token::COMPOUND:
    {
      if (t.compoundToken().empty())
      {
        os  << " the empty compound of type "
          << t.compoundToken().type();
      }
      else
      {
        os  << " the compound of type "
          << t.compoundToken().type();
      }
    }
    break;
    case token::ERROR:
      os  << " an error";
    break;
    default:
      os  << " an unknown token type "  << '\'' << int(t.type()) << '\'';
  }
  return os;
}
