// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

// Constructors 
inline mousse::JanevReactionRate::JanevReactionRate
(
  const scalar A,
  const scalar beta,
  const scalar Ta,
  const FixedList<scalar, nb_> b
)
:
  A_(A),
  beta_(beta),
  Ta_(Ta),
  b_(b)
{}
inline mousse::JanevReactionRate::JanevReactionRate
(
  const speciesTable&,
  Istream& is
)
:
  A_(readScalar(is.readBegin("JanevReactionRate(Istream&)"))),
  beta_(readScalar(is)),
  Ta_(readScalar(is)),
  b_(is)
{
  is.readEnd("JanevReactionRate(Istream&)");
}
inline mousse::JanevReactionRate::JanevReactionRate
(
  const speciesTable&,
  const dictionary& dict
)
:
  A_(readScalar(dict.lookup("A"))),
  beta_(readScalar(dict.lookup("beta"))),
  Ta_(readScalar(dict.lookup("Ta"))),
  b_(dict.lookup("b"))
{}
// Member Functions 
inline mousse::scalar mousse::JanevReactionRate::operator()
(
  const scalar /*p*/,
  const scalar T,
  const scalarField&
) const
{
  scalar lta = A_;
  if (mag(beta_) > VSMALL)
  {
    lta *= pow(T, beta_);
  }
  scalar expArg = 0.0;
  if (mag(Ta_) > VSMALL)
  {
    expArg -= Ta_/T;
  }
  scalar lnT = log(T);
  for (int n=0; n<nb_; n++)
  {
    expArg += b_[n]*pow(lnT, n);
  }
  lta *= exp(expArg);
  return lta;
}
inline void mousse::JanevReactionRate::write(Ostream& os) const
{
  os.writeKeyword("A") << A_ << nl;
  os.writeKeyword("beta") << beta_ << nl;
  os.writeKeyword("Ta") << Ta_ << nl;
  os.writeKeyword("b") << b_ << nl;
}
inline mousse::Ostream& mousse::operator<<
(
  Ostream& os,
  const JanevReactionRate& jrr
)
{
  os  << token::BEGIN_LIST
    << jrr.A_ << token::SPACE << jrr.beta_ << token::SPACE << jrr.Ta_;
  for (int n=0; n<JanevReactionRate::nb_; n++)
  {
    os  << token::SPACE << jrr.b_[n];
  }
  os << token::END_LIST;
  return os;
}
