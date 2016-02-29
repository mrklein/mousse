#ifndef LAGRANGIAN_INTERMEDIATE_SUBMODELS_TTHERMODYNAMIC_THEAT_TRANSFER_MODEL_TNO_HEAT_TRANSFER_HPP_
#define LAGRANGIAN_INTERMEDIATE_SUBMODELS_TTHERMODYNAMIC_THEAT_TRANSFER_MODEL_TNO_HEAT_TRANSFER_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::NoHeatTransfer
// Description
//   Dummy heat transfer model for 'none'
#include "_heat_transfer_model.hpp"
namespace mousse
{
template<class CloudType>
class NoHeatTransfer
:
  public HeatTransferModel<CloudType>
{
public:
  //- Runtime type information
  TYPE_NAME("none");
  // Constructors
    //- Construct from dictionary
    NoHeatTransfer(const dictionary&, CloudType& owner);
    //- Construct copy
    NoHeatTransfer(const NoHeatTransfer<CloudType>& im);
    //- Construct and return a clone
    virtual autoPtr<HeatTransferModel<CloudType> > clone() const
    {
      return autoPtr<HeatTransferModel<CloudType> >
      (
        new NoHeatTransfer<CloudType>(*this)
      );
    }
  //- Destructor
  virtual ~NoHeatTransfer();
  // Member Functions
    //- Flag to indicate whether model activates heat transfer model
    virtual bool active() const;
    //- Nusselt number
    virtual scalar Nu(const scalar, const scalar) const;
    //- Prandtl number
    virtual scalar Pr() const;
};
}  // namespace mousse
#ifdef NoRepository
#   include "_no_heat_transfer.cpp"
#endif
#endif
