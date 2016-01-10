// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include <limits>

#include "dimension_set.hpp"
#include "iostreams.hpp"
#include "dimensioned_scalar.hpp"
#include "istring_stream.hpp"
#include "dictionary.hpp"

// Constructors
mousse::dimensionSet::dimensionSet(Istream& is)
{
  is >> *this;
}
mousse::dimensionSet::tokeniser::tokeniser(Istream& is)
:
  is_(is),
  tokens_(100),
  start_(0),
  size_(0)
{}
// Member Functions
void mousse::dimensionSet::tokeniser::push(const token& t)
{
  label end = (start_+size_)%tokens_.size();
  tokens_[end] = t;
  if (size_ == tokens_.size())
  {
    start_ = tokens_.fcIndex(start_);
  }
  else
  {
    size_++;
  }
}
mousse::token mousse::dimensionSet::tokeniser::pop()
{
  token t = tokens_[start_];
  start_ = tokens_.fcIndex(start_);
  --size_;
  return t;
}
void mousse::dimensionSet::tokeniser::unpop(const token& t)
{
  ++size_;
  start_ = tokens_.rcIndex(start_);
  tokens_[start_] = t;
}
bool mousse::dimensionSet::tokeniser::hasToken() const
{
  return size_ || is_.good();
}
bool mousse::dimensionSet::tokeniser::valid(char c)
{
  return
  (
    !isspace(c)
  && c != '"'   // string quote
  && c != '\''  // string quote
  && c != '/'   // div
  && c != ';'   // end statement
  && c != '{'   // beg subdict
  && c != '}'   // end subdict
  && c != '('   // beg expr
  && c != ')'   // end expr
  && c != '['   // beg dim
  && c != ']'   // end dim
  && c != '^'   // power
  && c != '*'   // mult
  );
}
mousse::label mousse::dimensionSet::tokeniser::priority(const token& t)
{
  if (!t.isPunctuation())
  {
    return 0;
  }
  else if
  (
    t.pToken() == token::MULTIPLY
  || t.pToken() == token::DIVIDE
  )
  {
    return 2;
  }
  else if (t.pToken() == '^')
  {
    return 3;
  }
  else
  {
    return 0;
  }
}
void mousse::dimensionSet::tokeniser::splitWord(const word& w)
{
  size_t start = 0;
  for (size_t i=0; i<w.size(); ++i)
  {
    if (!valid(w[i]))
    {
      if (i > start)
      {
        word subWord = w(start, i-start);
        if (isdigit(subWord[0]) || subWord[0] == token::SUBTRACT)
        {
          push(token(readScalar(IStringStream(subWord)())));
        }
        else
        {
          push(token(subWord));
        }
      }
      if (w[i] != token::SPACE)
      {
        if (isdigit(w[i]))
        {
          push(token(readScalar(IStringStream(w[i])())));
        }
        else
        {
          push(token::punctuationToken(w[i]));
        }
      }
      start = i+1;
    }
  }
  if (start < w.size())
  {
    word subWord = w(start, w.size()-start);
    if (isdigit(subWord[0]) || subWord[0] == token::SUBTRACT)
    {
      push(token(readScalar(IStringStream(subWord)())));
    }
    else
    {
      push(token(subWord));
    }
  }
}
mousse::token mousse::dimensionSet::tokeniser::nextToken()
{
  if (size_ == 0)
  {
    token t(is_);
    if (t.isWord())
    {
      splitWord(t.wordToken());
      return pop();
    }
    else
    {
      return t;
    }
  }
  else
  {
    return pop();
  }
}
void mousse::dimensionSet::tokeniser::putBack(const token& t)
{
  if (size_ == 0)
  {
    push(t);
  }
  else
  {
    unpop(t);
  }
}
void mousse::dimensionSet::round(const scalar tol)
{
  for (int i=0; i < dimensionSet::nDimensions; ++i)
  {
    scalar integralPart;
    scalar fractionalPart = std::modf(exponents_[i], &integralPart);
    if (mag(fractionalPart-1.0) <= tol)
    {
      exponents_[i] = 1.0+integralPart;
    }
    else if (mag(fractionalPart+1.0) <= tol)
    {
      exponents_[i] = -1.0+integralPart;
    }
    else if (mag(fractionalPart) <= tol)
    {
      exponents_[i] = integralPart;
    }
  }
}
mousse::dimensionedScalar mousse::dimensionSet::parse
(
  const label lastPrior,
  tokeniser& tis,
  const HashTable<dimensionedScalar>& readSet
) const
{
  dimensionedScalar ds("", dimless, 1.0);
  // Get initial token
  token nextToken(tis.nextToken());
  // Store type of last token read. Used to detect two consecutive
  // symbols and assume multiplication
  bool haveReadSymbol = false;
  while (true)
  {
    if (nextToken.isWord())
    {
      const word& unitName = nextToken.wordToken();
      const dimensionedScalar& unitDim = readSet[unitName];
      ds.dimensions() *= unitDim.dimensions();
      ds.value() *= unitDim.value();
      haveReadSymbol = true;
    }
    else if (nextToken.isNumber())
    {
      // no dimensions, just value
      ds.value() *= nextToken.number();
      haveReadSymbol = true;
    }
    else if (nextToken.isPunctuation())
    {
      label nextPrior = tokeniser::priority(nextToken);
      if (nextToken.pToken() == token::BEGIN_SQR)
      {
        // No idea when this will happen
        tis.putBack(nextToken);
        return ds;
      }
      else if (nextToken.pToken() == token::END_SQR)
      {
        tis.putBack(nextToken);
        return ds;
      }
      else if (nextToken.pToken() == token::BEGIN_LIST)
      {
        dimensionedScalar sub(parse(nextPrior, tis, readSet));
        token t = tis.nextToken();
        if (!t.isPunctuation()  || t.pToken() !=  token::END_LIST)
        {
          FATAL_IO_ERROR_IN
          (
            "dimensionSet::parse"
            "(const label, tokeniser&"
            ", const HashTable<dimensionedScalar>&)",
            tis.stream()
          )   << "Illegal token " << t << exit(FatalIOError);
        }
        ds.dimensions() *= sub.dimensions();
        ds.value() *= sub.value();
        haveReadSymbol = true;
      }
      else if (nextToken.pToken() == token::END_LIST)
      {
        tis.putBack(nextToken);
        return ds;
      }
      else if (nextToken.pToken() == token::MULTIPLY)
      {
        if (nextPrior > lastPrior)
        {
          dimensionedScalar sub(parse(nextPrior, tis, readSet));
          ds.dimensions() *= sub.dimensions();
          ds.value() *= sub.value();
        }
        else
        {
          // Restore token
          tis.putBack(nextToken);
          return ds;
        }
        haveReadSymbol = false;
      }
      else if (nextToken.pToken() == token::DIVIDE)
      {
        if (nextPrior > lastPrior)
        {
          dimensionedScalar sub(parse(nextPrior, tis, readSet));
          ds.dimensions() /= sub.dimensions();
          ds.value() /= sub.value();
        }
        else
        {
          tis.putBack(nextToken);
          return ds;
        }
        haveReadSymbol = false;
      }
      else if (nextToken.pToken() == '^')
      {
        if (nextPrior > lastPrior)
        {
          dimensionedScalar exp(parse(nextPrior, tis, readSet));
          ds.dimensions().reset(pow(ds.dimensions(), exp.value()));
          // Round to nearest integer if close to it
          ds.dimensions().round(10*smallExponent);
          ds.value() = mousse::pow(ds.value(), exp.value());
        }
        else
        {
          tis.putBack(nextToken);
          return ds;
        }
        haveReadSymbol = false;
      }
      else
      {
        FATAL_IO_ERROR_IN
        (
          "dimensionSet::parse"
          "(const label, tokeniser&"
          ", const HashTable<dimensionedScalar>&)",
          tis.stream()
        )   << "Illegal token " << nextToken << exit(FatalIOError);
      }
    }
    else
    {
      FATAL_IO_ERROR_IN
      (
        "dimensionSet::parse"
        "(const label, tokeniser&"
        ", const HashTable<dimensionedScalar>&)",
        tis.stream()
      )   << "Illegal token " << nextToken << exit(FatalIOError);
    }
    if (!tis.hasToken())
    {
      break;
    }
    nextToken = tis.nextToken();
    if (nextToken.error())
    {
      break;
    }
    if (haveReadSymbol && (nextToken.isWord() || nextToken.isNumber()))
    {
      // Two consecutive symbols. Assume multiplication
      tis.putBack(nextToken);
      nextToken = token(token::MULTIPLY);
    }
  }
  return ds;
}
mousse::Istream& mousse::dimensionSet::read
(
  Istream& is,
  scalar& multiplier,
  const HashTable<dimensionedScalar>& readSet
)
{
  multiplier = 1.0;
  // Read begining of dimensionSet
  token startToken(is);
  if (startToken != token::BEGIN_SQR)
  {
    FATAL_IO_ERROR_IN
    (
      "dimensionSet::read"
      "(Istream&, scalar&, const HashTable<dimensionedScalar>&)",
      is
    )   << "expected a " << token::BEGIN_SQR << " in dimensionSet"
      << endl << "in stream " << is.info()
      << exit(FatalIOError);
  }
  // Read next token
  token nextToken(is);
  if (!nextToken.isNumber())
  {
    is.putBack(nextToken);
    tokeniser tis(is);
    dimensionedScalar ds(parse(0, tis, readSet));
    multiplier = ds.value();
    for (int i=0; i < dimensionSet::nDimensions; ++i)
    {
      exponents_[i] = ds.dimensions()[i];
    }
  }
  else
  {
    // Read first five dimensions
    exponents_[dimensionSet::MASS] = nextToken.number();
    for (int Dimension=1; Dimension<dimensionSet::CURRENT; Dimension++)
    {
      is >> exponents_[Dimension];
    }
    // Read next token
    token nextToken(is);
    // If next token is another number
    // read last two dimensions
    // and then read another token for the end of the dimensionSet
    if (nextToken.isNumber())
    {
      exponents_[dimensionSet::CURRENT] = nextToken.number();
      is >> nextToken;
      exponents_[dimensionSet::LUMINOUS_INTENSITY] = nextToken.number();
      is >> nextToken;
    }
    else
    {
      exponents_[dimensionSet::CURRENT] = 0;
      exponents_[dimensionSet::LUMINOUS_INTENSITY] = 0;
    }
    // Check end of dimensionSet
    if (nextToken != token::END_SQR)
    {
      FATAL_IO_ERROR_IN
      (
        "dimensionSet::read"
        "(Istream&, scalar&, const HashTable<dimensionedScalar>&)",
        is
      )   << "expected a " << token::END_SQR << " in dimensionSet "
        << endl << "in stream " << is.info()
        << exit(FatalIOError);
    }
  }
  // Check state of Istream
  is.check("Istream& operator>>(Istream&, dimensionSet&)");
  return is;
}
mousse::Istream& mousse::dimensionSet::read
(
  Istream& is,
  scalar& multiplier
)
{
  return read(is, multiplier, unitSet());
}
mousse::Istream& mousse::dimensionSet::read
(
  Istream& is,
  scalar& multiplier,
  const dictionary& readSet
)
{
  multiplier = 1.0;
  // Read begining of dimensionSet
  token startToken(is);
  if (startToken != token::BEGIN_SQR)
  {
    FATAL_IO_ERROR_IN
    (
      "dimensionSet::read"
      "(Istream&, scalar&, const dictionary&)",
      is
    )   << "expected a " << token::BEGIN_SQR << " in dimensionSet"
      << endl << "in stream " << is.info()
      << exit(FatalIOError);
  }
  // Read next token
  token nextToken(is);
  if (nextToken.isWord())
  {
    bool continueParsing = true;
    do
    {
      word symbolPow = nextToken.wordToken();
      if (symbolPow[symbolPow.size()-1] == token::END_SQR)
      {
        symbolPow = symbolPow(0, symbolPow.size()-1);
        continueParsing = false;
      }
      // Parse unit
      dimensionSet symbolSet(dimless);
      size_t index = symbolPow.find('^');
      if (index != string::npos)
      {
        word symbol = symbolPow(0, index);
        word exp = symbolPow(index+1, symbolPow.size()-index+1);
        scalar exponent = readScalar(IStringStream(exp)());
        dimensionedScalar s;
        s.read(readSet[symbol], readSet);
        symbolSet.reset(pow(s.dimensions(), exponent));
        // Round to nearest integer if close to it
        symbolSet.round(10*smallExponent);
        multiplier *= mousse::pow(s.value(), exponent);
      }
      else
      {
        dimensionedScalar s;
        s.read(readSet[symbolPow], readSet);
        symbolSet.reset(s.dimensions());
        multiplier *= s.value();
      }
      // Add dimensions without checking
      for (int i=0; i < dimensionSet::nDimensions; ++i)
      {
        exponents_[i] += symbolSet[i];
      }
      if (continueParsing)
      {
        nextToken = token(is);
        if (!nextToken.isWord() || nextToken == token::END_SQR)
        {
          continueParsing = false;
        }
      }
    }
    while (continueParsing);
  }
  else
  {
    // Read first five dimensions
    exponents_[dimensionSet::MASS] = nextToken.number();
    for (int Dimension=1; Dimension<dimensionSet::CURRENT; Dimension++)
    {
      is >> exponents_[Dimension];
    }
    // Read next token
    token nextToken(is);
    // If next token is another number
    // read last two dimensions
    // and then read another token for the end of the dimensionSet
    if (nextToken.isNumber())
    {
      exponents_[dimensionSet::CURRENT] = nextToken.number();
      is >> nextToken;
      exponents_[dimensionSet::LUMINOUS_INTENSITY] = nextToken.number();
      is >> nextToken;
    }
    else
    {
      exponents_[dimensionSet::CURRENT] = 0;
      exponents_[dimensionSet::LUMINOUS_INTENSITY] = 0;
    }
    // Check end of dimensionSet
    if (nextToken != token::END_SQR)
    {
      FATAL_IO_ERROR_IN
      (
        "dimensionSet::read"
        "(Istream&, scalar&, const dictionary&)",
        is
      )   << "expected a " << token::END_SQR << " in dimensionSet "
        << endl << "in stream " << is.info()
        << exit(FatalIOError);
    }
  }
  // Check state of Istream
  is.check("Istream& operator>>(Istream&, dimensionSet&)");
  return is;
}
mousse::Ostream& mousse::dimensionSet::write
(
  Ostream& os,
  scalar& multiplier,
  const dimensionSets& writeUnits
) const
{
  multiplier = 1.0;
  os << token::BEGIN_SQR;
  if (writeUnits.valid() && os.format() == IOstream::ASCII)
  {
    scalarField exponents(dimensionSet::nDimensions);
    for (int d=0; d<dimensionSet::nDimensions; d++)
    {
      exponents[d] = exponents_[d];
    }
    writeUnits.coefficients(exponents);
    bool hasPrinted = false;
    // Set precision to lots
    std::streamsize oldPrecision = os.precision
    (
      std::numeric_limits<scalar>::digits10
    );
    FOR_ALL(exponents, i)
    {
      if (mag(exponents[i]) > smallExponent)
      {
        const dimensionedScalar& ds = writeUnits.units()[i];
        if (hasPrinted)
        {
          os << token::SPACE;
        }
        hasPrinted = true;
        os << ds.name();
        if (mag(exponents[i]-1) > smallExponent)
        {
          os << '^' << exponents[i];
          multiplier *= mousse::pow(ds.value(), exponents[i]);
        }
        else
        {
          multiplier *= ds.value();
        }
      }
    }
    // Reset precision
    os.precision(oldPrecision);
  }
  else
  {
    for (int d=0; d<dimensionSet::nDimensions-1; d++)
    {
      os << exponents_[d] << token::SPACE;
    }
    os << exponents_[dimensionSet::nDimensions-1];
  }
  os  << token::END_SQR;
  // Check state of Ostream
  os.check("Ostream& operator<<(Ostream&, const dimensionSet&)");
  return os;
}
mousse::Ostream& mousse::dimensionSet::write
(
  Ostream& os,
  scalar& multiplier
) const
{
  return write(os, multiplier, writeUnitSet());
}

// IOstream Operators
mousse::Istream& mousse::operator>>(Istream& is, dimensionSet& dset)
{
  scalar multiplier;
  dset.read(is, multiplier);
  if (mag(multiplier-1.0) > dimensionSet::smallExponent)
  {
    FATAL_IO_ERROR_IN("mousse::operator>>(Istream&, dimensionSet&)", is)
      << "Cannot use scaled units in dimensionSet"
      << exit(FatalIOError);
  }
  // Check state of Istream
  is.check("Istream& operator>>(Istream&, dimensionSet&)");
  return is;
}
mousse::Ostream& mousse::operator<<(Ostream& os, const dimensionSet& dset)
{
  scalar multiplier;
  dset.write(os, multiplier);
  // Check state of Ostream
  os.check("Ostream& operator<<(Ostream&, const dimensionSet&)");
  return os;
}
