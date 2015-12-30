// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::mappedConvectiveHeatTransfer
// Description
//   Convective heat transfer model based on a re-working of a Nusselt number
//   correlation
// SourceFiles
//   mapped_convective_heat_transfer.cpp
#ifndef mapped_convective_heat_transfer_hpp_
#define mapped_convective_heat_transfer_hpp_
#include "heat_transfer_model.hpp"
#include "vol_fields.hpp"
namespace mousse
{
namespace regionModels
{
namespace surfaceFilmModels
{
class mappedConvectiveHeatTransfer
:
  public heatTransferModel
{
private:
  // Private data
    //- Heat transfer coefficient - primary region [W/m2/K]
    volScalarField htcConvPrimary_;
    //- Heat transfer coefficient - film region [W/m2/K]
    //  Assumes that the primary regtion to film region boundaries are
    //  described as mappedPushed types
    volScalarField htcConvFilm_;
  // Private member functions
    //- Disallow default bitwise copy construct
    mappedConvectiveHeatTransfer(const mappedConvectiveHeatTransfer&);
    //- Disallow default bitwise assignment
    void operator=(const mappedConvectiveHeatTransfer&);
public:
  //- Runtime type information
  TypeName("mappedConvectiveHeatTransfer");
  // Constructors
    //- Construct from surface film model and dictionary
    mappedConvectiveHeatTransfer
    (
      surfaceFilmModel& owner,
      const dictionary& dict
    );
  //- Destructor
  virtual ~mappedConvectiveHeatTransfer();
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
