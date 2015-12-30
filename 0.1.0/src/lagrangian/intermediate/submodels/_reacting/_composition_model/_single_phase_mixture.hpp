// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::SinglePhaseMixture
// Description
//   Templated parcel single phase, multi-component class
// SourceFiles
//   _single_phase_mixture.cpp
#ifndef _single_phase_mixture_hpp_
#define _single_phase_mixture_hpp_
#include "_composition_model.hpp"
namespace mousse
{
template<class CloudType>
class SinglePhaseMixture
:
  public CompositionModel<CloudType>
{
  // Private data
    // Indices of the phases - only 1 will be set
      //- Gas
      label idGas_;
      //- Liquid
      label idLiquid_;
      //- Solid
      label idSolid_;
  // Private Member Functions
    //- Construct the indices and check correct specification of
    //  1 gas or 1 liquid or 1 solid
    void constructIds();
public:
  //- Runtime type information
  TypeName("singlePhaseMixture");
  // Constructors
    //- Construct from dictionary
    SinglePhaseMixture(const dictionary& dict, CloudType& owner);
    //- Construct copy
    SinglePhaseMixture(const SinglePhaseMixture<CloudType>& cm);
    //- Construct and return a clone
    virtual autoPtr<CompositionModel<CloudType> > clone() const
    {
      return autoPtr<CompositionModel<CloudType> >
      (
        new SinglePhaseMixture<CloudType>(*this)
      );
    }
  //- Destructor
  virtual ~SinglePhaseMixture();
  // Member Functions
    // Access
      // Mixture properties
        //- Return the list of mixture mass fractions
        virtual const scalarField& YMixture0() const;
        // Indices of gas, liquid and solid phases in phase properties
        // list
          //- Gas id
          virtual label idGas() const;
          //- Liquid id
          virtual label idLiquid() const;
          //- Solid id
          virtual label idSolid() const;
};
}  // namespace mousse
#ifdef NoRepository
#   include "_single_phase_mixture.cpp"
#endif
#endif
