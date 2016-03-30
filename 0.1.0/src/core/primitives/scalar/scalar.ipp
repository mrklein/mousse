// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

namespace mousse
{
const char* const pTraits<Scalar>::typeName = "scalar";
const Scalar pTraits<Scalar>::zero = 0.0;
const Scalar pTraits<Scalar>::one = 1.0;
const Scalar pTraits<Scalar>::min = -ScalarVGREAT;
const Scalar pTraits<Scalar>::max = ScalarVGREAT;
const Scalar pTraits<Scalar>::rootMin = -ScalarROOTVGREAT;
const Scalar pTraits<Scalar>::rootMax = ScalarROOTVGREAT;
const char* pTraits<Scalar>::componentNames[] = { "x" };

pTraits<Scalar>::pTraits(const Scalar& p)
:
  p_{p}
{}


pTraits<Scalar>::pTraits(Istream& is)
{
  is >> p_;
}


word name(const Scalar val)
{
  std::ostringstream buf;
  buf << val;
  return buf.str();
}


// IOstream Operators 
Scalar readScalar(Istream& is)
{
  Scalar rs;
  is  >> rs;
  return rs;
}


Istream& operator>>(Istream& is, Scalar& s)
{
  token t(is);
  if (!t.good())
  {
    is.setBad();
    return is;
  }
  if (t.isNumber())
  {
    s = t.number();
  }
  else
  {
    is.setBad();
    FATAL_IO_ERROR_IN("operator>>(Istream&, Scalar&)", is)
      << "wrong token type - expected Scalar, found " << t.info()
      << exit(FatalIOError);
    return is;
  }
  // Check state of Istream
  is.check("Istream& operator>>(Istream&, Scalar&)");
  return is;
}


Ostream& operator<<(Ostream& os, const Scalar s)
{
  os.write(s);
  os.check("Ostream& operator<<(Ostream&, const Scalar&)");
  return os;
}
}  // namespace mousse