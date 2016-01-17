// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::faceAreaWeightModel
// Description
//   Abstract base class for providing faceAreaWeight values to the cell motion
//   controller based on an argument faceAreaFraction value, typically the ratio
//   of the area of the face in question to the area of a square face with a side
//   length of the local target cell size.
// SourceFiles
//   face_area_weight_model.cpp
#ifndef face_area_weight_model_hpp_
#define face_area_weight_model_hpp_
#include "point.hpp"
#include "dictionary.hpp"
#include "auto_ptr.hpp"
#include "run_time_selection_tables.hpp"
namespace mousse
{
class faceAreaWeightModel
:
  public dictionary
{
protected:
  // Protected data
    //- Method coeffs dictionary
    dictionary coeffDict_;
private:
  // Private Member Functions
    //- Disallow default bitwise copy construct
    faceAreaWeightModel(const faceAreaWeightModel&);
    //- Disallow default bitwise assignment
    void operator=(const faceAreaWeightModel&);
public:
  //- Runtime type information
  TypeName("faceAreaWeightModel");
  // Declare run-time constructor selection table
    declareRunTimeSelectionTable
    (
      autoPtr,
      faceAreaWeightModel,
      dictionary,
      (
        const dictionary& faceAreaWeightDict
      ),
      (faceAreaWeightDict)
    );
  // Constructors
    //- Construct from components
    faceAreaWeightModel
    (
      const word& type,
      const dictionary& faceAreaWeightDict
    );
  // Selectors
    //- Return a reference to the selected faceAreaWeightModel
    static autoPtr<faceAreaWeightModel> New
    (
      const dictionary& faceAreaWeightDict
    );
  //- Destructor
  virtual ~faceAreaWeightModel();
  // Member Functions
    //- Const access to the coeffs dictionary
    const dictionary& coeffDict() const
    {
      return coeffDict_;
    }
    //- Return the current faceAreaWeight coefficient
    virtual scalar faceAreaWeight(scalar faceAreaFraction) const = 0;
};
}  // namespace mousse
#endif
