// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "ca_co3.hpp"
#include "add_to_run_time_selection_table.hpp"


// Static Data Members
namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(CaCO3, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE(solidProperties, CaCO3,);
ADD_TO_RUN_TIME_SELECTION_TABLE(solidProperties, CaCO3, Istream);
ADD_TO_RUN_TIME_SELECTION_TABLE(solidProperties, CaCO3, dictionary);

}


// Constructors 
mousse::CaCO3::CaCO3()
:
  solidProperties{2710, 850, 1.3, 0.0, 1.0}
{
  if (debug) {
    WARNING_IN("CaCO3::CaCO3()")
      << "Properties of CaCO3 need to be checked!!!"
      << endl;
  }
}


mousse::CaCO3::CaCO3(const solidProperties& s)
:
  solidProperties(s)
{}


mousse::CaCO3::CaCO3(Istream& is)
:
  solidProperties(is)
{}


mousse::CaCO3::CaCO3(const dictionary& dict)
:
  solidProperties(dict)
{}


mousse::CaCO3::CaCO3(const CaCO3& s)
:
  solidProperties(s)
{}


// Member Functions 
void mousse::CaCO3::writeData(Ostream& os) const
{
  solidProperties::writeData(os);
}


// IOStream operators 
mousse::Ostream& mousse::operator<<(Ostream& os, const CaCO3& s)
{
  s.writeData(os);
  return os;
}

