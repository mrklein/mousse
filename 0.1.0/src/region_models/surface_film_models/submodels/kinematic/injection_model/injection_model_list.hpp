// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::injectionModelList
// Description
//   List container for film injection models
// SourceFiles
//   injection_model_list.cpp
#ifndef injection_model_list_hpp_
#define injection_model_list_hpp_
#include "ptr_list.hpp"
#include "injection_model.hpp"
#include "film_sub_model_base.hpp"
namespace mousse
{
namespace regionModels
{
namespace surfaceFilmModels
{
class injectionModelList
:
  public PtrList<injectionModel>,
  public filmSubModelBase
{
private:
  // Private data
    //- List of mass injected per patch
    scalarField massInjected_;
  // Private Member Functions
    //- Disallow default bitwise copy construct
    injectionModelList(const injectionModelList&);
    //- Disallow default bitwise assignment
    void operator=(const injectionModelList&);
public:
  // Constructors
    //- Construct null
    injectionModelList(surfaceFilmModel& owner);
    //- Construct from type name, dictionary and surface film model
    injectionModelList
    (
      surfaceFilmModel& owner,
      const dictionary& dict
    );
  //- Destructor
  virtual ~injectionModelList();
  // Member Functions
    // Evolution
      //- Correct
      virtual void correct
      (
        scalarField& availableMass,
        volScalarField& massToInject,
        volScalarField& diameterToInject
      );
    // I-O
      //- Provide some info
      virtual void info(Ostream& os);
};
}  // namespace surfaceFilmModels
}  // namespace regionModels
}  // namespace mousse
#endif
