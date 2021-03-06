#ifndef MARLIN_PROCESSORAPI_h
#define MARLIN_PROCESSORAPI_h 1

// -- std headers
#include <string>

// -- marlin headers
#include <marlin/Processor.h>
#include <marlin/Application.h>
#include <marlin/GeometryManager.h>
#include <marlin/MarlinConfig.h>

namespace marlin {

  class Processor ;

  /**
   *  @brief  ProcessorApi class
   *
   *  Provide a static API for processors to make high level calls.
   *  For example, to register your processor for random seeds:
   *  @code{cpp}
   *  // in your processor init() function
   *  ProcessorApi::registerForRandomSeeds( this ) ;
   *  @endcode
   */
  class ProcessorApi {
  public:
    // only static API
    ProcessorApi() = delete ;
    ~ProcessorApi() = delete ;

    /**
     *  @brief  Register the processor to get random seeds
     *
     *  @param  proc the processor to register
     */
    static void registerForRandomSeeds( Processor *const proc ) ;

    /**
     *  @brief  Get a random seed from the event.
     *  Your processor must have been registered before hand using registerForRandomSeeds()
     *
     *  @param  proc the processor instance
     *  @param  event the current event from which to get random seeds
     */
    static unsigned int getRandomSeed( const Processor *const proc, EventStore *event ) ;

    /**
     *  @brief  Set the processor return value
     *
     *  @param  proc the processor instance
     *  @param  event the event to which the condition is attached
     *  @param  value the processor return value
     */
    static void setReturnValue( const Processor *const proc, EventStore *event, bool value ) ;

    /**
     *  @brief  Set the named processor return value
     *
     *  @param  proc the processor instance
     *  @param  event the event to which the condition is attached
     *  @param  name the name of the return value
     *  @param  value the processor return value
     */
    static void setReturnValue( const Processor *const proc, EventStore *event, const std::string &name, bool value ) ;

    /**
     *  @brief  Whether the event is the first event to be processed.
     *  WARNING! Use this method with caution in multi-threading mode.
     *  The first event is the first that has been pushed in the scheduler
     *  queue, maybe not the first one received in Processor::processEvent().
     *  Don't rely on this method to initialize data on first event, but rather
     *  use the init() method for that purpose.
     *
     *  @param  event the event to test
     */
    static bool isFirstEvent( EventStore *event ) ;

    /**
     *  @brief  Get the geometry handle as a given type
     *
     *  @param  proc the processor instance initiating the call
     */
    template <typename T>
    static T *geometry( const Processor *const proc ) ;

    /**
     *  @brief  Notify the application to skip the current event processing
     *  and go directly to the next event by skipping next processors in the sequence
     *
     *  @param  proc the processor instance initiating the call
     */
    static void skipCurrentEvent( const Processor *const proc ) ;

    /**
     *  @brief  Abort program execution properly
     *
     *  @param  proc the processor initiating the abort call
     *  @param  reason the reason why the processor aborts the program
     */
    static void abort( const Processor *const proc, const std::string &reason ) ;
  };

  //--------------------------------------------------------------------------
  //--------------------------------------------------------------------------

  template <typename T>
  inline T *ProcessorApi::geometry( const Processor *const proc ) {
    return proc->app().geometryManager().geometry<T>() ;
  }

}

#endif
