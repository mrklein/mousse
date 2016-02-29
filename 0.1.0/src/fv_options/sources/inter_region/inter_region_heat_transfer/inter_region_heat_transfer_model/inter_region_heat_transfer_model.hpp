#ifndef FV_OPTIONS_SOURCES_INTER_REGION_INTER_REGION_HEAT_TRANSFER_INTER_REGION_HEAT_TRANSFER_MODEL_INTER_REGION_HEAT_TRANSFER_MODEL_HPP_
#define FV_OPTIONS_SOURCES_INTER_REGION_INTER_REGION_HEAT_TRANSFER_INTER_REGION_HEAT_TRANSFER_MODEL_INTER_REGION_HEAT_TRANSFER_MODEL_HPP_

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
  TYPE_NAME("interRegionHeatTransferModel");
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

inline const mousse::word&
mousse::fv::interRegionHeatTransferModel::nbrRegionName() const
{
  return nbrRegionName_;
}
inline const mousse::meshToMesh&
mousse::fv::interRegionHeatTransferModel::meshInterp() const
{
  if (!meshInterpPtr_.valid())
  {
    FATAL_ERROR_IN
    (
      "const meshToMesh& interRegionHeatTransferModel::meshInterp() const"
    )
    << "Interpolation object not set"
    << abort(FatalError);
  }
  return meshInterpPtr_();
}
inline const mousse::volScalarField&
mousse::fv::interRegionHeatTransferModel::htc() const
{
  return  htc_;
}
inline const mousse::fv::interRegionHeatTransferModel&
mousse::fv::interRegionHeatTransferModel::nbrModel() const
{
  if (nbrModel_ == NULL)
  {
    FATAL_ERROR_IN("const interRegionHeatTransferModel& nbrModel() const")
      << "Neighbour model not set"
      << abort(FatalError);
  }
  return *nbrModel_;
}
inline mousse::fv::interRegionHeatTransferModel&
mousse::fv::interRegionHeatTransferModel::nbrModel()
{
  if (nbrModel_ == NULL)
  {
    FATAL_ERROR_IN("interRegionHeatTransferModel& nbrModel()")
      << "Neighbour model not set"
      << abort(FatalError);
  }
  return *nbrModel_;
}
#ifdef NoRepository
  #include "inter_region_heat_transfer_model_templates.cpp"
#endif
#endif
