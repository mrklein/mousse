// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "dummy.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse
{
namespace diameterModels
{
namespace IATEsources
{
  defineTypeNameAndDebug(dummy, 0);
  addToRunTimeSelectionTable(IATEsource, dummy, word);
}
}
}
// Member Functions 
mousse::tmp<mousse::volScalarField>
mousse::diameterModels::IATEsources::dummy::R() const
{
  return tmp<volScalarField>
  (
    new volScalarField
    (
      IOobject
      (
        "R",
        iate_.phase().U().time().timeName(),
        iate_.phase().mesh()
      ),
      iate_.phase().U().mesh(),
      dimensionedScalar("R", dimless/dimTime, 0)
    )
  );
}
