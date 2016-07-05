#ifndef LAGRANGIAN_INTERMEDIATE_SUBMODELS_TTHERMODYNAMIC_THEAT_TRANSFER_MODEL_TRANZ_MARSHALL_HPP_
#define LAGRANGIAN_INTERMEDIATE_SUBMODELS_TTHERMODYNAMIC_THEAT_TRANSFER_MODEL_TRANZ_MARSHALL_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::RanzMarshall
// Description
//   The Ranz-Marshall correlation for heat transfer

#include "_heat_transfer_model.hpp"


namespace mousse {

template<class CloudType>
class RanzMarshall
:
  public HeatTransferModel<CloudType>
{
public:
  //- Runtime type information
  TYPE_NAME("RanzMarshall");
  // Constructors
    //- Construct from dictionary
    RanzMarshall(const dictionary& dict, CloudType& cloud);
    //- Construct copy
    RanzMarshall(const RanzMarshall<CloudType>& im);
    //- Construct and return a clone
    virtual autoPtr<HeatTransferModel<CloudType>> clone() const
    {
      return
        autoPtr<HeatTransferModel<CloudType>>
        {
          new RanzMarshall<CloudType>{*this}
        };
    }
  //- Destructor
  virtual ~RanzMarshall();
  // Member Functions
    // Evaluation
      //- Nusselt number
      virtual scalar Nu
      (
        const scalar Re,
        const scalar Pr
      ) const;
};

}  // namespace mousse

#include "_ranz_marshall.ipp"

#endif
