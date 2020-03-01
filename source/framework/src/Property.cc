
#include <marlin/Property.h>
#include <marlin/Algorithm.h>
#include <marlin/AlgorithmTool.h>

namespace marlin {
  
  namespace experimental {  
  
  AlgorithmProperty::AlgorithmProperty( Algorithm *alg, const std::string &key, const std::string &desc ) :
    PropertyBase<std::string>( *alg, EPropertyType::eAlgorithm, key, desc ) {
    // TODO register/create an algorithm 
  }
  
  
  AlgorithmListProperty::AlgorithmListProperty( Algorithm *alg, const std::string &key, const std::string &desc ) :
    PropertyBase<std::vector<std::string>>( *alg, EPropertyType::eAlgorithm, key, desc ) {
    // TODO register/create an algorithm 
  }
  
  
  ToolProperty::ToolProperty( AlgorithmTool *t, const std::string &key, const std::string &desc ) :
    PropertyBase<std::string>( *t, EPropertyType::eAlgorithmTool, key, desc ) {
    // TODO register/create algorithm tool
  }
  
  }
 
}