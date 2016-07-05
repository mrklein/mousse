#ifndef REGION_MODELS_PYROLYSIS_MODELS_PYROLYSIS_MODEL_COLLECTION_HPP_
#define REGION_MODELS_PYROLYSIS_MODELS_PYROLYSIS_MODEL_COLLECTION_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::pyrolysisModelCollection
// Description
//   A centralized pyrolysis collection.
//   Container class for a set of pyrolysis with functions implemented
//   to loop over the functions for each type.

#include "ptr_list.hpp"
#include "pyrolysis_model.hpp"


namespace mousse {

// Forward class declarations
class fvMesh;


namespace regionModels {
namespace pyrolysisModels {

class pyrolysisModelCollection
:
  public PtrList<pyrolysisModel>
{
public:
  // Runtime type information
  TYPE_NAME("pyrolysisModelCollection");
  // Constructors
    //- Construct from mesh
    pyrolysisModelCollection(const fvMesh&);
    //- Disallow default bitwise copy construct
    pyrolysisModelCollection(const pyrolysisModelCollection&) = delete;
    //- Disallow default bitwise assignment
    pyrolysisModelCollection& operator=
    (
      const pyrolysisModelCollection&
    ) = delete;
  //- Destructor
  virtual ~pyrolysisModelCollection();
  // Member Functions
    //- Pre-evolve regions
    virtual void preEvolveRegion();
    //- Evolve the pyrolysis equation regions
    virtual void evolveRegion();
    //- Evolve regions
    virtual void evolve();
    //- Provide some feedback from pyrolysis regions
    virtual void info();
    //- Return max diffusivity allowed in the solid
    virtual scalar maxDiff() const;
    //- Mean diffusion number of the solid regions
    virtual scalar solidRegionDiffNo() const;
};

}  // namespace pyrolysisModels
}  // namespace regionModels
}  // namespace mousse

#endif

