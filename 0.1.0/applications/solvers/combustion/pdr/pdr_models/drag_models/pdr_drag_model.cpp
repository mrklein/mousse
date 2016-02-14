// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "pdr_drag_model.hpp"
// Static Data Members
namespace mousse
{
DEFINE_TYPE_NAME_AND_DEBUG(PDRDragModel, 0);
DEFINE_RUN_TIME_SELECTION_TABLE(PDRDragModel, dictionary);
};
// Constructors 
mousse::PDRDragModel::PDRDragModel
(
  const dictionary& PDRProperties,
  const compressible::RASModel& turbulence,
  const volScalarField& rho,
  const volVectorField& U,
  const surfaceScalarField& phi
)
:
  regIOobject
  {
    {
      "PDRDragModel",
      U.time().constant(),
      U.db()
    }
  },
  PDRDragModelCoeffs_
  {
    PDRProperties.subDict
    (
      word{PDRProperties.lookup("PDRDragModel")} + "Coeffs"
    )
  },
  turbulence_{turbulence},
  rho_{rho},
  U_{U},
  phi_{phi},
  on_{PDRDragModelCoeffs_.lookup("drag")}
{}
// Destructor 
mousse::PDRDragModel::~PDRDragModel()
{}
// Member Functions 
bool mousse::PDRDragModel::read(const dictionary& PDRProperties)
{
  PDRDragModelCoeffs_ = PDRProperties.subDict(type() + "Coeffs");
  PDRDragModelCoeffs_.lookup("drag") >> on_;
  return true;
}
