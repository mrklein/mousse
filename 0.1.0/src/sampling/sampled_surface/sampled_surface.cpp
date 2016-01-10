// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "sampled_surface.hpp"
#include "poly_mesh.hpp"
#include "demand_driven_data.hpp"
// Static Data Members
namespace mousse
{
  DEFINE_TYPE_NAME_AND_DEBUG(sampledSurface, 0);
  DEFINE_RUN_TIME_SELECTION_TABLE(sampledSurface, word);
}
// Private Member Functions 
void mousse::sampledSurface::clearGeom() const
{
  deleteDemandDrivenData(SfPtr_);
  deleteDemandDrivenData(magSfPtr_);
  deleteDemandDrivenData(CfPtr_);
  area_ = -1;
}
void mousse::sampledSurface::makeSf() const
{
  // It is an error to recalculate if the pointer is already set
  if (SfPtr_)
  {
    FATAL_ERROR_IN("mousse::sampledSurface::makeSf()")
      << "face area vectors already exist"
      << abort(FatalError);
  }
  const faceList& theFaces = faces();
  SfPtr_ = new vectorField(theFaces.size());
  vectorField& values = *SfPtr_;
  FOR_ALL(theFaces, faceI)
  {
    values[faceI] = theFaces[faceI].normal(points());
  }
}
void mousse::sampledSurface::makeMagSf() const
{
  // It is an error to recalculate if the pointer is already set
  if (magSfPtr_)
  {
    FATAL_ERROR_IN("mousse::sampledSurface::makeMagSf()")
      << "mag face areas already exist"
      << abort(FatalError);
  }
  const faceList& theFaces = faces();
  magSfPtr_ = new scalarField(theFaces.size());
  scalarField& values = *magSfPtr_;
  FOR_ALL(theFaces, faceI)
  {
    values[faceI] = theFaces[faceI].mag(points());
  }
}
void mousse::sampledSurface::makeCf() const
{
  // It is an error to recalculate if the pointer is already set
  if (CfPtr_)
  {
    FATAL_ERROR_IN("mousse::sampledSurface::makeCf()")
      << "face centres already exist"
      << abort(FatalError);
  }
  const faceList& theFaces = faces();
  CfPtr_ = new vectorField(theFaces.size());
  vectorField& values = *CfPtr_;
  FOR_ALL(theFaces, faceI)
  {
    values[faceI] = theFaces[faceI].centre(points());
  }
}
// Selectors
mousse::autoPtr<mousse::sampledSurface> mousse::sampledSurface::New
(
  const word& name,
  const polyMesh& mesh,
  const dictionary& dict
)
{
  const word sampleType(dict.lookup("type"));
  if (debug)
  {
    Info<< "Selecting sampledType " << sampleType << endl;
  }
  wordConstructorTable::iterator cstrIter =
    wordConstructorTablePtr_->find(sampleType);
  if (cstrIter == wordConstructorTablePtr_->end())
  {
    FATAL_ERROR_IN
    (
      "sampledSurface::New"
      "(const word&, const polyMesh&, const dictionary&)"
    )   << "Unknown sample type "
      << sampleType << nl << nl
      << "Valid sample types : " << endl
      << wordConstructorTablePtr_->sortedToc()
      << exit(FatalError);
  }
  return autoPtr<sampledSurface>(cstrIter()(name, mesh, dict));
}
// Constructors 
mousse::sampledSurface::sampledSurface
(
  const word& name,
  const polyMesh& mesh,
  const bool interpolate
)
:
  name_(name),
  mesh_(mesh),
  interpolate_(interpolate),
  SfPtr_(NULL),
  magSfPtr_(NULL),
  CfPtr_(NULL),
  area_(-1)
{}
mousse::sampledSurface::sampledSurface
(
  const word& name,
  const polyMesh& mesh,
  const dictionary& dict
)
:
  name_(name),
  mesh_(mesh),
  interpolate_(dict.lookupOrDefault("interpolate", false)),
  SfPtr_(NULL),
  magSfPtr_(NULL),
  CfPtr_(NULL),
  area_(-1)
{
  dict.readIfPresent("name", name_);
}
// Destructor 
mousse::sampledSurface::~sampledSurface()
{
  clearGeom();
}
// Member Functions 
const mousse::vectorField& mousse::sampledSurface::Sf() const
{
  if (!SfPtr_)
  {
    makeSf();
  }
  return *SfPtr_;
}
const mousse::scalarField& mousse::sampledSurface::magSf() const
{
  if (!magSfPtr_)
  {
    makeMagSf();
  }
  return *magSfPtr_;
}
const mousse::vectorField& mousse::sampledSurface::Cf() const
{
  if (!CfPtr_)
  {
    makeCf();
  }
  return *CfPtr_;
}
mousse::scalar mousse::sampledSurface::area() const
{
  if (area_ < 0)
  {
    area_ = sum(magSf());
    reduce(area_, sumOp<scalar>());
  }
  return area_;
}
mousse::tmp<mousse::scalarField> mousse::sampledSurface::sample
(
  const surfaceScalarField& /*sField*/
) const
{
  NOT_IMPLEMENTED("tmp<mousse::scalarField> sampledSurface::sample");
  return tmp<scalarField>{NULL};
}
mousse::tmp<mousse::vectorField> mousse::sampledSurface::sample
(
  const surfaceVectorField& /*sField*/
) const
{
  NOT_IMPLEMENTED("tmp<mousse::vectorField> sampledSurface::sample");
  return tmp<vectorField>{NULL};
}
mousse::tmp<mousse::sphericalTensorField> mousse::sampledSurface::sample
(
  const surfaceSphericalTensorField& /*sField*/
) const
{
  NOT_IMPLEMENTED("tmp<mousse::sphericalTensorField> sampledSurface::sample");
  return tmp<sphericalTensorField>{NULL};
}
mousse::tmp<mousse::symmTensorField> mousse::sampledSurface::sample
(
  const surfaceSymmTensorField& /*sField*/
) const
{
  NOT_IMPLEMENTED("tmp<mousse::symmTensorField> sampledSurface::sample");
  return tmp<symmTensorField>{NULL};
}
mousse::tmp<mousse::tensorField> mousse::sampledSurface::sample
(
  const surfaceTensorField& /*sField*/
) const
{
  NOT_IMPLEMENTED("tmp<mousse::tensorField> sampledSurface::sample");
  return tmp<tensorField>{NULL};
}
mousse::tmp<mousse::Field<mousse::scalar> >
mousse::sampledSurface::project(const Field<scalar>& field) const
{
  tmp<Field<scalar> > tRes(new Field<scalar>(faces().size()));
  Field<scalar>& res = tRes();
  FOR_ALL(faces(), faceI)
  {
    res[faceI] = field[faceI];
  }
  return tRes;
}
mousse::tmp<mousse::Field<mousse::scalar> >
mousse::sampledSurface::project(const Field<vector>& field) const
{
  tmp<Field<scalar> > tRes(new Field<scalar>(faces().size()));
  project(tRes(), field);
  return tRes;
}
mousse::tmp<mousse::Field<mousse::vector> >
mousse::sampledSurface::project(const Field<sphericalTensor>& field) const
{
  tmp<Field<vector> > tRes(new Field<vector>(faces().size()));
  project(tRes(), field);
  return tRes;
}
mousse::tmp<mousse::Field<mousse::vector> >
mousse::sampledSurface::project(const Field<symmTensor>& field) const
{
  tmp<Field<vector> > tRes(new Field<vector>(faces().size()));
  project(tRes(), field);
  return tRes;
}
mousse::tmp<mousse::Field<mousse::vector> >
mousse::sampledSurface::project(const Field<tensor>& field) const
{
  tmp<Field<vector> > tRes(new Field<vector>(faces().size()));
  project(tRes(), field);
  return tRes;
}
void mousse::sampledSurface::print(Ostream& os) const
{
  os << type();
}
// Friend Operators 
mousse::Ostream& mousse::operator<<(Ostream &os, const sampledSurface& s)
{
  s.print(os);
  os.check("Ostream& operator<<(Ostream&, const sampledSurface&");
  return os;
}
