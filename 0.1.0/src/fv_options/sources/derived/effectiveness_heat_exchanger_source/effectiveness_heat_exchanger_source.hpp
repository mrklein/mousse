// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::fv::effectivenessHeatExchangerSource
// Description
//   Heat exchanger source model, in which the heat exchanger is defined as a
//   selection of cells.
//   The total heat exchange source is given by:
//   \f[
//     Q_t = e(\phi, \dot{m}_2) (T_2 - T_1) \phi c_p
//   \f]
//   where:
//   \vartable
//     Q_t       | total heat source
//     e(\phi,\dot{m}_2) | effectivenes table
//     \phi      | net mass flux entering heat exchanger [kg/s]
//     \dot{m}_2 | secondary mass flow rate [kg/s]
//     T_1       | primary inlet temperature [K]
//     T_2       | secondary inlet temperature [K]
//     c_p       | specific heat capacity [J/kg/K]
//   \endvartable
//   The distribution inside the hear exchanger is given by:
//   \f[
//     Q_c = \frac{V_c |U_c| (T_c - T_{ref})}{\sum(V_c |U_c| (T_c - T_{ref}))}
//   \f]
//   where:
//   \vartable
//     Q_c     | source for cell
//     V_c     | volume of the cell [m3]
//     U_c     | local cell velocity [m/s]
//     T_c     | local call temperature [K]
//     T_{ref} | min or max(T) in cell zone depending on the sign of Q_t [K]
//   \endvartable
//   \heading Source usage
//   Example usage:
//   \verbatim
//   effectivenessHeatExchangerSource1
//   {
//     type            effectivenessHeatExchangerSource;
//     active          yes;
//     effectivenessHeatExchangerSourceCoeffs
//     {
//       selectionMode   cellZone;
//       cellZone        porosity;
//       secondaryMassFlowRate   1.0;
//       secondaryInletT         336;
//       primaryInletT           293;
//       faceZone                facesZoneInletOriented;
//       outOfBounds             clamp;
//       fileName                "effTable";
//     }
//   }
//   \endverbatim
//   The effectiveness table is described in terms of the primary and secondary
//   mass flow rates.  For example, the table:
//              secondary MFR
//            |  0.1   0.2   0.3
//          -----+-----------------
//          0.02 |   A     B     C
//   primary MFR  0.04 |   D     E     F
//          0.06 |   G     H     I
//   Is specified by the following:
//     (
//       0.02
//       (
//         (0.1    A)
//         (0.2    B)
//         (0.3    C)
//       ),
//       0.04
//       (
//         (0.1    D)
//         (0.2    E)
//         (0.3    F)
//       ),
//       0.06
//       (
//         (0.1    G)
//         (0.2    H)
//         (0.3    I)
//       )
//     );
// Note
// - the table with name "fileName" should have the same units as the
//  secondary mass flow rate and kg/s for phi
// - faceZone is the faces at the inlet of the cellzone, it needs to be
//  created with flip map flags. It is used to integrate the net mass flow
//  rate into the heat exchanger
// SourceFiles
//   effectiveness_heat_exchanger_source.cpp
#ifndef effectiveness_heat_exchanger_source_hpp_
#define effectiveness_heat_exchanger_source_hpp_
#include "cell_set_option.hpp"
#include "auto_ptr.hpp"
#include "interpolation_2d_table.hpp"
namespace mousse
{
namespace fv
{
class effectivenessHeatExchangerSource
:
  public cellSetOption
{
protected:
  // Protected data
    //- Secondary flow mass rate [kg/s]
    scalar secondaryMassFlowRate_;
    //- Inlet secondary temperature [K]
    scalar secondaryInletT_;
    //- Primary air temperature at the heat exchanger inlet [K]
    scalar primaryInletT_;
    //- 2D look up table efficiency = function of primary and secondary
    // mass flow rates [kg/s]
    autoPtr<interpolation2DTable<scalar> > eTable_;
    //- Name of velocity field; default = U
    word UName_;
    //- Name of temperature field; default = T
    word TName_;
    //- Name of the flux
    word phiName_;
    //- Name of the faceZone at the heat exchange inlet
    word faceZoneName_;
    //- Id for the face zone
    label zoneID_;
    //- Local list of face IDs
    labelList faceId_;
    //- Local list of patch ID per face
    labelList facePatchId_;
    //- List of +1/-1 representing face flip map (1 use as is, -1 negate)
    labelList faceSign_;
    //- Area of the face zone
    scalar faceZoneArea_;
private:
  // Private Member Functions
    //- Initialise heat exchanger source model
    void initialise();
    //- Calculate total area of faceZone accross processors
    void calculateTotalArea(scalar& area);
public:
  //- Runtime type information
  TYPE_NAME("effectivenessHeatExchangerSource");
  // Constructors
    //- Construct from components
    effectivenessHeatExchangerSource
    (
      const word& name,
      const word& modelType,
      const dictionary& dict,
      const fvMesh& mesh
    );
    //- Disallow default bitwise copy construct
    effectivenessHeatExchangerSource
    (
      const effectivenessHeatExchangerSource&
    ) = delete;
    //- Disallow default bitwise assignment
    effectivenessHeatExchangerSource& operator=
    (
      const effectivenessHeatExchangerSource&
    ) = delete;
  //- Destructor
  virtual ~effectivenessHeatExchangerSource()
  {}
  // Member Functions
    // Explicit and implicit source
      //- Scalar
      virtual void addSup
      (
        fvMatrix<scalar>& /*eqn*/,
        const label /*fieldI*/
      )
      {
        NOT_IMPLEMENTED
        (
          "effectivenessHeatExchangerSource::addSup(eqn, fieldI): "
          "only compressible solvers supported."
        );
      }
    // Explicit and implicit source for compressible equation
      //- Scalar
      virtual void addSup
      (
        const volScalarField& rho,
        fvMatrix<scalar>& eqn,
        const label fieldI
      );
    // IO
      //- Read dictionary
      virtual bool read(const dictionary& dict);
};
}  // namespace fv
}  // namespace mousse
#endif
