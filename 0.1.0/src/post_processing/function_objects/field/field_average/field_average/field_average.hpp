// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::fieldAverage
// Group
//   grpFieldFunctionObjects
// Description
//   This function object calculates average quantities for a user-specified
//   selection of volumetric and surface fields.  Fields are entered as a list
//   of sub-dictionaries, which indicate the type of averages to perform, and
//   can be updated during the calculation.  The current options include:
//   - \c mean: arithmetic mean:
//     \f[
//       \overline{x} = \frac{1}{N}\displaystyle\sum\limits_{i=0}^N x_i
//     \f]
//   - \c prime2Mean: prime-squared mean
//     \f[
//       \overline{x'}^2 = \frac{1}{N}\displaystyle\sum\limits_{i=0}^N
//       (x_i - \overline{x})^2
//     \f]
//   - base: average over 'time', or 'iteration' (\f$N\f$ in the above)
//   - window: optional averaging window, specified in 'base' units
//   Average field names are constructed by concatenating the base field with
//   the averaging type, e.g. when averaging field 'U', the resultant fields
//   are:
//   - arithmetic mean field, UMean
//   - prime-squared field, UPrime2Mean
//   Information regarding the number of averaging steps, and total averaging
//   time are written on a per-field basis to the
//   \c fieldAveragingProperties dictionary, located in \<time\>/uniform
//   When restarting form a previous calculation, the averaging is continuous.
//   However, the averaging process can be restarted using the \c resetOnRestart
//   option.
//   To restart the averaging process after each calculation output time, use
//   the \c resetOnOutput option.
//   Example of function object specification:
//   \verbatim
//   fieldAverage1
//   {
//     type fieldAverage;
//     functionObjectLibs ("libfieldFunctionObjects.so");
//     ...
//     resetOnRestart true;
//     resetOnOutput false;
//     fields
//     (
//       U
//       {
//         mean            on;
//         prime2Mean      on;
//         base            time;
//         window          10.0;
//         windowName      w1;
//       }
//       p
//       {
//         mean            on;
//         prime2Mean      on;
//         base            time;
//       }
//     );
//   }
//   \endverbatim
//   \heading Function object usage
//   \table
//     Property     | Description             | Required    | Default value
//     type         | type name: fieldAverage | yes         |
//     resetOnRestart | flag to reset the averaging on restart | yes  |
//     resetOnOutput| flag to reset the averaging on output | yes |
//     fields       | list of fields and averaging options | yes |
//   \endtable
// Note
//   To employ the \c prime2Mean option, the \c mean option must be selecetd.
// SeeAlso
//   mousse::functionObject
//   mousse::OutputFilterFunctionObject
// SourceFiles
//   field_average.cpp
//   field_average_templates.cpp
//   field_average_item.cpp
#ifndef field_average_hpp_
#define field_average_hpp_
#include "vol_fields_fwd.hpp"
#include "switch.hpp"
namespace mousse
{
// Forward declaration of classes
class objectRegistry;
class dictionary;
class fieldAverageItem;
template<class Type>
class List;
class polyMesh;
class mapPolyMesh;
class fieldAverage
{
protected:
  // Protected data
    //- Name of this set of field averages.
    word name_;
    //- Database this class is registered to
    const objectRegistry& obr_;
    //- On/off switch
    bool active_;
    //- Time at last call, prevents repeated averaging
    label prevTimeIndex_;
    //- Reset the averaging process on restart flag
    Switch resetOnRestart_;
    //- Reset the averaging process on output flag
    Switch resetOnOutput_;
    //- Initialised flag
    bool initialised_;
    //- List of field average items, describing what averages to be
    //  calculated and output
    List<fieldAverageItem> faItems_;
    // Counters
      //- Iteration steps counter
      List<label> totalIter_;
      //- Total time counter
      List<scalar> totalTime_;
  // Private Member Functions
    // Initialisation routines
      //- Checkout fields (causes deletion) from the database
      //  and reset lists
      void resetFields();
      //- Reset lists (clear existing values) and initialize averaging.
      //  Check requested field averages are valid, populate field lists
      void initialize();
      //- Add mean average field to database
      template<class Type>
      void addMeanFieldType(const label fieldI);
      //- Add mean average field to database
      template<class Type>
      void addMeanField(const label fieldI);
      //- Add prime-squared average field to database
      template<class Type1, class Type2>
      void addPrime2MeanFieldType(const label fieldI);
      //- Add prime-squared average field to database
      template<class Type1, class Type2>
      void addPrime2MeanField(const label fieldI);
    // Calculation functions
      //- Main calculation routine
      virtual void calcAverages();
      //- Calculate mean average fields
      template<class Type>
      void calculateMeanFieldType(const label fieldI) const;
      //- Calculate mean average fields
      template<class Type>
      void calculateMeanFields() const;
      //- Calculate prime-squared average fields
      template<class Type1, class Type2>
      void calculatePrime2MeanFieldType(const label fieldI) const;
      //- Calculate prime-squared average fields
      template<class Type1, class Type2>
      void calculatePrime2MeanFields() const;
      //- Add mean-squared field value to prime-squared mean field
      template<class Type1, class Type2>
      void addMeanSqrToPrime2MeanType(const label fieldI) const;
      //- Add mean-squared field value to prime-squared mean field
      template<class Type1, class Type2>
      void addMeanSqrToPrime2Mean() const;
    // I-O
      //- Write averages
      virtual void writeAverages() const;
      //- Write fields
      template<class Type>
      void writeFieldType(const word& fieldName) const;
      //- Write fields
      template<class Type>
      void writeFields() const;
      //- Write averaging properties - steps and time
      void writeAveragingProperties() const;
      //- Read averaging properties - steps and time
      void readAveragingProperties();
public:
  //- Runtime type information
  TYPE_NAME("fieldAverage");
  // Constructors
    //- Construct for given objectRegistry and dictionary.
    //  Allow the possibility to load fields from files
    fieldAverage
    (
      const word& name,
      const objectRegistry&,
      const dictionary&,
      const bool loadFromFiles = false
    );
    //- Disallow default bitwise copy construct
    fieldAverage(const fieldAverage&) = delete;
    //- Disallow default bitwise assignment
    fieldAverage& operator=(const fieldAverage&) = delete;
  //- Destructor
  virtual ~fieldAverage();
  // Member Functions
    //- Return name of the set of field averages
    virtual const word& name() const
    {
      return name_;
    }
    //- Read the field average data
    virtual void read(const dictionary&);
    //- Execute the averaging
    virtual void execute();
    //- Execute the averaging at the final time-loop, currently does nothing
    virtual void end();
    //- Called when time was set at the end of the Time::operator++
    virtual void timeSet();
    //- Calculate the field average data and write
    virtual void write();
    //- Update mesh
    virtual void updateMesh(const mapPolyMesh&);
    //- Move points
    virtual void movePoints(const polyMesh&);
};
}  // namespace mousse
#ifdef NoRepository
#   include "field_average_templates.cpp"
#endif
#endif
