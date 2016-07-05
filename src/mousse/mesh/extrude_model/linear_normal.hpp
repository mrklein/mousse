#ifndef MESH_EXTRUDE_MODEL_LINEAR_NORMAL_LINEAR_NORMAL_HPP_
#define MESH_EXTRUDE_MODEL_LINEAR_NORMAL_LINEAR_NORMAL_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::extrudeModels::linearNormal
// Description
//   Extrudes by transforming points normal to the surface by a given distance.

#include "point.hpp"
#include "extrude_model.hpp"
#include "scalar_list.hpp"


namespace mousse {
namespace extrudeModels {

class linearNormal
:
  public extrudeModel
{
  // Private data
    //- Layer thickness
    scalar thickness_;
    //- First cell thickness
    scalar firstCellThickness_;
    //- Layer cell distibution
    scalarList layerPoints_;
public:
  //- Runtime type information
  TYPE_NAME("linearNormal");
  // Constructors
    //- Construct from dictionary
    linearNormal(const dictionary& dict);
  //- Destructor
  virtual ~linearNormal();
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

