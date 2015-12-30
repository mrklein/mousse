// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::extrudeModels::linearRadial
// Description
#ifndef linear_radial_hpp_
#define linear_radial_hpp_
#include "extrude_model.hpp"
namespace mousse
{
namespace extrudeModels
{
class linearRadial
:
  public extrudeModel
{
  // Private data
    scalar R_;
    scalar Rsurface_;
public:
  //- Runtime type information
  TypeName("linearRadial");
  // Constructors
    //- Construct from dictionary
    linearRadial(const dictionary& dict);
  //- Destructor
  virtual ~linearRadial();
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
