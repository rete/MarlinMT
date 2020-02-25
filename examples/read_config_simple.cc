
class SimpleAlgo : public Algorithm {
public:
  SimpleAlgo() ;
  void run() ;
  
private:
  /// Single simple properties
  marlin::Property<float>        m_threshold { this, "Threshold", 
        "The energy threshold (unit GeV)", 0.05 } ;
  marlin::Property<float>        m_calibrationConstant { this, "CalibrationConstant", 
        "The calibration constant (unit GeV)", 3.5 } ;
  /// An input property. In this case an LCIO collection
  marlin::InputProperty          m_inputCollection { this, EVENT::LCIO::SIMCALORIMETERHIT, "InputHitCollection", 
        "The input sim hit collection name" } ;
  /// An output property. In this case an LCIO collection
  marlin::OutputProperty         m_outputCollection { this, EVENT::LCIO::CALORIMETERHIT, "DigitHitCollection", 
        "The output digitized hit collection name" } ;
};

//--------------------------------------------------------------------------------------

/// Constructor with algorithm type 
void SimpleAlgo::SimpleAlgo() : Algorithm("SimpleAlgo") {}


/// Run method
void SimpleAlgo::run() {
  // Get the event from the Marlin API
  std::shared_ptr<EVENT::LCEvent> event = MarlinApi::getEvent<EVENT::LCEvent>( this ) ;
  // do stuff with event ...
}