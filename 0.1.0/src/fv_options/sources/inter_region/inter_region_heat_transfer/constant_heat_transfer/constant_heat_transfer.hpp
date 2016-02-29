#ifndef FV_OPTIONS_SOURCES_INTER_REGION_INTER_REGION_HEAT_TRANSFER_CONSTANT_HEAT_TRANSFER_CONSTANT_HEAT_TRANSFER_HPP_
#define FV_OPTIONS_SOURCES_INTER_REGION_INTER_REGION_HEAT_TRANSFER_CONSTANT_HEAT_TRANSFER_CONSTANT_HEAT_TRANSFER_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::fv::constantHeatTransfer
// Description
//   Constant heat transfer model. htcConst [W/m2/K] and area/volume [1/m]
//   must be provided.
#include "inter_region_heat_transfer_model.hpp"
#include "auto_ptr.hpp"
namespace mousse
{
namespace fv
{
class constantHeatTransfer
:
  public interRegionHeatTransferModel
{
private:
  // Private data
    //- Constant heat transfer coefficient [W/m2/K]
    autoPtr<volScalarField> htcConst_;
    //- Area per unit volume of heat exchanger [1/m]
    autoPtr<volScalarField> AoV_;
public:
  //- Runtime type information
   TYPE_NAME("constantHeatTransfer");
  // Constructors
    //- Construct from dictionary
    constantHeatTransfer
    (
      const word& name,
      const word& modelType,
      const dictionary& dict,
      const fvMesh& mesh
    );
  //- Destructor
  virtual ~constantHeatTransfer();
  // Public Functions
    //- Calculate the heat transfer coefficient
    virtual void calculateHtc();
    // IO
      //- Read dictionary
      virtual bool read(const dictionary& dict);
};
}  // namespace fv
}  // namespace mousse
#endif
