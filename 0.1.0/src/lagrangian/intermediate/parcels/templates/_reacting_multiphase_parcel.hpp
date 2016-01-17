// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::ReactingMultiphaseParcel
// Description
//   Multiphase variant of the reacting parcel class with one/two-way coupling
//   with the continuous phase.
// SourceFiles
//   _reacting_multiphase_parcel.cpp
//   _reacting_multiphase_parcel_io.cpp
#ifndef reacting_multiphase_parcel_hpp_
#define reacting_multiphase_parcel_hpp_
#include "particle.hpp"
#include "slg_thermo.hpp"
#include "demand_driven_entry.hpp"
namespace mousse
{
template<class ParcelType>
class ReactingMultiphaseParcel;
template<class ParcelType>
Ostream& operator<<
(
  Ostream&,
  const ReactingMultiphaseParcel<ParcelType>&
);
template<class ParcelType>
class ReactingMultiphaseParcel
:
  public ParcelType
{
  // Private data
    //- Size in bytes of the fields
    static const std::size_t sizeofFields_;
public:
  // IDs of phases in ReacingParcel phase list (Y)
    static const label GAS;
    static const label LIQ;
    static const label SLD;
  //- Class to hold reacting multiphase particle constant properties
  class constantProperties
  :
    public ParcelType::constantProperties
  {
    // Private data
      //- Devolatilisation activation temperature [K]
      demandDrivenEntry<scalar> TDevol_;
      //- Latent heat of devolatilisation [J/kg]
      demandDrivenEntry<scalar> LDevol_;
      //- Fraction of enthalpy retained by parcel due to surface
      //  reactions
      demandDrivenEntry<scalar> hRetentionCoeff_;
  public:
    // Constructors
      //- Null constructor
      constantProperties();
      //- Copy constructor
      constantProperties(const constantProperties& cp);
      //- Construct from dictionary
      constantProperties(const dictionary& parentDict);
    // Access
      //- Return const access to the devolatilisation temperature
      inline scalar TDevol() const;
      //- Return const access to the latent heat of devolatilisation
      inline scalar LDevol() const;
      //- Return const access to the fraction of enthalpy retained by
      //  parcel due to surface reactions
      inline scalar hRetentionCoeff() const;
  };
private:
  // Private Member Functions
    //- Return the mixture effective specific heat capacity
    template<class TrackData>
    scalar CpEff
    (
      TrackData& td,
      const scalar p,
      const scalar T,
      const label idG,
      const label idL,
      const label idS
    ) const;
    //- Return the mixture effective sensible enthalpy
    template<class TrackData>
    scalar HsEff
    (
      TrackData& td,
      const scalar p,
      const scalar T,
      const label idG,
      const label idL,
      const label idS
    ) const;
    //- Return the mixture effective latent heat
    template<class TrackData>
    scalar LEff
    (
      TrackData& td,
      const scalar p,
      const scalar T,
      const label idG,
      const label idL,
      const label idS
    ) const;
    //- Update the mass fractions (Y, YGas, YLiquid, YSolid)
    scalar updateMassFractions
    (
      const scalar mass0,
      const scalarField& dMassGas,
      const scalarField& dMassLiquid,
      const scalarField& dMassSolid
    );
protected:
  // Protected data
    // Parcel properties
      //- Mass fractions of gases []
      scalarField YGas_;
      //- Mass fractions of liquids []
      scalarField YLiquid_;
      //- Mass fractions of solids []
      scalarField YSolid_;
      //- Flag to identify if the particle can devolatilise and combust
      //  Combustion possible only after volatile content falls below
      //  threshold value.  States include:
      //  0 = can devolatilise, cannot combust but can change
      //  1 = can devolatilise, can combust
      // -1 = cannot devolatilise or combust, and cannot change
      label canCombust_;
  // Protected Member Functions
    //- Calculate Devolatilisation
    template<class TrackData>
    void calcDevolatilisation
    (
      TrackData& td,
      const scalar dt,           // timestep
      const scalar age,          // age
      const scalar Ts,           // surface temperature
      const scalar d,            // diameter
      const scalar T,            // temperature
      const scalar mass,         // mass
      const scalar mass0,        // mass (initial on injection)
      const scalarField& YGasEff,// gas component mass fractions
      const scalarField& YLiquidEff,// liquid component mass fractions
      const scalarField& YSolidEff,// solid component mass fractions
      label& canCombust,          // 'can combust' flag
      scalarField& dMassDV,      // mass transfer - local to particle
      scalar& Sh,                // explicit particle enthalpy source
      scalar& N,                 // flux of species emitted from particle
      scalar& NCpW,              // sum of N*Cp*W of emission species
      scalarField& Cs            // carrier conc. of emission species
    ) const;
    //- Calculate surface reactions
    template<class TrackData>
    void calcSurfaceReactions
    (
      TrackData& td,
      const scalar dt,           // timestep
      const label cellI,         // owner cell
      const scalar d,            // diameter
      const scalar T,            // temperature
      const scalar mass,         // mass
      const label canCombust,     // 'can combust' flag
      const scalar N,            // flux of species emitted from particle
      const scalarField& YMix,   // mixture mass fractions
      const scalarField& YGas,   // gas-phase mass fractions
      const scalarField& YLiquid,// liquid-phase mass fractions
      const scalarField& YSolid, // solid-phase mass fractions
      scalarField& dMassSRGas,   // gas-phase mass transfer - local
      scalarField& dMassSRLiquid,// liquid-phase mass transfer - local
      scalarField& dMassSRSolid, // solid-phase mass transfer - local
      scalarField& dMassSRCarrier, // carrier phase mass transfer
      scalar& Sh,                // explicit particle enthalpy source
      scalar& dhsTrans           // sensible enthalpy transfer to carrier
    ) const;
public:
  // Static data members
    //- Runtime type information
    TYPE_NAME("ReactingMultiphaseParcel");
    //- String representation of properties
    AddToPropertyList
    (
      ParcelType,
      " nGas(Y1..YN)"
     + " nLiquid(Y1..YN)"
     + " nSolid(Y1..YN)"
    );
  // Constructors
    //- Construct from owner, position, and cloud owner
    //  Other properties initialised as null
    inline ReactingMultiphaseParcel
    (
      const polyMesh& mesh,
      const vector& position,
      const label cellI,
      const label tetFaceI,
      const label tetPtI
    );
    //- Construct from components
    inline ReactingMultiphaseParcel
    (
      const polyMesh& mesh,
      const vector& position,
      const label cellI,
      const label tetFaceI,
      const label tetPtI,
      const label typeId,
      const scalar nParticle0,
      const scalar d0,
      const scalar dTarget0,
      const vector& U0,
      const vector& f0,
      const vector& angularMomentum0,
      const vector& torque0,
      const scalarField& Y0,
      const scalarField& YGas0,
      const scalarField& YLiquid0,
      const scalarField& YSolid0,
      const constantProperties& constProps
    );
    //- Construct from Istream
    ReactingMultiphaseParcel
    (
      const polyMesh& mesh,
      Istream& is,
      bool readFields = true
    );
    //- Construct as a copy
    ReactingMultiphaseParcel(const ReactingMultiphaseParcel& p);
    //- Construct as a copy
    ReactingMultiphaseParcel
    (
      const ReactingMultiphaseParcel& p,
      const polyMesh& mesh
    );
    //- Construct and return a (basic particle) clone
    virtual autoPtr<particle> clone() const
    {
      return autoPtr<particle>(new ReactingMultiphaseParcel(*this));
    }
    //- Construct and return a (basic particle) clone
    virtual autoPtr<particle> clone(const polyMesh& mesh) const
    {
      return autoPtr<particle>(new ReactingMultiphaseParcel(*this, mesh));
    }
    //- Factory class to read-construct particles used for
    //  parallel transfer
    class iNew
    {
      const polyMesh& mesh_;
    public:
      iNew(const polyMesh& mesh)
      :
        mesh_(mesh)
      {}
      autoPtr<ReactingMultiphaseParcel<ParcelType> > operator()
      (
        Istream& is
      ) const
      {
        return autoPtr<ReactingMultiphaseParcel<ParcelType> >
        (
          new ReactingMultiphaseParcel<ParcelType>(mesh_, is, true)
        );
      }
    };
  // Member Functions
    // Access
      //- Return const access to mass fractions of gases
      inline const scalarField& YGas() const;
      //- Return const access to mass fractions of liquids
      inline const scalarField& YLiquid() const;
      //- Return const access to mass fractions of solids
      inline const scalarField& YSolid() const;
      //- Return const access to the canCombust flag
      inline label canCombust() const;
    // Edit
      //- Return access to mass fractions of gases
      inline scalarField& YGas();
      //- Return access to mass fractions of liquids
      inline scalarField& YLiquid();
      //- Return access to mass fractions of solids
      inline scalarField& YSolid();
      //- Return access to the canCombust flag
      inline label& canCombust();
    // Main calculation loop
      //- Set cell values
      template<class TrackData>
      void setCellValues
      (
        TrackData& td,
        const scalar dt,
        const label cellI
      );
      //- Correct cell values using latest transfer information
      template<class TrackData>
      void cellValueSourceCorrection
      (
        TrackData& td,
        const scalar dt,
        const label cellI
      );
      //- Update parcel properties over the time interval
      template<class TrackData>
      void calc
      (
        TrackData& td,
        const scalar dt,
        const label cellI
      );
    // I-O
      //- Read
      template<class CloudType, class CompositionType>
      static void readFields
      (
        CloudType& c,
        const CompositionType& compModel
      );
      //- Read - no composition
      template<class CloudType>
      static void readFields(CloudType& c);
      //- Write
      template<class CloudType, class CompositionType>
      static void writeFields
      (
        const CloudType& c,
        const CompositionType& compModel
      );
      //- Read - composition supplied
      template<class CloudType>
      static void writeFields(const CloudType& c);
  // Ostream Operator
    friend Ostream& operator<< <ParcelType>
    (
      Ostream&,
      const ReactingMultiphaseParcel<ParcelType>&
    );
};
}  // namespace mousse

