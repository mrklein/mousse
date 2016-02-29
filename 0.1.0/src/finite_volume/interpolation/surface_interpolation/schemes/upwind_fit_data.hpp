#ifndef FINITE_VOLUME_INTERPOLATION_SURFACE_INTERPOLATION_SCHEMES_UPWIND_FIT_DATA_HPP_
#define FINITE_VOLUME_INTERPOLATION_SURFACE_INTERPOLATION_SCHEMES_UPWIND_FIT_DATA_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::UpwindFitData
// Description
//   Data for the quadratic fit correction interpolation scheme to be used with
//   upwind biased stencil.
//   - linearCorrection = true : fit calculated for corrected linear scheme
//   - linearCorrection = false : fit calculated for corrected upwind scheme
// SourceFiles
//   upwind_fit_data.cpp
//

#include "fit_data.hpp"

namespace mousse
{
class extendedUpwindCellToFaceStencil;
template<class Polynomial>
class UpwindFitData
:
  public FitData
  <
    UpwindFitData<Polynomial>,
    extendedUpwindCellToFaceStencil,
    Polynomial
  >
{
  // Private data
    //- For each face of the mesh store the coefficients to multiply the
    //  stencil cell values by if the flow is from the owner
    List<scalarList> owncoeffs_;
    //- For each face of the mesh store the coefficients to multiply the
    //  stencil cell values by if the flow is from the neighbour
    List<scalarList> neicoeffs_;
  // Private Member Functions
    //- Calculate the fit for the all the mesh faces
    //  and set the coefficients
    void calcFit();
public:
  TYPE_NAME("UpwindFitData");
  // Constructors
    //- Construct from components
    UpwindFitData
    (
      const fvMesh& mesh,
      const extendedUpwindCellToFaceStencil& stencil,
      const bool linearCorrection,
      const scalar linearLimitFactor,
      const scalar centralWeight
    );
  //- Destructor
  virtual ~UpwindFitData()
  {}
  // Member functions
    //- Return reference to owner fit coefficients
    const List<scalarList>& owncoeffs() const
    {
      return owncoeffs_;
    }
    //- Return reference to neighbour fit coefficients
    const List<scalarList>& neicoeffs() const
    {
      return neicoeffs_;
    }
};
}  // namespace mousse
#ifdef NoRepository
#   include "upwind_fit_data.cpp"
#endif
#endif
