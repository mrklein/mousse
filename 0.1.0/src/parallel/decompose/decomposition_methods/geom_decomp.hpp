// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::geomDecomp
// Description
//   Geometrical domain decomposition
// SourceFiles
//   geom_decomp.cpp
#ifndef geom_decomp_hpp_
#define geom_decomp_hpp_
#include "decomposition_method.hpp"
#include "vector.hpp"
namespace mousse
{
class geomDecomp
:
  public decompositionMethod
{
protected:
  // Protected data
    const dictionary& geomDecomDict_;
    Vector<label> n_;
    scalar delta_;
    tensor rotDelta_;
public:
  // Constructors
    //- Construct given the decomposition dictionary
    //  and the derived type name
    geomDecomp
    (
      const dictionary& decompositionDict,
      const word& derivedType
    );
    //- Return for every coordinate the wanted processor number.
    virtual labelList decompose
    (
      const pointField& points,
      const scalarField& pointWeights
    ) = 0;
    //- Like decompose but with uniform weights on the points
    virtual labelList decompose(const pointField&) = 0;
};
}  // namespace mousse
#endif
