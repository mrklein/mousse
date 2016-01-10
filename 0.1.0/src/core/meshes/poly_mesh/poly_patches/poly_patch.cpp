// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "pstream.hpp"
#include "demand_driven_data.hpp"
#include "poly_patch.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "poly_boundary_mesh.hpp"
#include "poly_mesh.hpp"
#include "primitive_mesh.hpp"
#include "sub_field.hpp"
#include "entry.hpp"
#include "dictionary.hpp"
#include "point_patch_field.hpp"
// Static Data Members
namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(polyPatch, 0);

int polyPatch::disallowGenericPolyPatch
(
  debug::debugSwitch("disallowGenericPolyPatch", 0)
);

DEFINE_RUN_TIME_SELECTION_TABLE(polyPatch, word);
DEFINE_RUN_TIME_SELECTION_TABLE(polyPatch, dictionary);
ADD_TO_RUN_TIME_SELECTION_TABLE(polyPatch, polyPatch, word);
ADD_TO_RUN_TIME_SELECTION_TABLE(polyPatch, polyPatch, dictionary);

}

// Protected Member Functions
void mousse::polyPatch::movePoints(PstreamBuffers&, const pointField& p)
{
  primitivePatch::movePoints(p);
}
void mousse::polyPatch::updateMesh(PstreamBuffers&)
{
  primitivePatch::clearGeom();
  clearAddressing();
}
void mousse::polyPatch::clearGeom()
{
  primitivePatch::clearGeom();
}

