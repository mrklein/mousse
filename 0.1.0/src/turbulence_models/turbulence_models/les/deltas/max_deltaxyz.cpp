// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "max_deltaxyz.hpp"
#include "add_to_run_time_selection_table.hpp"


// Static Data Members
namespace mousse {
namespace LESModels {

DEFINE_TYPE_NAME_AND_DEBUG(maxDeltaxyz, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE(LESdelta, maxDeltaxyz, dictionary);

}
}


// Private Member Functions 
void mousse::LESModels::maxDeltaxyz::calcDelta()
{
  const fvMesh& mesh = turbulenceModel_.mesh();
  label nD = mesh.nGeometricD();
  tmp<volScalarField> hmax
  {
    new volScalarField
    {
      {
        "hmax",
        mesh.time().timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::NO_WRITE
      },
      mesh,
      {"hmax", dimLength, 0.0}
    }
  };
  const cellList& cells = mesh.cells();
  FOR_ALL(cells,cellI) {
    scalar deltaMaxTmp = 0.0;
    const labelList& cFaces = mesh.cells()[cellI];
    const point& centrevector = mesh.cellCentres()[cellI];
    FOR_ALL(cFaces, cFaceI) {
      label faceI = cFaces[cFaceI];
      const point& facevector = mesh.faceCentres()[faceI];
      scalar tmp = mag(facevector - centrevector);
      if (tmp > deltaMaxTmp) {
        deltaMaxTmp = tmp;
      }
    }
    hmax()[cellI] = deltaCoeff_*deltaMaxTmp;
  }
  if (nD == 3) {
    delta_.internalField() = hmax();
  } else if (nD == 2) {
    WARNING_IN("maxDeltaxyz::calcDelta()")
      << "Case is 2D, LES is not strictly applicable\n"
      << endl;
    delta_.internalField() = hmax();
  } else {
    FATAL_ERROR_IN("maxDeltaxyz::calcDelta()")
      << "Case is not 3D or 2D, LES is not applicable"
      << exit(FatalError);
  }
}


// Constructors 
mousse::LESModels::maxDeltaxyz::maxDeltaxyz
(
  const word& name,
  const turbulenceModel& turbulence,
  const dictionary& dict
)
:
  LESdelta{name, turbulence},
  deltaCoeff_
  {
    dict.subDict(type() + "Coeffs").lookupOrDefault<scalar>("deltaCoeff", 1)
  }
{
  calcDelta();
}


// Member Functions 
void mousse::LESModels::maxDeltaxyz::read(const dictionary& dict)
{
  const dictionary& coeffsDict(dict.subDict(type() + "Coeffs"));
  coeffsDict.readIfPresent<scalar>("deltaCoeff", deltaCoeff_);
  calcDelta();
}


void mousse::LESModels::maxDeltaxyz::correct()
{
  if (!turbulenceModel_.mesh().changing()) {
    return;
  }
  calcDelta();
}

