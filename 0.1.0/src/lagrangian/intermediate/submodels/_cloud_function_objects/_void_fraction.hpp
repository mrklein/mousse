#ifndef LAGRANGIAN_INTERMEDIATE_SUBMODELS_TCLOUD_FUNCTION_OBJECTS_TVOID_FRACTION_HPP_
#define LAGRANGIAN_INTERMEDIATE_SUBMODELS_TCLOUD_FUNCTION_OBJECTS_TVOID_FRACTION_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::VoidFraction
// Description
//   Creates particle void fraction field on carrier phase

#include "_cloud_function_object.hpp"
#include "vol_fields.hpp"


namespace mousse {

template<class CloudType>
class VoidFraction
:
  public CloudFunctionObject<CloudType>
{
  // Private Data
    // Typedefs
      //- Convenience typedef for parcel type
      typedef typename CloudType::parcelType parcelType;
    //- Void fraction field
    autoPtr<volScalarField> thetaPtr_;
protected:
  // Protected Member Functions
    //- Write post-processing info
    virtual void write();
public:
  //- Runtime type information
  TYPE_NAME("voidFraction");
  // Constructors
    //- Construct from dictionary
    VoidFraction
    (
      const dictionary& dict,
      CloudType& owner,
      const word& modelName
    );
    //- Construct copy
    VoidFraction(const VoidFraction<CloudType>& vf);
    //- Construct and return a clone
    virtual autoPtr<CloudFunctionObject<CloudType>> clone() const
    {
      return
        autoPtr<CloudFunctionObject<CloudType>>
        {
          new VoidFraction<CloudType>{*this}
        };
    }
  //- Destructor
  virtual ~VoidFraction();
  // Member Functions
    // Evaluation
      //- Pre-evolve hook
      virtual void preEvolve();
      //- Post-evolve hook
      virtual void postEvolve();
      //- Post-move hook
      virtual void postMove
      (
        parcelType& p,
        const label cellI,
        const scalar dt,
        const point& position0,
        bool& keepParticle
      );
};

}  // namespace mousse

#include "_void_fraction.ipp"

#endif
