// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::surfaceInterpolation
// Description
//   Cell to surface interpolation scheme. Included in fvMesh.
// SourceFiles
//   surface_interpolation.cpp
#ifndef surface_interpolation_hpp_
#define surface_interpolation_hpp_
#include "tmp.hpp"
#include "scalar.hpp"
#include "vol_fields_fwd.hpp"
#include "surface_fields_fwd.hpp"
#include "class_name.hpp"
namespace mousse
{
class surfaceInterpolation
{
  // Private data
    // Reference to fvMesh
    const fvMesh& mesh_;
    // Demand-driven data
      //- Linear difference weighting factors
      mutable surfaceScalarField* weights_;
      //- Cell-centre difference coefficients
      mutable surfaceScalarField* deltaCoeffs_;
      //- Non-orthogonal cell-centre difference coefficients
      mutable surfaceScalarField* nonOrthDeltaCoeffs_;
      //- Non-orthogonality correction vectors
      mutable surfaceVectorField* nonOrthCorrectionVectors_;
  // Private Member Functions
    //- Construct central-differencing weighting factors
    void makeWeights() const;
    //- Construct face-gradient difference factors
    void makeDeltaCoeffs() const;
    //- Construct face-gradient difference factors
    void makeNonOrthDeltaCoeffs() const;
    //- Construct non-orthogonality correction vectors
    void makeNonOrthCorrectionVectors() const;
protected:
  // Protected Member Functions
    // Storage management
      //- Clear all geometry and addressing
      void clearOut();
public:
  // Declare name of the class and its debug switch
  CLASS_NAME("surfaceInterpolation");
  // Constructors
    //- Construct given an fvMesh
    explicit surfaceInterpolation(const fvMesh&);
  //- Destructor
  ~surfaceInterpolation();
  // Member functions
    //- Return reference to linear difference weighting factors
    const surfaceScalarField& weights() const;
    //- Return reference to cell-centre difference coefficients
    const surfaceScalarField& deltaCoeffs() const;
    //- Return reference to non-orthogonal cell-centre difference
    //  coefficients
    const surfaceScalarField& nonOrthDeltaCoeffs() const;
    //- Return reference to non-orthogonality correction vectors
    const surfaceVectorField& nonOrthCorrectionVectors() const;
    //- Do what is neccessary if the mesh has moved
    bool movePoints();
};
}  // namespace mousse
#endif
