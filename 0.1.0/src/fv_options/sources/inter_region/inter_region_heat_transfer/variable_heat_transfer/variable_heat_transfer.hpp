#ifndef FV_OPTIONS_SOURCES_INTER_REGION_INTER_REGION_HEAT_TRANSFER_VARIABLE_HEAT_TRANSFER_VARIABLE_HEAT_TRANSFER_HPP_
#define FV_OPTIONS_SOURCES_INTER_REGION_INTER_REGION_HEAT_TRANSFER_VARIABLE_HEAT_TRANSFER_VARIABLE_HEAT_TRANSFER_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::fv::variableHeatTransfer
// Description
//   Variable heat transfer model depending on local values. The area of contact
//   between regions (area) must be provided. The Nu number is calculated as:
//     Nu = a*pow(Re, b)*pow(Pr, c)
//   and the heat transfer coefficient as:
//     htc = Nu*K/ds
//   where:
//     K is the heat conduction
//     ds is the strut diameter
#include "inter_region_heat_transfer_model.hpp"
#include "auto_ptr.hpp"
namespace mousse
{
namespace fv
{
class variableHeatTransfer
:
  public interRegionHeatTransferModel
{
private:
  // Private data
    //- Name of neighbour velocity field; default = U
    word UNbrName_;
    //- Model constants
    scalar a_;
    scalar b_;
    scalar c_;
    //- Strut diameter
    scalar ds_;
    //- Fluid Prandt number
    scalar Pr_;
    //- Area per unit volume of heat exchanger
    autoPtr<volScalarField> AoV_;
public:
  //- Runtime type information
  TYPE_NAME("variableHeatTransfer");
  // Constructors
    //- Construct from dictionary
    variableHeatTransfer
    (
      const word& name,
      const word& modelType,
      const dictionary& dict,
      const fvMesh& mesh
    );
  //- Destructor
  virtual ~variableHeatTransfer();
  // Public Functions
    //- Calculate the heat transfer coefficient
    virtual void calculateHtc();
    // IO
      //- Read dictionary
      virtual bool read(const dictionary& dict) ;
};
}  // namespace fv
}  // namespace mousse
#endif
