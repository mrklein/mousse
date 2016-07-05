#ifndef MESH_EXTRUDE_MODEL_PLANE_EXTRUSION_HPP_
#define MESH_EXTRUDE_MODEL_PLANE_EXTRUSION_HPP_

//! mousse: CFD toolbox
//! \copyright (C) 2015 OpenFOAM Foundation
//! \copyright (C) 2016 mousse project
//! \class mousse::extrudeModels::plane
//! \description Extrudes by transforming points normal to the surface by
//!              1 layer over a given distance.
//! \seealso
//!   mousse::extrudeModels::linearNormal

#include "linear_normal.hpp"


namespace mousse {
namespace extrudeModels {

class plane
:
  public linearNormal
{
public:
  //- Runtime type information
  TYPE_NAME("plane");
  // Constructors
    //- Construct from dictionary
    plane(const dictionary& dict);
  //- Destructor
  virtual ~plane();
};

}  // namespace extrudeModels
}  // namespace mousse

#endif

