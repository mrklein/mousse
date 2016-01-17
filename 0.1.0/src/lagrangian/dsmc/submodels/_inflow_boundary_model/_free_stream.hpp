// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::FreeStream
// Description
//   Inserting new particles across the faces of a all patched of type
//   "patch" for a free stream.  Uniform values number density, temperature
//   and velocity sourced face-by-face from the boundaryT and boundaryU fields
//   of the cloud.

#ifndef _free_stream_hpp_
#define _free_stream_hpp_

#include "_inflow_boundary_model.hpp"
#include "poly_mesh.hpp"

namespace mousse
{
template<class CloudType>
class FreeStream
:
  public InflowBoundaryModel<CloudType>
{
  // Private data
    //- The indices of patches to introduce molecules across
    labelList patches_;
    //- The molecule types to be introduced
    List<label> moleculeTypeIds_;
    //- The number density of the species in the inflow
    Field<scalar> numberDensities_;
    //- A List of Lists of Fields specifying carry-over of mass flux from
    // one timestep to the next
    // + Outer List - one inner List for each patch
    // + Inner List - one Field for every species to be introduced
    // + Each field entry corresponding to a face to be injected across
    //   with a particular species
    List<List<Field<scalar> > > particleFluxAccumulators_;
public:
  //- Runtime type information
  TYPE_NAME("FreeStream");
  // Constructors
    //- Construct from dictionary
    FreeStream
    (
      const dictionary& dict,
      CloudType& cloud
    );
  //- Destructor
  virtual ~FreeStream();
  // Member Functions
    // Mapping
      //- Remap the particles to the correct cells following mesh change
      virtual void autoMap(const mapPolyMesh&);
    //- Introduce particles
    virtual void inflow();
};
}  // namespace mousse
#ifdef NoRepository
#   include "_free_stream.cpp"
#endif
#endif
