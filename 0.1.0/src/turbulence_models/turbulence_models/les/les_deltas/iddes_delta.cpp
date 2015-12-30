// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "iddes_delta.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "wall_dist.hpp"
// Static Data Members
namespace mousse
{
namespace LESModels
{
  defineTypeNameAndDebug(IDDESDelta, 0);
  addToRunTimeSelectionTable(LESdelta, IDDESDelta, dictionary);
}
}
// Private Member Functions 
void mousse::LESModels::IDDESDelta::calcDelta()
{
  const volScalarField& hmax = hmax_;
  const fvMesh& mesh = turbulenceModel_.mesh();
  // Wall-normal vectors
  const volVectorField& n = wallDist::New(mesh).n();
  tmp<volScalarField> tfaceToFacenMax
  (
    new volScalarField
    (
      IOobject
      (
        "faceToFaceMax",
        mesh.time().timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::NO_WRITE
      ),
      mesh,
      dimensionedScalar("zero", dimLength, 0.0)
    )
  );
  scalarField& faceToFacenMax = tfaceToFacenMax().internalField();
  const cellList& cells = mesh.cells();
  const vectorField& faceCentres = mesh.faceCentres();
  forAll(cells, celli)
  {
    scalar maxDelta = 0.0;
    const labelList& cFaces = cells[celli];
    const vector nci = n[celli];
    forAll(cFaces, cFacei)
    {
      label facei = cFaces[cFacei];
      const point& fci = faceCentres[facei];
      forAll(cFaces, cFacej)
      {
        label facej = cFaces[cFacej];
        const point& fcj = faceCentres[facej];
        scalar ndfc = nci & (fcj - fci);
        if (ndfc > maxDelta)
        {
          maxDelta = ndfc;
        }
      }
    }
    faceToFacenMax[celli] = maxDelta;
  }
  label nD = mesh.nGeometricD();
  if (nD == 2)
  {
    WarningIn("IDDESDelta::calcDelta()")
      << "Case is 2D, LES is not strictly applicable" << nl
      << endl;
  }
  else if (nD != 3)
  {
    FatalErrorIn("IDDESDelta::calcDelta()")
      << "Case must be either 2D or 3D" << exit(FatalError);
  }
  delta_.internalField() =
    min
    (
      max
      (
        max
        (
          Cw_*wallDist::New(mesh).y(),
          Cw_*hmax
        ),
        tfaceToFacenMax
      ),
      hmax
    );
}
// Constructors 
mousse::LESModels::IDDESDelta::IDDESDelta
(
  const word& name,
  const turbulenceModel& turbulence,
  const dictionary& dict
)
:
  LESdelta(name, turbulence),
  hmax_
  (
    IOobject::groupName("hmax", turbulence.U().group()),
    turbulence,
    dict
  ),
  Cw_
  (
    dict.subDict(type() + "Coeffs").lookupOrDefault<scalar>("Cw", 0.15)
  )
{
  calcDelta();
}
// Member Functions 
void mousse::LESModels::IDDESDelta::read(const dictionary& dict)
{
  const dictionary& coeffsDict(dict.subDict(type() + "Coeffs"));
  coeffsDict.readIfPresent<scalar>("Cw", Cw_);
  calcDelta();
}
void mousse::LESModels::IDDESDelta::correct()
{
  if (turbulenceModel_.mesh().changing())
  {
    hmax_.correct();
    calcDelta();
  }
}
