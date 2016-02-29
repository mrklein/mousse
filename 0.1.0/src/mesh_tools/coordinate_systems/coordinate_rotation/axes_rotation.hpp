#ifndef MESH_TOOLS_COORDINATE_SYSTEMS_COORDINATE_ROTATION_AXES_ROTATION_HPP_
#define MESH_TOOLS_COORDINATE_SYSTEMS_COORDINATE_ROTATION_AXES_ROTATION_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::axesRotation
// Description
//   A coordinate rotation specified using global axis
//   The rotation is defined by a combination of vectors (e1/e2), (e2/e3)
//   or (e3/e1). Any nonorthogonality will be absorbed into the second vector.
//   \verbatim
//     axesRotation
//     {
//       type        axesRotation;
//       e1          (1 0 0);
//       e2          (0 1 0);
//     }
//   \endverbatim
#include "vector.hpp"
#include "coordinate_rotation.hpp"
#include "dictionary.hpp"
#include "run_time_selection_tables.hpp"
namespace mousse
{
class axesRotation
:
  public coordinateRotation
{
  // Private data
    //- Local-to-Global transformation tensor
    tensor R_;
    //- Global-to-Local transformation tensor
    tensor Rtr_;
    //- The combination of local axes to be used
    enum axisOrder
    {
      e1e2,
      e2e3,
      e3e1
    };
  // Private Member Functions
    //- Calculate transformation tensor
    void calcTransform
    (
      const vector& axis1,
      const vector& axis2,
      const axisOrder& order = e3e1
    );
public:
  //- Runtime type information
  TYPE_NAME("axesRotation");
  // Constructors
    //- Construct null
    axesRotation();
    //- Construct from 2 axes
    axesRotation(const vector& axis, const vector& dir);
    //- Construct from dictionary
    axesRotation(const dictionary&);
    //- Construct from components
    axesRotation(const tensor& R);
    //- Construct from dictionary and mesh
    axesRotation(const dictionary&, const objectRegistry&);
    //- Return clone
    autoPtr<axesRotation> clone() const
    {
      return autoPtr<axesRotation>(new axesRotation(*this));
    }
  //- Destructor
  virtual ~axesRotation()
  {}
  // Member Functions
    //- Reset rotation to an identity rotation
    virtual void clear()
    {
      R_ = sphericalTensor::I;
      Rtr_ = sphericalTensor::I;
    }
    //- Update the rotation for a list of cells
    virtual void updateCells(const polyMesh&, const labelList&)
    {
      // Do nothing
    }
    //- Return local-to-global transformation tensor
    virtual const tensor& R() const
    {
      return R_;
    }
    //- Return global-to-local transformation tensor
    virtual const tensor& Rtr() const
    {
      return Rtr_;
    }
    //- Return local Cartesian x-axis
    virtual const vector e1() const
    {
      return R_.x();
    }
    //- Return local Cartesian y-axis
    virtual const vector e2() const
    {
      return R_.y();
    }
    //- Return local Cartesian z-axis
    virtual const vector e3() const
    {
      return R_.z();
    }
    //- Return transformation tensor field
    virtual const tensorField& Tr() const;
    //- Transform vectorField using transformation tensor field
    virtual tmp<vectorField> transform(const vectorField& st) const;
    //- Transform vector using transformation tensor
    virtual vector transform(const vector& st) const;
    //- Inverse transform vectorField using transformation tensor field
    virtual tmp<vectorField> invTransform(const vectorField& st) const;
    //- Inverse transform vector using transformation tensor
    virtual vector invTransform(const vector& st) const;
    //- Transform tensor field using transformation tensorField
    virtual tmp<tensorField> transformTensor(const tensorField& st) const;
    //- Transform tensor using transformation tensorField
    virtual tensor transformTensor(const tensor& st) const;
    //- Transform tensor sub-field using transformation tensorField
    virtual tmp<tensorField> transformTensor
    (
      const tensorField& st,
      const labelList& cellMap
    ) const;
    //- Transform vectorField using transformation tensorField and return
    //  symmetric tensorField
    virtual tmp<symmTensorField> transformVector
    (
      const vectorField& st
    ) const;
    //- Transform vector using transformation tensor and return
    //  symmetric tensor
    virtual symmTensor transformVector(const vector& st) const;
  // Member Operators
    //- Assign from dictionary
    void operator=(const dictionary&);
  // Write
    //- Write
    virtual void write(Ostream&) const;
};
}  // namespace mousse
#endif
