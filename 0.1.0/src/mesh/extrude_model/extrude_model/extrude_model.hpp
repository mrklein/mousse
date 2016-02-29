#ifndef MESH_EXTRUDE_MODEL_EXTRUDE_MODEL_EXTRUDE_MODEL_HPP_
#define MESH_EXTRUDE_MODEL_EXTRUDE_MODEL_EXTRUDE_MODEL_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::extrudeModel
// Description
//   Top level extrusion model class
// SourceFiles
//   extrude_model.cpp
#include "dictionary.hpp"
#include "point.hpp"
#include "auto_ptr.hpp"
#include "run_time_selection_tables.hpp"
namespace mousse
{
class extrudeModel
{
protected:
  // Protected data
    label nLayers_;
    const scalar expansionRatio_;
    const dictionary& dict_;
    const dictionary& coeffDict_;
public:
  //- Runtime type information
  TYPE_NAME("extrudeModel");
  //- Declare runtime constructor selection table
    DECLARE_RUN_TIME_SELECTION_TABLE
    (
      autoPtr,
      extrudeModel,
      dictionary,
      (
        const dictionary& dict
      ),
      (dict)
    );
  // Constructors
    //- Construct from dictionary
    extrudeModel(const word& modelType, const dictionary&);
    //- Disallow default bitwise copy construct
    extrudeModel(const extrudeModel&) = delete;
    //- Disallow default bitwise assignment
    extrudeModel& operator=(const extrudeModel&) = delete;
  // Selectors
    //- Select null constructed
    static autoPtr<extrudeModel> New(const dictionary&);
  //- Destructor
  virtual ~extrudeModel();
  // Member Functions
    // Access
      label nLayers() const;
      scalar expansionRatio() const;
  // Member Operators
    //- Helper: calculate cumulative relative thickness for layer.
    //  (layer=0 -> 0; layer=nLayers -> 1)
    scalar sumThickness(const label layer) const;
    virtual point operator()
    (
      const point& surfacePoint,
      const vector& surfaceNormal,
      const label layer
    ) const = 0;
};
}  // namespace mousse
#endif
