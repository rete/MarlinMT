#pragma once

#include <string>
#include <memory>

namespace marlin {
  
  class Algorithm ;
  class RunInfo ;
  
  class MarlinApi {
  public:
    /// No constructor, static Api only
    MarlinApi() = delete ;
    ~MarlinApi() = delete ;
    
    /// Register the algorithm in the random seeds generator
    static void registerForRandomSeeds( Algorithm *algo ) ;
    /// Get a random seed. Requires a registration before makeing a call
    static unsigned int randomSeed( const Algorithm *const algo ) ;
    
    /// Set a runtime flag boolean value. Valid for a single event processing period 
    static void setRuntimeFlag( const Algorithm *const algo, const std::string &name, bool value ) ;
    /// Get a runtime flag by name
    static bool runtimeFlag( const Algorithm *const algo, const std::string &name ) ;
    
    /// Access the geometry
    template <typename HANDLER>
    static const HANDLER* geometry( const Algorithm *const algo ) ;

    /// Access the current event data
    template <typename T>
    static std::shared_ptr<T> event( const Algorithm *const algo ) ;
    
    /// Get the current information
    static const RunInfo &currentRunInfo( const Algorithm *const algo ) ;
    
    /// Stop the current event processing and go to next event if available
    static void skipCurrentEvent( const Algorithm *const algo ) ;
    /// Abort the application
    static void abort( const Algorithm *const algo ) ;
    /// Abort the application, specifying a reason
    static void abort( const Algorithm *const algo, const std::string &reason ) ;
    
    /// Get an algorithm tool from the global pool
    template <typename T>
    static const T* globalAlgorithmTool( const Algorithm *const algo, const std::string &name ) ;

    /// Run a daughter algorithm
    static void runDaughterAlgorithm( const Algorithm *const parent, const std::string &name ) ;
  };
  
}