// Constructors 
template<class ParcelType>
inline mousse::ReactingMultiphaseParcel<ParcelType>::constantProperties::
constantProperties()
:
  ParcelType::constantProperties{},
  TDevol_{this->dict_, 0.0},
  LDevol_{this->dict_, 0.0},
  hRetentionCoeff_{this->dict_, 0.0}
{}
template<class ParcelType>
inline mousse::ReactingMultiphaseParcel<ParcelType>::constantProperties::
constantProperties
(
  const constantProperties& cp
)
:
  ParcelType::constantProperties{cp},
  TDevol_{cp.TDevol_},
  LDevol_{cp.LDevol_},
  hRetentionCoeff_{cp.hRetentionCoeff_}
{}
template<class ParcelType>
inline mousse::ReactingMultiphaseParcel<ParcelType>::constantProperties::
constantProperties
(
  const dictionary& parentDict
)
:
  ParcelType::constantProperties{parentDict},
  TDevol_{this->dict_, "TDevol"},
  LDevol_{this->dict_, "LDevol"},
  hRetentionCoeff_{this->dict_, "hRetentionCoeff"}
{}
template<class ParcelType>
inline mousse::ReactingMultiphaseParcel<ParcelType>::ReactingMultiphaseParcel
(
  const polyMesh& mesh,
  const vector& position,
  const label cellI,
  const label tetFaceI,
  const label tetPtI
)
:
  ParcelType{mesh, position, cellI, tetFaceI, tetPtI},
  YGas_{0},
  YLiquid_{0},
  YSolid_{0},
  canCombust_{0}
{}
template<class ParcelType>
inline mousse::ReactingMultiphaseParcel<ParcelType>::ReactingMultiphaseParcel
(
  const polyMesh& mesh,
  const vector& position,
  const label cellI,
  const label tetFaceI,
  const label tetPtI,
  const label typeId,
  const scalar nParticle0,
  const scalar d0,
  const scalar dTarget0,
  const vector& U0,
  const vector& f0,
  const vector& angularMomentum0,
  const vector& torque0,
  const scalarField& Y0,
  const scalarField& YGas0,
  const scalarField& YLiquid0,
  const scalarField& YSolid0,
  const constantProperties& constProps
)
:
  ParcelType
  {
    mesh,
    position,
    cellI,
    tetFaceI,
    tetPtI,
    typeId,
    nParticle0,
    d0,
    dTarget0,
    U0,
    f0,
    angularMomentum0,
    torque0,
    Y0,
    constProps
  },
  YGas_{YGas0},
  YLiquid_{YLiquid0},
  YSolid_{YSolid0},
  canCombust_{0}
{}
// constantProperties Member Functions
template<class ParcelType>
inline mousse::scalar
mousse::ReactingMultiphaseParcel<ParcelType>::constantProperties::TDevol() const
{
  return TDevol_.value();
}
template<class ParcelType>
inline mousse::scalar
mousse::ReactingMultiphaseParcel<ParcelType>::constantProperties::LDevol() const
{
  return LDevol_.value();
}
template<class ParcelType>
inline mousse::scalar
mousse::ReactingMultiphaseParcel<ParcelType>::constantProperties::
hRetentionCoeff() const
{
  scalar value = hRetentionCoeff_.value();
  if ((value < 0) || (value > 1))
  {
    FATAL_ERROR_IN
    (
      "ReactingMultiphaseParcel<ParcelType>::constantProperties::"
      "constantProperties"
    )
    << "hRetentionCoeff must be in the range 0 to 1" << nl
    << exit(FatalError) << endl;
  }
  return value;
}
// ThermoParcel Member Functions
template<class ParcelType>
inline const mousse::scalarField& mousse::ReactingMultiphaseParcel<ParcelType>::
YGas() const
{
  return YGas_;
}
template<class ParcelType>
inline const mousse::scalarField& mousse::ReactingMultiphaseParcel<ParcelType>::
YLiquid() const
{
  return YLiquid_;
}
template<class ParcelType>
inline const mousse::scalarField& mousse::ReactingMultiphaseParcel<ParcelType>::
YSolid() const
{
  return YSolid_;
}
template<class ParcelType>
inline mousse::label
mousse::ReactingMultiphaseParcel<ParcelType>::canCombust() const
{
  return canCombust_;
}
template<class ParcelType>
inline mousse::scalarField& mousse::ReactingMultiphaseParcel<ParcelType>::YGas()
{
  return YGas_;
}
template<class ParcelType>
inline mousse::scalarField& mousse::ReactingMultiphaseParcel<ParcelType>::YLiquid()
{
  return YLiquid_;
}
template<class ParcelType>
inline mousse::scalarField& mousse::ReactingMultiphaseParcel<ParcelType>::YSolid()
{
  return YSolid_;
}
template<class ParcelType>
inline mousse::label& mousse::ReactingMultiphaseParcel<ParcelType>::canCombust()
{
  return canCombust_;
}
#ifdef NoRepository
  #include "_reacting_multiphase_parcel.cpp"
#endif
#endif
