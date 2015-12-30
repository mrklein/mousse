// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::RanzMarshall
// Description
//   The Ranz-Marshall correlation for heat transfer
#ifndef _ranz_marshall_hpp_
#define _ranz_marshall_hpp_
#include "_heat_transfer_model.hpp"
namespace mousse
{
template<class CloudType>
class RanzMarshall
:
  public HeatTransferModel<CloudType>
{
public:
  //- Runtime type information
  TypeName("RanzMarshall");
  // Constructors
    //- Construct from dictionary
    RanzMarshall(const dictionary& dict, CloudType& cloud);
    //- Construct copy
    RanzMarshall(const RanzMarshall<CloudType>& im);
    //- Construct and return a clone
    virtual autoPtr<HeatTransferModel<CloudType> > clone() const
    {
      return autoPtr<HeatTransferModel<CloudType> >
      (
        new RanzMarshall<CloudType>(*this)
      );
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
#ifdef NoRepository
#   include "_ranz_marshall.cpp"
#endif
#endif
