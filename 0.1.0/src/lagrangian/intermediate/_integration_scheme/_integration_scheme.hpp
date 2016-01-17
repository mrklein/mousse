// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::IntegrationScheme
// Description
//   Top level model for Integration schemes
// SourceFiles
//   _integration_scheme.cpp
#ifndef _integration_scheme_hpp_
#define _integration_scheme_hpp_
#include "auto_ptr.hpp"
#include "run_time_selection_tables.hpp"
#include "dictionary.hpp"
namespace mousse
{
template<class Type>
class IntegrationScheme
{
public:
  //- Helper class to supply results of integration
  class integrationResult
  {
    //- Integration value
    Type value_;
    //- Average value across integration step
    Type average_;
  public:
    //- Constructor
    integrationResult()
    :
      value_(pTraits<Type>::zero),
      average_(pTraits<Type>::zero)
    {}
    // Member functions
      // Access
        //- Return const access to the value
        Type value() const
        {
          return value_;
        }
        //- Return const access to the average
        Type average() const
        {
          return average_;
        }
      // Edit
        //- Return access to the value for changing
        Type& value()
        {
          return value_;
        }
        //- Return access to the average for changing
        Type& average()
        {
          return average_;
        }
    };
private:
  // Private data
    //- Name of the Integration variable
    const word& phiName_;
    //- Reference to the dictionary
    const dictionary& dict_;
public:
  //- Runtime type information
  TYPE_NAME("integrationScheme");
  //- Declare runtime constructor selection table
    DECLARE_RUN_TIME_SELECTION_TABLE
    (
      autoPtr,
      IntegrationScheme,
      dictionary,
      (
        const word& phiName,
        const dictionary& dict
      ),
      (phiName, dict)
    );
  // Constructors
    //- Construct from components
    IntegrationScheme(const word& phiName, const dictionary& dict);
    //- Copy constructor
    IntegrationScheme(const IntegrationScheme& is);
    //- Construct and return clone
    virtual autoPtr<IntegrationScheme<Type> > clone() const = 0;
    //- Disallow default bitwise assignment
    IntegrationScheme& operator=(const IntegrationScheme&) = delete;
  // Selectors
    //- Return a reference to the selected radiation model
    static autoPtr<IntegrationScheme> New
    (
      const word& phiName,
      const dictionary& dict
    );
  //- Destructor
  virtual ~IntegrationScheme();
  // Member Functions
    //- Perform the Integration
    virtual integrationResult integrate
    (
      const Type& phi,
      const scalar dt,
      const Type& alphaBeta,
      const scalar beta
    ) const = 0;
};
}  // namespace mousse

#define MAKE_INTEGRATION_SCHEME(Type)                                         \
                                                                              \
  DEFINE_NAMED_TEMPLATE_TYPE_NAME_AND_DEBUG(IntegrationScheme<Type>, 0);      \
                                                                              \
  DEFINE_TEMPLATE_RUN_TIME_SELECTION_TABLE                                    \
  (                                                                           \
    IntegrationScheme<Type>,                                                  \
    dictionary                                                                \
  );

#define MAKE_INTEGRATION_SCHEME_TYPE(SS, Type)                                \
                                                                              \
  DEFINE_NAMED_TEMPLATE_TYPE_NAME_AND_DEBUG(SS<Type>, 0);                     \
                                                                              \
  IntegrationScheme<Type>::adddictionaryConstructorToTable<SS<Type> >         \
    add##SS##Type##ConstructorToTable_;

#ifdef NoRepository
#   include "_integration_scheme.cpp"
#endif
#endif
