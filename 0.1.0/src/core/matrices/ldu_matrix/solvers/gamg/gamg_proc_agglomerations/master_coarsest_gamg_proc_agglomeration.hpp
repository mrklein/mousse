// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::masterCoarsestGAMGProcAgglomeration
// Description
//   Processor agglomeration of GAMGAgglomerations.
// SourceFiles
//   master_coarsest_gamg_proc_agglomeration.cpp

#ifndef master_coarsest_gamg_proc_agglomeration_hpp_
#define master_coarsest_gamg_proc_agglomeration_hpp_

#include "gamg_proc_agglomeration.hpp"
#include "dynamic_list.hpp"

namespace mousse
{

class GAMGAgglomeration;
class masterCoarsestGAMGProcAgglomeration
:
  public GAMGProcAgglomeration
{
  // Private data
    DynamicList<label> comms_;

public:

  //- Runtime type information
  TYPE_NAME("masterCoarsest");

  // Constructors

    //- Construct given agglomerator and controls
    masterCoarsestGAMGProcAgglomeration
    (
      GAMGAgglomeration& agglom,
      const dictionary& controlDict
    );

    //- Disallow default bitwise copy construct
    masterCoarsestGAMGProcAgglomeration
    (
      const masterCoarsestGAMGProcAgglomeration&
    ) = delete;

    //- Disallow default bitwise assignment
    masterCoarsestGAMGProcAgglomeration& operator=
    (
      const masterCoarsestGAMGProcAgglomeration&
    ) = delete;

  //- Destructor
  virtual ~masterCoarsestGAMGProcAgglomeration();

  // Member Functions

   //- Modify agglomeration. Return true if modified
    virtual bool agglomerate();

};

}  // namespace mousse
#endif
