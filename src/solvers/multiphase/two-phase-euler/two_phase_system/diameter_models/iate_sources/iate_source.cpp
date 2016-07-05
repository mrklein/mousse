// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "iate_source.hpp"
#include "two_phase_system.hpp"
#include "fv_matrix.hpp"
#include "phase_compressible_turbulence_model.hpp"
#include "uniform_dimensioned_fields.hpp"
// Static Data Members
namespace mousse
{
namespace diameterModels
{
DEFINE_TYPE_NAME_AND_DEBUG(IATEsource, 0);
DEFINE_RUN_TIME_SELECTION_TABLE(IATEsource, dictionary);
}
}
// Selectors
mousse::autoPtr<mousse::diameterModels::IATEsource>
mousse::diameterModels::IATEsource::New
(
  const word& type,
  const IATE& iate,
  const dictionary& dict
)
{
  dictionaryConstructorTable::iterator cstrIter =
    dictionaryConstructorTablePtr_->find(type);
  if (cstrIter == dictionaryConstructorTablePtr_->end())
  {
    FATAL_ERROR_IN
    (
      "IATEsource::New"
      "(const word& type, const IATE&, const dictionary&)"
    )
    << "Unknown IATE source type "
    << type << nl << nl
    << "Valid IATE source types : " << endl
    << dictionaryConstructorTablePtr_->sortedToc()
    << exit(FatalError);
  }
  return autoPtr<IATEsource>(cstrIter()(iate, dict));
}
// Member Functions 
mousse::tmp<mousse::volScalarField> mousse::diameterModels::IATEsource::Ur() const
{
  const uniformDimensionedVectorField& g =
    phase().U().db().lookupObject<uniformDimensionedVectorField>("g");
  return
    sqrt(2.0)*pow025
    (
      fluid().sigma()*mag(g)*(otherPhase().rho()
                              - phase().rho())/sqr(otherPhase().rho())
    )*pow(max(1 - phase(), scalar(0)), 1.75);
}
mousse::tmp<mousse::volScalarField> mousse::diameterModels::IATEsource::Ut() const
{
  return sqrt(2*otherPhase().turbulence().k());
}
mousse::tmp<mousse::volScalarField> mousse::diameterModels::IATEsource::Re() const
{
  return max(Ur()*phase().d()/otherPhase().nu(), scalar(1.0e-3));
}
mousse::tmp<mousse::volScalarField> mousse::diameterModels::IATEsource::CD() const
{
  const volScalarField Eo{this->Eo()};
  const volScalarField Re{this->Re()};
  return
    max
    (
      min
      (
        (16/Re)*(1 + 0.15*pow(Re, 0.687)),
        48/Re
      ),
      8*Eo/(3*(Eo + 4))
    );
}
mousse::tmp<mousse::volScalarField> mousse::diameterModels::IATEsource::Mo() const
{
  const uniformDimensionedVectorField& g =
    phase().U().db().lookupObject<uniformDimensionedVectorField>("g");
  return
    mag(g)*pow4(otherPhase().nu())*sqr(otherPhase().rho())
    *(otherPhase().rho() - phase().rho())/pow3(fluid().sigma());
}
mousse::tmp<mousse::volScalarField> mousse::diameterModels::IATEsource::Eo() const
{
  const uniformDimensionedVectorField& g =
    phase().U().db().lookupObject<uniformDimensionedVectorField>("g");
  return
    mag(g)*sqr(phase().d())*(otherPhase().rho() - phase().rho())/fluid().sigma();
}
mousse::tmp<mousse::volScalarField> mousse::diameterModels::IATEsource::We() const
{
  return otherPhase().rho()*sqr(Ur())*phase().d()/fluid().sigma();
}
