#ifndef FINITE_VOLUME_FINITE_VOLUME_SN_GRAD_SCHEMES_CENTRED_FIT_SN_GRAD_DATA_HPP_
#define FINITE_VOLUME_FINITE_VOLUME_SN_GRAD_SCHEMES_CENTRED_FIT_SN_GRAD_DATA_HPP_

// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::CentredFitSnGradData
// Description
//   Data for centred fit snGrad schemes

#include "fit_data.hpp"


namespace mousse {

class extendedCentredCellToFaceStencil;

template<class Polynomial>
class CentredFitSnGradData
:
  public FitData
  <
    CentredFitSnGradData<Polynomial>,
    extendedCentredCellToFaceStencil,
    Polynomial
  >
{
  // Private data
    //- For each cell in the mesh store the values which multiply the
    //  values of the stencil to obtain the gradient for each direction
    List<scalarList> coeffs_;
public:
  TYPE_NAME("CentredFitSnGradData");

  // Constructors
    //- Construct from components
    CentredFitSnGradData
    (
      const fvMesh& mesh,
      const extendedCentredCellToFaceStencil& stencil,
      const scalar linearLimitFactor,
      const scalar centralWeight
    );

  //- Destructor
  virtual ~CentredFitSnGradData()
  {}

  // Member functions
    //- Return reference to fit coefficients
    const List<scalarList>& coeffs() const
    {
      return coeffs_;
    }

    //- Calculate the fit for the specified face and set the coefficients
    void calcFit
    (
      scalarList& coeffsi, // coefficients to be set
      const List<point>&,  // Stencil points
      const scalar wLin,   // Weight for linear approximation (weights
                           // nearest neighbours)
      const scalar deltaCoeff, // uncorrected delta coefficient
      const label faci     // Current face index
    );
    void calcFit();
};

}  // namespace mousse

#include "centred_fit_sn_grad_data.ipp"

#endif
