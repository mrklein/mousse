// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "phase_properties.hpp"
#include "dictionary_entry.hpp"
// Constructors 
mousse::phaseProperties::phaseProperties(Istream& is)
:
  phase_(UNKNOWN),
  stateLabel_("(unknown)"),
  names_(0),
  Y_(0),
  carrierIds_(0)
{
  is.check("mousse::phaseProperties::phaseProperties(Istream& is)");
  dictionaryEntry phaseInfo(dictionary::null, is);
  phase_ = phaseTypeNames[phaseInfo.keyword()];
  stateLabel_ = phaseToStateLabel(phase_);
  if (phaseInfo.size() > 0)
  {
    label nComponents = phaseInfo.size();
    names_.setSize(nComponents, "unknownSpecie");
    Y_.setSize(nComponents, 0.0);
    carrierIds_.setSize(nComponents, -1);
    label cmptI = 0;
    FOR_ALL_CONST_ITER(IDLList<entry>, phaseInfo, iter)
    {
      names_[cmptI] = iter().keyword();
      Y_[cmptI] = readScalar(phaseInfo.lookup(names_[cmptI]));
      cmptI++;
    }
    checkTotalMassFraction();
  }
}
// IOstream Operators 
mousse::Istream& mousse::operator>>(Istream& is, phaseProperties& pp)
{
  is.check
  (
    "mousse::Istream& mousse::operator>>(Istream&, phaseProperties&)"
  );
  dictionaryEntry phaseInfo(dictionary::null, is);
  pp.phase_ = pp.phaseTypeNames[phaseInfo.keyword()];
  pp.stateLabel_ = pp.phaseToStateLabel(pp.phase_);
  if (phaseInfo.size() > 0)
  {
    label nComponents = phaseInfo.size();
    pp.names_.setSize(nComponents, "unknownSpecie");
    pp.Y_.setSize(nComponents, 0.0);
    pp.carrierIds_.setSize(nComponents, -1);
    label cmptI = 0;
    FOR_ALL_CONST_ITER(IDLList<entry>, phaseInfo, iter)
    {
      pp.names_[cmptI] = iter().keyword();
      pp.Y_[cmptI] = readScalar(phaseInfo.lookup(pp.names_[cmptI]));
      cmptI++;
    }
    pp.checkTotalMassFraction();
  }
  return is;
}
mousse::Ostream& mousse::operator<<(Ostream& os, const phaseProperties& pp)
{
  os.check
  (
    "mousse::Ostream& mousse::operator<<(Ostream&, const phaseProperties&)"
  );
  os  << pp.phaseTypeNames[pp.phase_] << nl << token::BEGIN_BLOCK << nl
    << incrIndent;
  FOR_ALL(pp.names_, cmptI)
  {
    os.writeKeyword(pp.names_[cmptI]) << pp.Y_[cmptI]
      << token::END_STATEMENT << nl;
  }
  os  << decrIndent << token::END_BLOCK << nl;
  os.check
  (
    "mousse::Ostream& mousse::operator<<(Ostream&, const phaseProperties&)"
  );
  return os;
}
