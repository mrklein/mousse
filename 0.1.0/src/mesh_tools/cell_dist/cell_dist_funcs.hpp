#ifndef MESH_TOOLS_CELL_DIST_CELL_DIST_FUNCS_HPP_
#define MESH_TOOLS_CELL_DIST_CELL_DIST_FUNCS_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::cellDistFuncs
// Description
//   Collection of functions used in wall distance calculation.
// SourceFiles
//   cell_dist_funcs.cpp
//   cell_dist_funcs_templates.cpp


#include "scalar_field.hpp"
#include "hash_set.hpp"
#include "map.hpp"
#include "word_re_list.hpp"
#include "scalar_field.hpp"
#include "point.hpp"
#include "primitive_patch.hpp"
#include "class_name.hpp"

namespace mousse
{

// Forward declaration of classes
class polyMesh;
class polyPatch;
class polyBoundaryMesh;

class cellDistFuncs
{
  // Private Member Data

    //- Reference to mesh
    const polyMesh& mesh_;

  // Private Member Functions

    //- Search for element in first n elements of labelList. Return index
    //  or -1.
    static label findIndex(const label n, const labelList&, const label);

public:

  CLASS_NAME("cellDistFuncs");

  // Constructors

    //- Construct from mesh
    cellDistFuncs(const polyMesh& mesh);

    //- Disallow default bitwise copy construct
    cellDistFuncs(const cellDistFuncs&) = delete;

    //- Disallow default bitwise assignment
    cellDistFuncs& operator=(const cellDistFuncs&) = delete;

  // Member Functions

    //- Access mesh
    const polyMesh& mesh() const
    {
      return mesh_;
    }

    //- Return the set of patch IDs corresponding to the given names
    labelHashSet getPatchIDs(const wordReList& patchNames) const;

    //- Get patchIDs of/derived off certain type (e.g. 'processorPolyPatch')
    //  Uses isA, not isType
    template<class Type>
    labelHashSet getPatchIDs() const;

    //- Calculate smallest true distance (and face index)
    //  from pt to faces wallFaces.
    //  For efficiency reasons we still pass in patch instead of extracting
    //  it from mesh_
    scalar smallestDist
    (
      const point& p,
      const polyPatch& patch,
      const label nWallFaces,
      const labelList& wallFaces,
      label& meshFaceI
    ) const;

    //- Get faces sharing point with face on patch
    label getPointNeighbours
    (
      const primitivePatch&,
      const label patchFaceI,
      labelList&
    ) const;

    //- Size of largest patch (out of supplied subset of patches)
    label maxPatchSize(const labelHashSet& patchIDs) const;

    //- Sum of patch sizes (out of supplied subset of patches).
    //  Used in sizing arrays.
    label sumPatchSize(const labelHashSet& patchIDs) const;

    //- Correct all cells connected to boundary (via face). Sets values in
    //  wallDistCorrected. Sets nearest wallface in nearestFace.
    void correctBoundaryFaceCells
    (
      const labelHashSet& patchIDs,
      scalarField& wallDistCorrected,
      Map<label>& nearestFace
    ) const;

    //- Correct all cells connected to wall (via point). Sets values in
    //  wallDistCorrected. Uses/sets nearest wallFace in nearestFace.
    void correctBoundaryPointCells
    (
      const labelHashSet& patchIDs,
      scalarField& wallDistCorrected,
      Map<label>& nearestFace
    ) const;

};

}  // namespace mousse
#ifdef NoRepository
#   include "cell_dist_funcs_templates.cpp"
#endif
#endif
