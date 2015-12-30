// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::SpecularReflection
// Description
//   Reversing the wall-normal component of the particle velocity

#ifndef _specular_reflection_hpp_
#define _specular_reflection_hpp_

#include "_wall_interaction_model.hpp"

namespace mousse
{
template<class CloudType>
class SpecularReflection
:
  public WallInteractionModel<CloudType>
{
public:
  //- Runtime type information
  TypeName("SpecularReflection");
  // Constructors
    //- Construct from dictionary
    SpecularReflection
    (
      const dictionary& dict,
      CloudType& cloud
    );
  //- Destructor
  virtual ~SpecularReflection();
  // Member Functions
    //- Apply wall correction
    virtual void correct
    (
      typename CloudType::parcelType& p,
      const wallPolyPatch& wpp
    );
};
}  // namespace mousse

#ifdef NoRepository
#   include "_specular_reflection.cpp"
#endif
#endif
