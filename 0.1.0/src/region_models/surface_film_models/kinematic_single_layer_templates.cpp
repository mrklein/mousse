// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "kinematic_single_layer.hpp"
namespace mousse
{
namespace regionModels
{
namespace surfaceFilmModels
{
// Protected Member Functions 
template<class Type>
void kinematicSingleLayer::constrainFilmField
(
  Type& field,
  const typename Type::cmptType& value
)
{
  FOR_ALL(intCoupledPatchIDs_, i)
  {
    label patchI = intCoupledPatchIDs_[i];
    field.boundaryField()[patchI] = value;
    if (debug)
    {
      Info<< "Constraining " << field.name()
        << " boundary " << field.boundaryField()[patchI].patch().name()
        << " to " << value << endl;
    }
  }
  FOR_ALL(passivePatchIDs_, i)
  {
    label patchI = passivePatchIDs_[i];
    field.boundaryField()[patchI] = value;
    if (debug)
    {
      Info<< "Constraining " << field.name()
        << " boundary " << field.boundaryField()[patchI].patch().name()
        << " to " << value << endl;
    }
  }
}
} // end namespace mousse
} // end namespace regionModels
} // end namespace surfaceFilmModels
