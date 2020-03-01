#pragma once

// -- marlin headers
#include <marlin/Component.h>

namespace marlin {
  
  /**
   *  @brief  AlgorithmTool class
   *  Re-usable algorithm tool used inside algorithms. Algorithms tools
   *  can be either create locally in algorithms or created globally in
   *  the application.
   */
  class AlgorithmTool : public Component {
  public:
    /// No default constructor
    AlgorithmTool() = delete ;
    virtual ~AlgorithmTool() = default ;

    /**
     *  @brief  Constructor with algorithm tool name 
     * 
     *  @param  name the algorithm tool name
     */
    inline AlgorithmTool( const std::string &name ) : 
      Component(name) {
      /* nop */
    }
  };
  
}