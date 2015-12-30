// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::fv::interRegionHeatTransferModel
// Description
//   Base class for inter region heat exchange. The derived classes must
//   provide the heat transfer coeffisine (htc) which is used as follows
//   in the energy equation.
//   \f[
//     -htc*Tmapped + Sp(htc, T)
//   \f]
#ifndef inter_region_heat_transfer_model_hpp_
#define inter_region_heat_transfer_model_hpp_
#include "inter_region_option.hpp"
#include "vol_fields.hpp"
namespace mousse
{
namespace fv
{
class interRegionHeatTransferModel
:
  public interRegionOption
{
protected:
  // Protected data
    //- Name of the model in the neighbour mesh
    word nbrModelName_;
    //- Pointer to neighbour interRegionHeatTransferModel
    interRegionHeatTransferModel* nbrModel_;
    //- First iteration
    bool firstIter_;
    //- Time index - used for updating htc
    label timeIndex_;
    //- Heat transfer coefficient [W/m2/k] times area/volume [1/m]
    volScalarField htc_;
    //- Flag to activate semi-implicit coupling
    bool semiImplicit_;
    //- Name of temperature field; default = "T"
    word TName_;
    //- Name of neighbour temperature field; default = "T"
    word TNbrName_;
  // Protected member functions
    //- Set the neighbour interRegionHeatTransferModel
    void setNbrModel();
    //- Inherit correct from interRegionOption
    using interRegionOption::correct;
    //- Correct to calculate the inter-region heat transfer coefficient
    void correct();
    //- Interpolate field with nbrModel specified
    template<class Type>
    tmp<Field<Type> > interpolate
    (
      const interRegionHeatTransferModel& nbrModel,
      const Field<Type>& field
    ) const;
    //- Interpolate field without nbrModel specified
    template<class Type>
    tmp<Field<Type> > interpolate
    (
      const Field<Type>& field
    ) const;
    //- Interpolate field with nbrModel specified
    template<class Type>
    void interpolate
    (
      const interRegionHeatTransferModel& nbrModel,
      const Field<Type>& field,
      Field<Type>& result
    ) const;
    //- Interpolate field without nbrModel specified
    template<class Type>
    void interpolate
    (
      const Field<Type>& field,
      Field<Type>& result
    ) const;
public:
  //- Runtime type information
  TypeName("interRegionHeatTransferModel");
  // Constructors
    //- Construct from dictionary
    interRegionHeatTransferModel
    (
      const word& name,
      const word& modelType,
      const dictionary& dict,
      const fvMesh& mesh
    );
  //- Destructor
  virtual ~interRegionHeatTransferModel();
  // Member Functions
    // Access
      //- Return const access to the neighbour region name
      inline const word& nbrRegionName() const;
      //- Return const access to the mapToMap pointer
      inline const meshToMesh& meshInterp() const;
      //- Return the heat transfer coefficient
      inline const volScalarField& htc() const;
      //- Return const access to the neighbour model
      inline const interRegionHeatTransferModel& nbrModel() const;
      //- Return access to the neighbour model
      inline interRegionHeatTransferModel& nbrModel();
      //- Source term to energy equation
      virtual void addSup
      (
        fvMatrix<scalar>& eqn,
        const label fieldI
      );
      //- Source term to compressible energy equation
      virtual void addSup
      (
        const volScalarField& rho,
        fvMatrix<scalar>& eqn,
        const label fieldI
      );
      //- Calculate heat transfer coefficient
      virtual void calculateHtc() = 0;
    // IO
      //- Read dictionary
      virtual bool read(const dictionary& dict);
};
}  // namespace fv
}  // namespace mousse
#include "inter_region_heat_transfer_model_i.hpp"
#ifdef NoRepository
  #include "inter_region_heat_transfer_model_templates.cpp"
#endif
#endif
