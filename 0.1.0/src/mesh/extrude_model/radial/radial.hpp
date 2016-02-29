#ifndef MESH_EXTRUDE_MODEL_RADIAL_RADIAL_HPP_
#define MESH_EXTRUDE_MODEL_RADIAL_RADIAL_HPP_

// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::extrudeModels::radial
// Description
#include "extrude_model.hpp"
#include "data_entry.hpp"
namespace mousse
{
namespace extrudeModels
{
class radial
:
  public extrudeModel
{
  // Private data
    autoPtr<DataEntry<scalar> > R_;
public:
  //- Runtime type information
  TYPE_NAME("radial");
  // Constructors
    //- Construct from dictionary
    radial(const dictionary& dict);
  //-Destructor
  virtual ~radial();
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
