// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::relaxationModel
// Description
//   Abstract base class for providing relaxation values to the cell motion
//   controller
// SourceFiles
//   relaxation_model.cpp
#ifndef relaxation_model_hpp_
#define relaxation_model_hpp_
#include "point.hpp"
#include "time.hpp"
#include "dictionary.hpp"
#include "auto_ptr.hpp"
#include "run_time_selection_tables.hpp"
namespace mousse
{
class relaxationModel
:
  public dictionary
{
protected:
  // Protected data
    //- Reference to the conformalVoronoiMesh holding this cvControls object
    const Time& runTime_;
    //- Method coeffs dictionary
    dictionary coeffDict_;
private:
  // Private Member Functions
    //- Disallow default bitwise copy construct
    relaxationModel(const relaxationModel&);
    //- Disallow default bitwise assignment
    void operator=(const relaxationModel&);
public:
  //- Runtime type information
  TypeName("relaxationModel");
  // Declare run-time constructor selection table
    declareRunTimeSelectionTable
    (
      autoPtr,
      relaxationModel,
      dictionary,
      (
        const dictionary& relaxationDict,
        const Time& runTime
      ),
      (relaxationDict, runTime)
    );
  // Constructors
    //- Construct from components
    relaxationModel
    (
      const word& type,
      const dictionary& relaxationDict,
      const Time& runTime
    );
  // Selectors
    //- Return a reference to the selected relaxationModel
    static autoPtr<relaxationModel> New
    (
      const dictionary& relaxationDict,
      const Time& runTime
    );
  //- Destructor
  virtual ~relaxationModel();
  // Member Functions
    //- Const access to the coeffs dictionary
    const dictionary& coeffDict() const
    {
      return coeffDict_;
    }
    //- Return the current relaxation coefficient
    virtual scalar relaxation() = 0;
};
}  // namespace mousse
#endif
