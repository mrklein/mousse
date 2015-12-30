// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::pyrolysisModelCollection
// Description
//   A centralized pyrolysis collection.
//   Container class for a set of pyrolysis with functions implemented
//   to loop over the functions for each type.
// SourceFiles
//   pyrolysis_model_collection.cpp
#ifndef pyrolysis_model_collection_hpp_
#define pyrolysis_model_collection_hpp_
#include "ptr_list.hpp"
#include "pyrolysis_model.hpp"
namespace mousse
{
// Forward class declarations
class fvMesh;
namespace regionModels
{
namespace pyrolysisModels
{
class pyrolysisModelCollection
:
  public PtrList<pyrolysisModel>
{
  // Private Member Functions
    //- Disallow default bitwise copy construct
    pyrolysisModelCollection(const pyrolysisModelCollection&);
    //- Disallow default bitwise assignment
    void operator=(const pyrolysisModelCollection&);
public:
  // Runtime type information
  TypeName("pyrolysisModelCollection");
  // Constructors
    //- Construct from mesh
    pyrolysisModelCollection(const fvMesh&);
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
