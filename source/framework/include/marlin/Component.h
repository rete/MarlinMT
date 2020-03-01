#pragma once

#include <marlin/Logging.h>

namespace marlin {
  
  class Application ;
  
  /**
   *  @brief  Component class.
   *  Base class for all application components. Groups a few feature common
   *  too all application components:
   *   - configuration via properties
   *   - logger access
   *   - access to application
   */
  class Component : public Configurable {
  public:    
    friend class Application ; 
    using LoggerPtr = Logging::Logger ;
    
  public:
    /// Default constructor
    Component() = default ;
    
    /// Default destructor
    virtual ~Component() = default ;
    
    /**
     *  @brief  Get the application in which the component is registered
     */
    inline const Application &application() const {
      if( nullptr == _application ) {
        MARLIN_THROW( "Application not set" ) ;
      }
      return *_application ;
    }
    
    /**
     *  @brief  Get the application in which the component is registered
     */
    inline Application &application() {
      if( nullptr == _application ) {
        MARLIN_THROW( "Application not set" ) ;
      }
      return *_application ;
    }
    
    template <class T>
    inline Logging::StreamType log() const {
      return _logger->log<T>() ;
    }
    
    inline Logging::StreamType debug() const {
      return log<loglevel::DEBUG>() ;
    }
    
  private:
    /// Setup the component. Called by the application before initialization
    inline void setup( Application *app ) ;
    
  private:
    Application             *_application {nullptr} ;
    LoggerPtr                _logger {nullptr} ;
  };
  
}