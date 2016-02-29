#ifndef MESH_TOOLS_CELL_QUALITY_CELL_QUALITY_HPP_
#define MESH_TOOLS_CELL_QUALITY_CELL_QUALITY_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::cellQuality
// Description
//   Class calculates cell quality measures.
// SourceFiles
//   cell_quality.cpp
#include "poly_mesh.hpp"
namespace mousse
{
class cellQuality
{
  // Private data
    //- Reference to mesh
    const polyMesh& mesh_;
public:
  // Constructors
    //- Construct from mesh
    cellQuality(const polyMesh& mesh);
    //- Disallow default bitwise copy construct
    cellQuality(const cellQuality&) = delete;
    //- Disallow default bitwise assignment
    cellQuality& operator=(const cellQuality&) = delete;
  // Destructor - default
  // Member Functions
    //- Return cell non-orthogonality
    tmp<scalarField> nonOrthogonality() const;
    //- Return cell skewness
    tmp<scalarField> skewness() const;
    //- Return face non-orthogonality
    tmp<scalarField> faceNonOrthogonality() const;
    //- Return face skewness
    tmp<scalarField> faceSkewness() const;
};
}  // namespace mousse
#endif
