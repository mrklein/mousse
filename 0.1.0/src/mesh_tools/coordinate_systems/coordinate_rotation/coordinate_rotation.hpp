// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::coordinateRotation
// Description
//   Abstract base class for coordinate rotation
//   \verbatim
//     coordinateRotation
//     {
//       type        axesRotation
//       e1          (1 0 0);
//       e2          (0 1 0);
//     }
//   \endverbatim
//   Types of coordinateRotation:
//   1) axesRotation
//   2) STARCDRotation
//   3) cylindrical
//   4) EulerCoordinateRotation
#ifndef coordinate_rotation_hpp_
#define coordinate_rotation_hpp_
#include "vector.hpp"
#include "tensor.hpp"
#include "tensor_field.hpp"
#include "dictionary.hpp"
#include "run_time_selection_tables.hpp"
#include "object_registry.hpp"
#include "poly_mesh.hpp"
namespace mousse
{
class coordinateRotation
{
protected:
  // Protected member functions
    //- Transform principal
    symmTensor transformPrincipal(const tensor&, const vector&) const;
public:
  //- Runtime type information
  TYPE_NAME("coordinateRotation");
  // Declare run-time constructor selection table
  // for constructors with dictionary and objectRegistry
  DECLARE_RUN_TIME_SELECTION_TABLE
  (
    autoPtr,
    coordinateRotation,
    objectRegistry,
    (
      const dictionary& dict, const objectRegistry& obr
    ),
    (dict, obr)
  );
  // Declare run-time constructor selection table
  // for constructors with dictionary
  DECLARE_RUN_TIME_SELECTION_TABLE
  (
    autoPtr,
    coordinateRotation,
    dictionary,
    (
      const dictionary& dict
    ),
    (dict)
  );
  // Selectors
    //- Select constructed from dictionary and objectRegistry
    static autoPtr<coordinateRotation> New
    (
      const dictionary& dict, const objectRegistry& obr
    );
    //- Select constructed from dictionary
    static autoPtr<coordinateRotation> New
    (
      const dictionary& dict
    );
  //- Destructor
  virtual ~coordinateRotation()
  {}
  // Member Functions
    //- Reset rotation to an identity rotation
    virtual void clear() = 0;
    //- Update the rotation for a list of cells
    virtual void updateCells
    (
      const polyMesh& mesh,
      const labelList& cells
    ) = 0;
    //- Return local-to-global transformation tensor
    virtual const tensor& R() const = 0;
    //- Return global-to-local transformation tensor
    virtual const tensor& Rtr() const = 0;
    //- Return local Cartesian x-axis
    virtual const vector e1() const = 0;
    //- Return local Cartesian y-axis
    virtual const vector e2() const = 0;
    //- Return local Cartesian z-axis
    virtual const vector e3() const = 0;
    //- Return local-to-global transformation tensor
    virtual const tensorField& Tr() const = 0;
    //- Return true if the rotation tensor is uniform
    virtual bool uniform() const
    {
      return true;
    }
    //- Transform vectorField using transformation tensor field
    virtual tmp<vectorField> transform(const vectorField& st) const = 0;
    //- Transform vector using transformation tensor
    virtual vector transform(const vector& st) const = 0;
    //- Inverse transform vectorField using transformation tensor field
    virtual tmp<vectorField> invTransform(const vectorField& st) const = 0;
    //- Inverse transform vector using transformation tensor
    virtual vector invTransform(const vector& st) const = 0;
    //- Transform tensor field using transformation tensorField
    virtual tmp<tensorField> transformTensor
    (
      const tensorField& st
    ) const = 0;
    //- Transform tensor sub-field using transformation tensorField
    virtual tmp<tensorField> transformTensor
    (
      const tensorField& st,
      const labelList& cellMap
    ) const = 0;
    //- Transform tensor using transformation tensorField
    virtual tensor transformTensor(const tensor& st) const = 0;
    //- Transform vectorField using transformation tensorField and return
    // symmetrical tensorField
    virtual tmp<symmTensorField> transformVector
    (
      const vectorField& st
    ) const = 0;
    //- Transform vector using transformation tensor and return
    // symmetrical tensor
    virtual symmTensor transformVector(const vector& st) const = 0;
  // Write
      //- Write
      virtual void write(Ostream&) const = 0;
};
}  // namespace mousse
#endif
