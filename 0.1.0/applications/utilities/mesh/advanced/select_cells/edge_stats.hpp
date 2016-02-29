#ifndef UTILITIES_MESH_ADVANCED_MODIFY_MESH_EDGE_STATS_HPP_
#define UTILITIES_MESH_ADVANCED_MODIFY_MESH_EDGE_STATS_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::edgeStats
// Description
//   Helper class to calculate minimum edge length on mesh.
// SourceFiles
//   edge_stats.cpp

#include "direction.hpp"
#include "scalar.hpp"

namespace mousse
{
// Forward declaration of classes
class polyMesh;
class Ostream;
class twoDPointCorrector;
class edgeStats
{
  // Private data
    //- Reference to mesh.
    const polyMesh& mesh_;
    //- Component (0,1,2) of normal direction or 3 if 3D case.
    direction normalDir_;
  // Private Member Functions
    //- If 2d get component of normal dir.
    direction getNormalDir(const twoDPointCorrector*) const;
public:
  // Static data members
    // Max (cos of) angle for edges to be considered aligned with axis.
    static const scalar edgeTol_;
  // Constructors
    //- Construct from mesh
    edgeStats(const polyMesh& mesh);
    //- Construct from mesh and corrector
    edgeStats(const polyMesh& mesh, const twoDPointCorrector* );
    //- Disallow default bitwise copy construct
    edgeStats(const edgeStats&) = delete;
    //- Disallow default bitwise assignment
    void operator=(const edgeStats&) = delete;
  // Member Functions
    //- Calculate minimum edge length and print
    scalar minLen(Ostream& os) const;
};
}  // namespace mousse
#endif
