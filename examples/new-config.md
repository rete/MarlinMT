
## API changes

Marlin:
```cpp
class Processor {
  virtual void init() ;
  virtual void processRunHeader(LCRunHeader*) ;
  virtual void processEvent(LCEvent*) ;
  virtual void end() ;
};

// Example:
MyProcessor::processRunHeader(LCRunHeader *hdr) {
  // do stuff with hdr
}

MyProcessor::processEvent(LCEvent *evt) {
  // do stuff with evt
}
```

MarlinMT:
```cpp
class Algorithm {
  virtual void init() ;
  virtual void startOfRun() ;
  virtual void run() ;
  virtual void endOfRun() ;
  virtual void end() ;
};

// Example:
Algorithm::startOfRun() {
  auto runInfo = MarlinApi::getCurrentRunInfo( this ) ;
  // do stuff with runInfo
}

Algorithm::run() {
  auto evt = MarlinApi::getEvent<LCEvent>( this ) ;
  // do stuff with evt
}

Algorithm::endOfRun() {
  auto runInfo = MarlinApi::getCurrentRunInfo( this ) ;
  // do stuff with runInfo
}
```

Marlin philosophy of API changes:

- Review the API for the long term
- Remove arguments from prototypes: 
  - use `MarlinApi` to get content
  - make the Api more robust to changes
- `Processor` -> `Algorithm`: 
  - adopt conventions from other frameworks
  - the name `Processor` is confusing with the hardware


## Nested algorithms  

Introduced nested algorithms:

```cpp
class ParentAlgo : public Algorithm {
public:
  ParentAlgo() ;
  void run() ;
  
private:
  /// Single simple property
  marlin::Property<float>        m_weightCut { this, "WeightCut", "The event weigth cut", 0.05 } ;
  /// A property configuring and describing a daughter algorithm
  marlin::AlgorithmProperty      m_daughterAlgorithm { this, "Daughter", "The daughter algorithm name"} ;
};

/// Constructor with algorithm type 
void ParentAlgo::ParentAlgo() : Algorithm("ParentAlgo") {}

/// Run method
void ParentAlgo::run() {
  // Get the event from the Marlin API
  auto event = MarlinApi::getEvent<EVENT::LCEvent>( this ) ;
  
  // Run the daughter algorithm depending on the event weigth
  if ( event->getWeigth() > m_weightCut ) {
    marlin::MarlinApi::RunDaughterAlgorithm( this, m_daughterAlgorithm.get() ) ;
  }
}
```
Why?

- Promote (small) algorithms reuse

Typical examples:

- Complicated event selection, then run daughter algorithm(s)
- Split events in sub-events (data taking) and process the full chain on sub-events
- Re-use the same algorithm instance multiple times within a parent algorithm

## Algorithm tools

Small re-usable components in algorithms. They can be algorithm-local or global.
They are always provided as `const` objects to avoid modification by multiple threads.

Example:

```cpp
class ConstantFactorTool : public AlgorithmTool {
public:
  ConstantFactorTool() : AlgorithmTool("ConstantFactorTool") {}
  
  inline float applyFactor( const float in ) const {
    return in * m_factor.get() ;
  }
  
private:
  /// A dummy factor to apply
  marlin::Property<float>        m_factor { this, "Factor", 
        "The constant factor to apply", 1 } ;
};

class SimpleAlgo : public Algorithm {
public:
  SimpleAlgo() ;
  void run() ;
  
private:
  /// Single simple property
  marlin::Property<std::string>             m_globalTool { this, "GlobalTool", 
        "The global tool name to use" } ;
  /// Local algorithm tool to configure
  marlin::ToolProperty<ConstantFactorTool>  m_tool78 { this, "ConstantFactor_78", 
        "The tool multiplying by 78 any input" } ;
  /// Local algorithm tool to configure
  marlin::ToolProperty<ConstantFactorTool>  m_tool104 { this, "ConstantFactor_104", 
        "The tool multiplying by 104 any input" } ;
};

/// Constructor with algorithm type 
void SimpleAlgo::SimpleAlgo() : Algorithm("SimpleAlgo") {}

/// Run method
void SimpleAlgo::run() {
  // use operator -> to access the algorithm tool pointer directly
  log<MESSAGE>() << "Applying factor 78: " << m_tool78->applyFactor( 42 ) << std::endl ;
  log<MESSAGE>() << "Applying factor 104: " << m_tool104->applyFactor( 42 ) << std::endl ;
  
  // Global algorithm tools can be accessed via the MarlinApi
  auto globalTool = MarlinApi::getTool<ConstantFactorTool>( this, m_globalTool.get() ) ;
  log<MESSAGE>() << "Applying a factor (global tool): " << globalTool->applyFactor( 42 ) << std::endl ;
}
```

Typical usage:

- Holds calibration constants
- Energy correction plugins
- Particle ID functions