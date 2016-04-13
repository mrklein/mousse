#ifndef FINITE_VOLUME_FINITE_VOLUME_SN_GRAD_SCHEMES_LIMITED_SN_GRAD_HPP_
#define FINITE_VOLUME_FINITE_VOLUME_SN_GRAD_SCHEMES_LIMITED_SN_GRAD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::fv::limitedSnGrad
// Description
//   Run-time selected snGrad scheme with limited non-orthogonal correction.
//   The limiter is controlled by a coefficient with a value between 0 and 1
//   which when 0 switches the correction off and the scheme behaves as
//   uncorrectedSnGrad, when set to 1 the full correction of the selected scheme
//   is used and when set to 0.5 the limiter is calculated such that the
//   non-orthogonal contribution does not exceed the orthogonal part.
//   Format:
//     limited <corrected scheme> <coefficient>;
//     or
//     limited <coefficient>;  // Backward compatibility

#include "corrected_sn_grad.hpp"


namespace mousse {
namespace fv {

template<class Type>
class limitedSnGrad
:
  public snGradScheme<Type>
{
  // Private data
    tmp<snGradScheme<Type>> correctedScheme_;

    scalar limitCoeff_;

  // Private Member Functions
    //- Lookup function for the corrected to support backward compatibility
    //  of dictionary specification
    tmp<snGradScheme<Type>> lookupCorrectedScheme(Istream& schemeData)
    {
      token nextToken{schemeData};
      if (nextToken.isNumber()) {
        limitCoeff_ = nextToken.number();
        return tmp<snGradScheme<Type>>
        {
          new correctedSnGrad<Type>{this->mesh()}
        };
      } else {
        schemeData.putBack(nextToken);
        tmp<snGradScheme<Type>> tcorrectedScheme
        {
          fv::snGradScheme<Type>::New(this->mesh(), schemeData)
        };
        schemeData >> limitCoeff_;

        return tcorrectedScheme;
      }
    }

public:
  //- Runtime type information
  TYPE_NAME("limited");

  // Constructors
    //- Construct from mesh
    limitedSnGrad(const fvMesh& mesh)
    :
      snGradScheme<Type>{mesh},
      correctedScheme_{new correctedSnGrad<Type>(this->mesh())},
      limitCoeff_{1}
    {}

    //- Construct from mesh and data stream
    limitedSnGrad(const fvMesh& mesh, Istream& schemeData)
    :
      snGradScheme<Type>{mesh},
      correctedScheme_{lookupCorrectedScheme(schemeData)}
    {
      if (limitCoeff_ < 0 || limitCoeff_ > 1) {
        FATAL_IO_ERROR_IN
        (
          "limitedSnGrad(const fvMesh& mesh, Istream& schemeData) : ",
          schemeData
        )
        << "limitCoeff is specified as " << limitCoeff_
        << " but should be >= 0 && <= 1"
        << exit(FatalIOError);
      }
    }

    //- Disallow default bitwise assignment
    limitedSnGrad& operator=(const limitedSnGrad&) = delete;

  //- Destructor
  virtual ~limitedSnGrad();

  // Member Functions
  //
    //- Return the interpolation weighting factors for the given field
    virtual tmp<surfaceScalarField> deltaCoeffs
    (
      const GeometricField<Type, fvPatchField, volMesh>&
    ) const
    {
      return this->mesh().nonOrthDeltaCoeffs();
    }

    //- Return true if this scheme uses an explicit correction
    virtual bool corrected() const
    {
      return true;
    }

    //- Return the explicit correction to the limitedSnGrad
    //  for the given field
    virtual tmp<GeometricField<Type, fvsPatchField, surfaceMesh>>
    correction(const GeometricField<Type, fvPatchField, volMesh>&) const;
};

}  // namespace fv
}  // namespace mousse

#include "limited_sn_grad.ipp"

#endif
