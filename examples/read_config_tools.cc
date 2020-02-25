

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

//--------------------------------------------------------------------------------------

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



