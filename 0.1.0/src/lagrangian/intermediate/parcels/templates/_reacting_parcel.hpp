// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::ReactingParcel
// Description
//   Reacting parcel class with one/two-way coupling with the continuous
//   phase.
// SourceFiles
//   _reacting_parcel_i.hpp
//   _reacting_parcel.cpp
//   _reacting_parcel_io.cpp
#ifndef _reacting_parcel_hpp_
#define _reacting_parcel_hpp_
#include "particle.hpp"
#include "slg_thermo.hpp"
#include "demand_driven_entry.hpp"
namespace mousse
{
template<class ParcelType>
class ReactingParcel;
template<class ParcelType>
Ostream& operator<<
(
  Ostream&,
  const ReactingParcel<ParcelType>&
);
template<class ParcelType>
class ReactingParcel
:
  public ParcelType
{
  // Private data
    //- Size in bytes of the fields
    static const std::size_t sizeofFields_;
public:
  //- Class to hold reacting parcel constant properties
  class constantProperties
  :
    public ParcelType::constantProperties
  {
    // Private data
      //- Minimum pressure [Pa]
      demandDrivenEntry<scalar> pMin_;
      //- Constant volume flag - e.g. during mass transfer
      demandDrivenEntry<bool> constantVolume_;
  public:
    // Constructors
      //- Null constructor
      constantProperties();
      //- Copy constructor
      constantProperties(const constantProperties& cp);
      //- Construct from dictionary
      constantProperties(const dictionary& parentDict);
    // Access
      //- Return const access to the minimum pressure
      inline scalar pMin() const;
      //- Return const access to the constant volume flag
      inline bool constantVolume() const;
  };
  template<class CloudType>
  class TrackingData
  :
    public ParcelType::template TrackingData<CloudType>
  {
  private:
    // Private data
      // Interpolators for continuous phase fields
        //- Interpolator for continuous phase pressure field
        autoPtr<interpolation<scalar> > pInterp_;
  public:
    typedef typename ParcelType::template TrackingData<CloudType>::trackPart
      trackPart;
    // Constructors
      //- Construct from components
      inline TrackingData
      (
        CloudType& cloud,
        trackPart part = ParcelType::template
          TrackingData<CloudType>::tpLinearTrack
      );
    // Member functions
      //- Return const access to the interpolator for continuous phase
      //  pressure field
      inline const interpolation<scalar>& pInterp() const;
  };
protected:
  // Protected data
    // Parcel properties
      //- Initial mass [kg]
      scalar mass0_;
      //- Mass fractions of mixture []
      scalarField Y_;
    // Cell-based quantities
      //- Pressure [Pa]
      scalar pc_;
  // Protected Member Functions
    //- Calculate Phase change
    template<class TrackData>
    void calcPhaseChange
    (
      TrackData& td,
      const scalar dt,           // timestep
      const label cellI,         // owner cell
      const scalar Re,           // Reynolds number
      const scalar Pr,           // Prandtl number
      const scalar Ts,           // Surface temperature
      const scalar nus,          // Surface kinematic viscosity
      const scalar d,            // diameter
      const scalar T,            // temperature
      const scalar mass,         // mass
      const label idPhase,       // id of phase involved in phase change
      const scalar YPhase,       // total mass fraction
      const scalarField& YComponents, // component mass fractions
      scalarField& dMassPC,      // mass transfer - local to parcel
      scalar& Sh,                // explicit parcel enthalpy source
      scalar& N,                 // flux of species emitted from parcel
      scalar& NCpW,              // sum of N*Cp*W of emission species
      scalarField& Cs            // carrier conc. of emission species
    );
    //- Update mass fraction
    scalar updateMassFraction
    (
      const scalar mass0,
      const scalarField& dMass,
      scalarField& Y
    ) const;
public:
  // Static data members
    //- Runtime type information
    TYPE_NAME("ReactingParcel");
    //- String representation of properties
    AddToPropertyList
    (
      ParcelType,
      " mass0"
     + " nPhases(Y1..YN)"
    );
  // Constructors
    //- Construct from owner, position, and cloud owner
    //  Other properties initialised as null
    inline ReactingParcel
    (
      const polyMesh& mesh,
      const vector& position,
      const label cellI,
      const label tetFaceI,
      const label tetPtI
    );
    //- Construct from components
    inline ReactingParcel
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
      const constantProperties& constProps
    );
    //- Construct from Istream
    ReactingParcel
    (
      const polyMesh& mesh,
      Istream& is,
      bool readFields = true
    );
    //- Construct as a copy
    ReactingParcel
    (
      const ReactingParcel& p,
      const polyMesh& mesh
    );
    //- Construct as a copy
    ReactingParcel(const ReactingParcel& p);
    //- Construct and return a (basic particle) clone
    virtual autoPtr<particle> clone() const
    {
      return autoPtr<particle>(new ReactingParcel<ParcelType>(*this));
    }
    //- Construct and return a (basic particle) clone
    virtual autoPtr<particle> clone(const polyMesh& mesh) const
    {
      return autoPtr<particle>
      (
        new ReactingParcel<ParcelType>(*this, mesh)
      );
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
      autoPtr<ReactingParcel<ParcelType> > operator()(Istream& is) const
      {
        return autoPtr<ReactingParcel<ParcelType> >
        (
          new ReactingParcel<ParcelType>(mesh_, is, true)
        );
      }
    };
  // Member Functions
    // Access
      //- Return const access to initial mass [kg]
      inline scalar mass0() const;
      //- Return const access to mass fractions of mixture []
      inline const scalarField& Y() const;
      //- Return the owner cell pressure [Pa]
      inline scalar pc() const;
      //- Return reference to the owner cell pressure [Pa]
      inline scalar& pc();
    // Edit
      //- Return access to initial mass [kg]
      inline scalar& mass0();
      //- Return access to mass fractions of mixture []
      inline scalarField& Y();
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
      //- Correct surface values due to emitted species
      template<class TrackData>
      void correctSurfaceValues
      (
        TrackData& td,
        const label cellI,
        const scalar T,
        const scalarField& Cs,
        scalar& rhos,
        scalar& mus,
        scalar& Prs,
        scalar& kappas
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
      //- Write - composition supplied
      template<class CloudType>
      static void writeFields(const CloudType& c);
  // Ostream Operator
    friend Ostream& operator<< <ParcelType>
    (
      Ostream&,
      const ReactingParcel<ParcelType>&
    );
};
}  // namespace mousse
#include "_reacting_parcel_i.hpp"
#include "_reacting_parcel_tracking_data_i.hpp"
#ifdef NoRepository
  #include "_reacting_parcel.cpp"
#endif
#endif
