// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "c.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse
{
  defineTypeNameAndDebug(C, 0);
  addToRunTimeSelectionTable(solidProperties, C,);
  addToRunTimeSelectionTable(solidProperties, C, Istream);
  addToRunTimeSelectionTable(solidProperties, C, dictionary);
}
// Constructors 
mousse::C::C()
:
  solidProperties(2010, 710, 0.04, 0.0, 1.0)
{
  if (debug)
  {
    WarningIn("C::C()")
      << "Properties of graphite need to be checked!!!"
      << endl;
  }
}
mousse::C::C(const solidProperties& s)
:
  solidProperties(s)
{}
mousse::C::C(Istream& is)
:
  solidProperties(is)
{}
mousse::C::C(const dictionary& dict)
:
  solidProperties(dict)
{}
mousse::C::C(const C& s)
:
  solidProperties(s)
{}
// Member Functions 
void mousse::C::writeData(Ostream& os) const
{
  solidProperties::writeData(os);
}
// IOStream operators 
mousse::Ostream& mousse::operator<<(Ostream& os, const C& s)
{
  s.writeData(os);
  return os;
}
