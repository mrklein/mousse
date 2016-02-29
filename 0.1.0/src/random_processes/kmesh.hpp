#ifndef RANDOM_PROCESSES_KMESH_HPP_
#define RANDOM_PROCESSES_KMESH_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::Kmesh
// Description
//   Calculate the wavenumber vector field corresponding to the
//   space vector field of a finite volume mesh;
// SourceFiles
//   kmesh.cpp
#include "fv_mesh.hpp"
namespace mousse
{
class Kmesh
:
  public vectorField
{
  // Private data
    //- Dimensions of box
    vector l_;
    //- Multi-dimensional addressing array
    labelList nn_;
    //- Maximum wavenumber
    scalar kmax_;
  // Private functions
    //- Return the linear index from the i-j-k indices
    static inline label index
    (
      const label i,
      const label j,
      const label k,
      const labelList& nn
    );
public:
  // Constructors
    //- Construct from fvMesh
    Kmesh(const fvMesh&);
  // Member Functions
    // Access
    const vector& sizeOfBox() const
    {
      return l_;
    }
    const labelList& nn() const
    {
      return nn_;
    }
    scalar max() const
    {
      return kmax_;
    }
};
}  // namespace mousse
#endif
