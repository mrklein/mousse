// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::NoInflow
// Description
//   Not inserting any particles
//
#ifndef _no_inflow_hpp_
#define _no_inflow_hpp_

#include "_inflow_boundary_model.hpp"

namespace mousse
{
template<class CloudType>
class NoInflow
:
  public InflowBoundaryModel<CloudType>
{
public:
  //- Runtime type information
  TYPE_NAME("none");
  // Constructors
    //- Construct from dictionary
    NoInflow
    (
      const dictionary& dict,
      CloudType& cloud
    );
  //- Destructor
  virtual ~NoInflow();
  // Member Functions
    //- Introduce particles (none in this case)
    virtual void inflow();
};
}  // namespace mousse

#ifdef NoRepository
#   include "_no_inflow.cpp"
#endif
#endif
