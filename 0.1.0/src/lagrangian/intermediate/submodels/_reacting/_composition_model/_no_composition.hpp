// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::NoComposition
// Description
//   Dummy class for 'none' option - will raise an error if any functions are
//   called that require return values.
// SourceFiles
//   _no_composition.cpp
#ifndef _no_composition_hpp_
#define _no_composition_hpp_
#include "_composition_model.hpp"
namespace mousse
{
template<class CloudType>
class NoComposition
:
  public CompositionModel<CloudType>
{
public:
  //- Runtime type information
  TypeName("none");
  // Constructors
    //- Construct from dictionary
    NoComposition(const dictionary& dict, CloudType& owner);
    //- Construct copy
    NoComposition(const NoComposition<CloudType>& cm);
    //- Construct and return a clone
    virtual autoPtr<CompositionModel<CloudType> > clone() const
    {
      return autoPtr<CompositionModel<CloudType> >
      (
        new NoComposition<CloudType>(*this)
      );
    }
  //- Destructor
  virtual ~NoComposition();
  // Member Functions
    // Access
      // Mixture properties
        //- Return the list of mixture mass fractions
        //  If only 1 phase, return component fractions of that phase
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
#   include "_no_composition.cpp"
#endif
#endif