// Constructors
mousse::polyPatch::polyPatch
(
  const word& name,
  const label size,
  const label start,
  const label index,
  const polyBoundaryMesh& bm,
  const word& patchType
)
:
  patchIdentifier(name, index),
  primitivePatch
  (
    faceSubList(bm.mesh().faces(), size, start),
    bm.mesh().points()
  ),
  start_(start),
  boundaryMesh_(bm),
  faceCellsPtr_(NULL),
  mePtr_(NULL)
{
  if
  (
    patchType != word::null
  && constraintType(patchType)
  && findIndex(inGroups(), patchType) == -1
  )
  {
    inGroups().append(patchType);
  }
}
mousse::polyPatch::polyPatch
(
  const word& name,
  const dictionary& dict,
  const label index,
  const polyBoundaryMesh& bm,
  const word& patchType
)
:
  patchIdentifier(name, dict, index),
  primitivePatch
  (
    faceSubList
    (
      bm.mesh().faces(),
      readLabel(dict.lookup("nFaces")),
      readLabel(dict.lookup("startFace"))
    ),
    bm.mesh().points()
  ),
  start_(readLabel(dict.lookup("startFace"))),
  boundaryMesh_(bm),
  faceCellsPtr_(NULL),
  mePtr_(NULL)
{
  if
  (
    patchType != word::null
  && constraintType(patchType)
  && findIndex(inGroups(), patchType) == -1
  )
  {
    inGroups().append(patchType);
  }
}
mousse::polyPatch::polyPatch
(
  const polyPatch& pp,
  const polyBoundaryMesh& bm
)
:
  patchIdentifier(pp),
  primitivePatch
  (
    faceSubList
    (
      bm.mesh().faces(),
      pp.size(),
      pp.start()
    ),
    bm.mesh().points()
  ),
  start_(pp.start()),
  boundaryMesh_(bm),
  faceCellsPtr_(NULL),
  mePtr_(NULL)
{}
mousse::polyPatch::polyPatch
(
  const polyPatch& pp,
  const polyBoundaryMesh& bm,
  const label index,
  const label newSize,
  const label newStart
)
:
  patchIdentifier(pp, index),
  primitivePatch
  (
    faceSubList
    (
      bm.mesh().faces(),
      newSize,
      newStart
    ),
    bm.mesh().points()
  ),
  start_(newStart),
  boundaryMesh_(bm),
  faceCellsPtr_(NULL),
  mePtr_(NULL)
{}
mousse::polyPatch::polyPatch
(
  const polyPatch& pp,
  const polyBoundaryMesh& bm,
  const label index,
  const labelUList& mapAddressing,
  const label newStart
)
:
  patchIdentifier(pp, index),
  primitivePatch
  (
    faceSubList
    (
      bm.mesh().faces(),
      mapAddressing.size(),
      newStart
    ),
    bm.mesh().points()
  ),
  start_(newStart),
  boundaryMesh_(bm),
  faceCellsPtr_(NULL),
  mePtr_(NULL)
{}
mousse::polyPatch::polyPatch(const polyPatch& p)
:
  patchIdentifier(p),
  primitivePatch(p),
  start_(p.start_),
  boundaryMesh_(p.boundaryMesh_),
  faceCellsPtr_(NULL),
  mePtr_(NULL)
{}
// Destructor
mousse::polyPatch::~polyPatch()
{
  clearAddressing();
}
// Member Functions
bool mousse::polyPatch::constraintType(const word& pt)
{
  return pointPatchField<scalar>::pointPatchConstructorTablePtr_->found(pt);
}
mousse::wordList mousse::polyPatch::constraintTypes()
{
  wordList cTypes(dictionaryConstructorTablePtr_->size());
  label i = 0;
  for
  (
    dictionaryConstructorTable::iterator cstrIter =
      dictionaryConstructorTablePtr_->begin();
    cstrIter != dictionaryConstructorTablePtr_->end();
    ++cstrIter
  )
  {
    if (constraintType(cstrIter.key()))
    {
      cTypes[i++] = cstrIter.key();
    }
  }
  cTypes.setSize(i);
  return cTypes;
}
const mousse::polyBoundaryMesh& mousse::polyPatch::boundaryMesh() const
{
  return boundaryMesh_;
}
const mousse::vectorField::subField mousse::polyPatch::faceCentres() const
{
  return patchSlice(boundaryMesh().mesh().faceCentres());
}
const mousse::vectorField::subField mousse::polyPatch::faceAreas() const
{
  return patchSlice(boundaryMesh().mesh().faceAreas());
}
// Return the patch face neighbour cell centres
mousse::tmp<mousse::vectorField> mousse::polyPatch::faceCellCentres() const
{
  tmp<vectorField> tcc(new vectorField(size()));
  vectorField& cc = tcc();
  // get reference to global cell centres
  const vectorField& gcc = boundaryMesh_.mesh().cellCentres();
  const labelUList& faceCells = this->faceCells();
  FOR_ALL(faceCells, facei)
  {
    cc[facei] = gcc[faceCells[facei]];
  }
  return tcc;
}
const mousse::labelUList& mousse::polyPatch::faceCells() const
{
  if (!faceCellsPtr_)
  {
    faceCellsPtr_ = new labelList::subList
    (
      patchSlice(boundaryMesh().mesh().faceOwner())
    );
  }
  return *faceCellsPtr_;
}
const mousse::labelList& mousse::polyPatch::meshEdges() const
{
  if (!mePtr_)
  {
    mePtr_ =
      new labelList
      (
        primitivePatch::meshEdges
        (
          boundaryMesh().mesh().edges(),
          boundaryMesh().mesh().pointEdges()
        )
      );
  }
  return *mePtr_;
}
void mousse::polyPatch::clearAddressing()
{
  primitivePatch::clearTopology();
  primitivePatch::clearPatchMeshAddr();
  deleteDemandDrivenData(faceCellsPtr_);
  deleteDemandDrivenData(mePtr_);
}
void mousse::polyPatch::write(Ostream& os) const
{
  os.writeKeyword("type") << type() << token::END_STATEMENT << nl;
  patchIdentifier::write(os);
  os.writeKeyword("nFaces") << size() << token::END_STATEMENT << nl;
  os.writeKeyword("startFace") << start() << token::END_STATEMENT << nl;
}
void mousse::polyPatch::initOrder(PstreamBuffers&, const primitivePatch&) const
{}
bool mousse::polyPatch::order
(
  PstreamBuffers&,
  const primitivePatch&,
  labelList& /*faceMap*/,
  labelList& /*rotation*/
) const
{
  // Nothing changed.
  return false;
}

// Member Operators
void mousse::polyPatch::operator=(const polyPatch& p)
{
  clearAddressing();
  patchIdentifier::operator=(p);
  primitivePatch::operator=(p);
  start_ = p.start_;
}

// Friend Operators
mousse::Ostream& mousse::operator<<(Ostream& os, const polyPatch& p)
{
  p.write(os);
  os.check("Ostream& operator<<(Ostream& os, const polyPatch& p");
  return os;
}
