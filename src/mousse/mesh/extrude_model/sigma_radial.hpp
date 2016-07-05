#ifndef MESH_EXTRUDE_MODEL_SIGMA_RADIAL_SIGMA_RADIAL_HPP_
#define MESH_EXTRUDE_MODEL_SIGMA_RADIAL_SIGMA_RADIAL_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::extrudeModels::sigmaRadial

#include "extrude_model.hpp"


namespace mousse {
namespace extrudeModels {

class sigmaRadial
:
  public extrudeModel
{
  // Private data
    scalar RTbyg_;
    scalar pRef_;
    scalar pStrat_;
public:
  //- Runtime type information
  TYPE_NAME("sigmaRadial");
  // Constructors
    //- Construct from dictionary
    sigmaRadial(const dictionary& dict);
  //-Destructor
  virtual ~sigmaRadial();
  // Member Operators
    point operator()
    (
      const point& surfacePoint,
      const vector& surfaceNormal,
      const label layer
    ) const;
};

}  // namespace extrudeModels
}  // namespace mousse

#endif

