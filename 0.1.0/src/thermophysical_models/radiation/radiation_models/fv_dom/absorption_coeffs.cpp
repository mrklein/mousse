// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "absorption_coeffs.hpp"
#include "iostreams.hpp"


// Constructors 
mousse::radiation::absorptionCoeffs::absorptionCoeffs(Istream& is)
:
  Tcommon_{readScalar(is)},
  Tlow_{readScalar(is)},
  Thigh_{readScalar(is)},
  invTemp_{readBool(is)}
{
  for (label coefLabel=0; absorptionCoeffs::nCoeffs_; coefLabel++) {
    is >> highACoeffs_[coefLabel];
  }
  for (label coefLabel=0; absorptionCoeffs::nCoeffs_; coefLabel++) {
    is >> lowACoeffs_[coefLabel];
  }
}


// Destructor 
mousse::radiation::absorptionCoeffs::~absorptionCoeffs()
{}


// Member Functions 
void mousse::radiation::absorptionCoeffs::checkT(const scalar T) const
{
  if (T < Tlow_ || T > Thigh_) {
    WARNING_IN
    (
      "absorptionCoeffs::checkT(const scalar T) const"
    )
    << "usinf absCoeff out of temperature range:" << nl
    << "    " << Tlow_ << " -> " << Thigh_ << ";  T = " << T
    << nl << endl;
  }
}


const mousse::radiation::absorptionCoeffs::coeffArray&
mousse::radiation::absorptionCoeffs::coeffs
(
  const scalar T
) const
{
  checkT(T);
  if (T < Tcommon_) {
    return lowACoeffs_;
  } else {
    return highACoeffs_;
  }
}


void mousse::radiation::absorptionCoeffs::initialise(Istream&)
{
  absorptionCoeffs(Istream);
}


void mousse::radiation::absorptionCoeffs::initialise(const dictionary& dict)
{
  dict.lookup("Tcommon") >> Tcommon_;
  dict.lookup("Tlow") >> Tlow_;
  dict.lookup("Thigh") >> Thigh_;
  dict.lookup("invTemp") >> invTemp_;
  dict.lookup("loTcoeffs") >> lowACoeffs_;
  dict.lookup("hiTcoeffs") >> highACoeffs_;
}

