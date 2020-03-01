#pragma once

// -- std headers
#include <map>
#include <string>
#include <optional>

// -- marlin headers
#include <marlin/Property.h>

namespace marlin {
  
  /**
   *  @brief  Algorithm class. Base component for event processing.
   *
   *  TODO: Write a nice documentation here
   *
   *  @author: Remi Ete
   */
  class Algorithm : public Configurable {
  public:
    /**
     *  @brief  ERuntimeOption enumerator.
     *  Runtime options constrains the execution of an algorithm 
     *  in the scheduling.  
     */
    enum class ERuntimeOption {
      eCritical,    /// Whether the algorithm has to be executed in a critical section
      eCopy         /// Whether the algorithm must be cloned in each thread worker (MT mode)
    };
    using RuntimeOptions = std::map<ERuntimeOption, bool> ;
    
  public:
    // Default constructor. No copy
    Algorithm() = default ;
    Algorithm(const Algorithm &) = delete ;
    Algorithm &operator=(const Algorithm &) = delete ;
    virtual ~Algorithm() = default ;
    
    /**
     *  @brief  Called once at initialization time by the application
     *  Algorithm parameters are already set at this point and can be used. 
     */
    virtual void init() { /* nop */ }
    
    /**
     *  @brief  Called every time a new run starts.
     *  Note that this function is always called before any event is processed.
     *  If a new run is started, the previous one is ended by calling 
     *  endOfRun() first. 
     *  In parallel event processing, note that this method is never called
     *  concurrently, so it is assumed to be thread safe by construction.
     *  Use MarlinApi::getRunInfo(this) to get the current run info
     */
    virtual void startOfRun() { /* nop */ }
    
    /**
     *  @brief  Run the algorithm. The main work horse.
     *  Depending on the scheduler implementation, this method could be 
     *  called concurrently.
     *
     *  @see setRuntimeOption
     */
    virtual void run() { /* nop */ }
    
    /**
     *  @brief  Called every time a run ends.
     *  Note that if new run is started, the current run is first ended.
     *  In case the application terminates without ending the current run,
     *  the framework also calls this function before terminating.
     *  In parallel event processing, this method is never called
     *  concurrently, so it is assumed to be thread safe by construction.
     *  Use MarlinApi::getRunInfo(this) to get the current run info
     */
    virtual void endOfRun() { /* nop */ }
    
    /**
     *  @brief  Called once when the application ends
     *  Common usage:
     *    - memory cleanup
     *    - statistics treatment
     *    - histogram fitting
     *    - etc ...
     */
    virtual void end() { /* nop */ }
    
    /**
     *  @brief  Get the algorithm type
     */
    inline const std::string &type() const {
      return _type ;
    }
    
    /**
     *  @brief  Get the algorithm name
     */
    inline const std::string &name() const {
      return _name ;
    }
    
    /**
     *  @brief  Get the algorithm description
     */
    inline const std::string &description() const {
      return _description ;
    }

    /**
     *  @brief  Set the algorithm description
     */
    inline void setDescription( const std::string &desc ) {
      _description = desc ;
    }

    /**
     *  @brief  Get an algorithm runtime option.
     *  
     *  @param  opt the runtime option
     */
    inline std::optional<bool> runtimeOption( ERuntimeOption opt ) const {
      auto iter = _runtimeOptions.find( opt ) ;
      if( _runtimeOptions.end() == iter ) {
        return  std::nullopt ;
      }
      return iter->second ;
    }
    
    /**
     *  @brief  Set an algorithm runtime option.
     * 
     *  @param opt the runtime option to set
     *  @param value the boolean value
     */
    inline void setRuntimeOption( ERuntimeOption opt, bool value ) {
      _runtimeOptions[ opt ] = value ;
    }
    
  private:
    /**
     *  @brief  Set the algorithm type
     * 
     *  @param  ty the algorithm type
     */
    inline void setType( const std::string &ty ) {
      _type = ty ;
    }
    
    /**
     *  @brief  Set the algorithm name
     * 
     *  @param  na the algorithm name
     */
    inline void setName( const std::string &na ) {
      _name = na ;
    }
    
  private:
    /// The algorithm type
    std::string                              _type ;
    /// The algorithm name
    std::string                              _name {""} ;
    /// The algorithm description
    std::string                              _description {""} ;
    /// The runtime options for parallel processing
    RuntimeOptions                           _runtimeOptions {} ;
  };
  
}