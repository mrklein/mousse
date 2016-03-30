#ifndef CORE_DB_FUNCTION_OBJECTS_OUTPUT_FILTER_OUTPUT_CONTROL_HPP_
#define CORE_DB_FUNCTION_OBJECTS_OUTPUT_FILTER_OUTPUT_CONTROL_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

// Class
//   mousse::outputFilterOutputControl
// Description
//   An output control for function objects.
//   The default is time-step execution at every interval.
// SourceFiles
//   output_filter_output_control.cpp

#include "dictionary.hpp"
#include "time.hpp"
#include "named_enum.hpp"


namespace mousse {

class outputFilterOutputControl
{

public:

  //- The output control options
  enum outputControls
  {
    ocTimeStep,       /*!< execution is coupled to the time-step */
    ocOutputTime,     /*!< execution is coupled to the output-time */
    ocAdjustableTime, /*!< Adjust time step for dumping */
    ocRunTime,        /*!< run time for dumping */
    ocClockTime,      /*!< clock time for dumping */
    ocCpuTime,        /*!< cpu time for dumping */
    ocNone            /*!< no output */
  };

private:

  // Private data

    //- Time object
    const Time& time_;

    //- Prefix
    const word prefix_;

    //- String representation of outputControls enums
    static const NamedEnum<outputControls, 7> outputControlNames_;

    //- Type of output
    outputControls outputControl_;

    //- The execution interval (in time steps) when using \c timeStep mode,
    //  a value <= 1 means execute at every time step
    label outputInterval_;

    //- Dumping counter for ocOutputTime or index dump for ocAdjustableTime
    label outputTimeLastDump_;

    //- Dump each deltaT (adjust Ttime)
    scalar writeInterval_;

public:

  // Constructors

    //- Construct from Time object and dictionary
    outputFilterOutputControl
    (
      const Time&,
      const dictionary&,
      const word& prefix
    );

    //- Disallow default bitwise copy construct and assignment
    outputFilterOutputControl(const outputFilterOutputControl&) = delete;

    outputFilterOutputControl& operator=
    (
      const outputFilterOutputControl&
    ) = delete;

  //- Destructor
  ~outputFilterOutputControl();

  // Member Functions
    //- Read from dictionary
    void read(const dictionary&);
    //- Return const access to the Time object
    const Time& time() const
    {
      return time_;
    }

    //- Flag to indicate whether to output
    bool output();

    //- Return outputControl
    outputControls outputControl() const
    {
      return outputControl_;
    }

    //- Return writeInterval
    scalar writeInterval() const
    {
      return writeInterval_;
    }

    //- Return outputTimeLastDump
    label outputTimeLastDump() const
    {
      return outputTimeLastDump_;
    }

};

}  // namespace mousse
#endif
