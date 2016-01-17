// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "field_from_file.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "tri_surface_mesh.hpp"
#include "tri_surface_fields.hpp"
#include "time.hpp"
// Static Data Members
namespace mousse
{
  defineTypeNameAndDebug(fieldFromFile, 0);
  addToRunTimeSelectionTable
  (
    cellSizeCalculationType,
    fieldFromFile,
    dictionary
  );
}
// Constructors 
mousse::fieldFromFile::fieldFromFile
(
  const dictionary& cellSizeCalcTypeDict,
  const triSurfaceMesh& surface,
  const scalar& defaultCellSize
)
:
  cellSizeCalculationType
  (
    typeName,
    cellSizeCalcTypeDict,
    surface,
    defaultCellSize
  ),
  coeffsDict_(cellSizeCalcTypeDict.subDict(typeName + "Coeffs")),
  fileName_
  (
    cellSizeCalcTypeDict.subDict(typeName + "Coeffs").lookup("fieldFile")
  ),
  cellSizeMultipleCoeff_
  (
    coeffsDict_.lookupOrDefault<scalar>("cellSizeMultipleCoeff", 1)
  )
{}
// Member Functions 
mousse::tmp<mousse::triSurfacePointScalarField> mousse::fieldFromFile::load()
{
  Info<< indent << "Loading: " << fileName_ << endl;
  tmp<triSurfacePointScalarField> pointCellSize
  (
    new triSurfacePointScalarField
    (
      IOobject
      (
        fileName_,
        surface_.searchableSurface::time().constant(),
        "triSurface",
        surface_.searchableSurface::time(),
        IOobject::MUST_READ,
        IOobject::NO_WRITE
      ),
      surface_,
      dimLength,
      true
    )
  );
  pointCellSize() *= cellSizeMultipleCoeff_;
  return pointCellSize;
}
