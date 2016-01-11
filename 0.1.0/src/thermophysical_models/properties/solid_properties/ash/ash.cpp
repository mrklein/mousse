// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "ash.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse
{
  DEFINE_TYPE_NAME_AND_DEBUG(ash, 0);
  ADD_TO_RUN_TIME_SELECTION_TABLE(solidProperties, ash,);
  ADD_TO_RUN_TIME_SELECTION_TABLE(solidProperties, ash, Istream);
  ADD_TO_RUN_TIME_SELECTION_TABLE(solidProperties, ash, dictionary);
}
// Constructors 
mousse::ash::ash()
:
  solidProperties(2010, 710, 0.04, 0.0, 1.0)
{
  if (debug)
  {
    WARNING_IN("ash::ash()")
      << "Properties of ash need to be checked!!!"
      << endl;
  }
}
mousse::ash::ash(const solidProperties& s)
:
  solidProperties(s)
{}
mousse::ash::ash(Istream& is)
:
  solidProperties(is)
{}
mousse::ash::ash(const dictionary& dict)
:
  solidProperties(dict)
{}
mousse::ash::ash(const ash& s)
:
  solidProperties(s)
{}
// Member Functions 
void mousse::ash::writeData(Ostream& os) const
{
  solidProperties::writeData(os);
}
// IOStream operators 
mousse::Ostream& mousse::operator<<(Ostream& os, const ash& s)
{
  s.writeData(os);
  return os;
}
