// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::constantHeatTransfer
// Description
//   Constant heat transfer model
// SourceFiles
//   constant_heat_transfer.cpp
#ifndef constant_heat_transfer_hpp_
#define constant_heat_transfer_hpp_
#include "heat_transfer_model.hpp"
#include "vol_fields_fwd.hpp"
namespace mousse
{
namespace regionModels
{
namespace surfaceFilmModels
{
class constantHeatTransfer
:
  public heatTransferModel
{
private:
  // Private data
    //- Constant heat transfer coefficient [W/m2/K]
    scalar c0_;
public:
  //- Runtime type information
  TYPE_NAME("constant");
  // Constructors
    //- Construct from surface film model and dictionary
    constantHeatTransfer
    (
      surfaceFilmModel& owner,
      const dictionary& dict
    );
    //- Disallow default bitwise copy construct
    constantHeatTransfer(const constantHeatTransfer&) = delete;
    //- Disallow default bitwise assignment
    constantHeatTransfer& operator=(const constantHeatTransfer&) = delete;
  //- Destructor
  virtual ~constantHeatTransfer();
  // Member Functions
    // Evolution
      //- Correct
      virtual void correct();
      //- Return the heat transfer coefficient [W/m2/K]
      virtual tmp<volScalarField> h() const;
};
}  // namespace surfaceFilmModels
}  // namespace regionModels
}  // namespace mousse
#endif
