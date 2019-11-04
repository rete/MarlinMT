#include <marlin/ProcessorApi.h>

// -- marlin headers
#include <marlin/Exceptions.h>
#include <marlin/Application.h>
#include <marlin/Processor.h>
#include <marlin/EventExtensions.h>

// -- lcio headers
#include <EVENT/LCEvent.h>

namespace marlin {

  void ProcessorApi::registerForRandomSeeds( Processor *const proc ) {
    proc->app().randomSeedManager().addEntry( proc ) ;
  }

  //--------------------------------------------------------------------------

  unsigned int ProcessorApi::getRandomSeed( const Processor *const proc, EVENT::LCEvent *event ) {
    auto randomSeeds = event->runtime().ext<RandomSeed>() ;
    if( nullptr == randomSeeds ) {
      MARLIN_THROW( "No random seed extension in event" ) ;
    }
    return randomSeeds->randomSeed( proc ) ;
  }

  //--------------------------------------------------------------------------

  void ProcessorApi::setReturnValue( const Processor *const proc, EVENT::LCEvent *event, bool value ) {
    auto procConds = event->runtime().ext<ProcessorConditions>() ;
    if( nullptr == procConds ) {
      MARLIN_THROW( "No processor conditions extension in event" ) ;
    }
    return procConds->set( proc, value ) ;
  }

  //--------------------------------------------------------------------------

  void ProcessorApi::setReturnValue( const Processor *const proc, EVENT::LCEvent *event, const std::string &name, bool value ) {
    auto procConds = event->runtime().ext<ProcessorConditions>() ;
    if( nullptr == procConds ) {
      MARLIN_THROW( "No processor conditions extension in event" ) ;
    }
    return procConds->set( proc, name, value ) ;
  }

  //--------------------------------------------------------------------------

  bool ProcessorApi::isFirstEvent( EVENT::LCEvent *event ) {
    return event->runtime().ext<IsFirstEvent>() ;
  }

  //--------------------------------------------------------------------------

  void ProcessorApi::skipCurrentEvent( const Processor *const proc ) {
    proc->log<WARNING>() << "Skipping current event !" << std::endl ;
    MARLIN_SKIP_EVENT( proc ) ;
  }

  //--------------------------------------------------------------------------

  void ProcessorApi::abort( const Processor *const proc, const std::string &reason ) {
    proc->log<WARNING>() << "Stopping application: " << reason << std::endl ;
    MARLIN_STOP_PROCESSING( proc ) ;
  }

}
