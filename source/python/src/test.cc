

#include <boost/python.hpp>
#include <boost/python/list.hpp>
#include <boost/python/make_function.hpp>
#include <boost/python/return_internal_reference.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include <boost/python/extract.hpp>
#include <boost/function.hpp>

#include <string>
#include <memory>

#include <marlin/PluginManager.h>
#include <marlin/StringParameters.h>

namespace pymarlin {
  
  template <typename T>
  struct Vec {
    Vec( const boost::python::list &l ) {
      auto count = boost::python::len( l ) ;
      for( decltype(count) index=0 ; index<count ; index++ ) {
        boost::python::extract<T> extr( l[index] );
        if( extr.check() ) {
          _container.push_back( extr() );
        }
      }
    }
    
    std::vector<T>     _container {} ;
  };
  
  typedef Vec<int> IntVec ;
  typedef Vec<float> FloatVec ;
  typedef Vec<std::string> StringVec ;
  
  struct ProcessorConfig {
    friend std::ostream &operator <<(std::ostream &s, const ProcessorConfig &c );
    
    ProcessorConfig() = default ;
    ~ProcessorConfig() = default ;
    
    ProcessorConfig( const std::string &t, const std::string &n ) : _type(t), _name(n) {}
    
    void setName( const std::string &n ) { _name = n ; }
    const std::string &getName() const { return _name ; }
    
    void setType( const std::string &t ) { _type = t ; }
    const std::string &getType() const { return _type ; }
    
    template <typename T>
    ProcessorConfig &set( const std::string &name, const T &value ) {
      _parameters->replace( name, value ) ;
      return *this ;
    }
    
    template <typename T>
    ProcessorConfig &set( const std::string &name, const Vec<T> &values ) {
      _parameters->replace( name, values._container ) ;
      return *this ;
    }
    
  private:
    std::string                           _type {} ;
    std::string                           _name {} ;
    std::shared_ptr<marlin::StringParameters>     _parameters {std::make_shared<marlin::StringParameters>()} ;
  };
  
  std::ostream &operator <<(std::ostream &s, const ProcessorConfig &c ) {
    s <<  c._type << "." << c._name << std::endl ;
    s << *c._parameters << std::endl ;
    return s ;
  }
  
  // Wrapper for marlin::PluginManager singleton
  class PluginManager {
  public:
    PluginManager() = default ;
    ~PluginManager() = default ;
    
    void loadLibraries( const std::string &env = "MARLIN_DLL" ) {
      marlin::PluginManager::instance().loadLibraries( env ) ;
    }
    
    void dump() {
      marlin::PluginManager::instance().dump() ;
    }
    
  };  

}

using namespace boost::python;

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(PluginManager_loadLibraries_overloads, pymarlin::PluginManager::loadLibraries, 0, 1)

BOOST_PYTHON_MODULE(pymarlin) {
  
  pymarlin::ProcessorConfig& (pymarlin::ProcessorConfig::*setI)(const std::string&, const int&) = &pymarlin::ProcessorConfig::set;
  pymarlin::ProcessorConfig& (pymarlin::ProcessorConfig::*setF)(const std::string&, const float&) = &pymarlin::ProcessorConfig::set;
  pymarlin::ProcessorConfig& (pymarlin::ProcessorConfig::*setD)(const std::string&, const double&) = &pymarlin::ProcessorConfig::set;
  pymarlin::ProcessorConfig& (pymarlin::ProcessorConfig::*setB)(const std::string&, const bool&) = &pymarlin::ProcessorConfig::set;
  pymarlin::ProcessorConfig& (pymarlin::ProcessorConfig::*setS)(const std::string&, const std::string&) = &pymarlin::ProcessorConfig::set;
  pymarlin::ProcessorConfig& (pymarlin::ProcessorConfig::*setVI)(const std::string&, const pymarlin::IntVec&) = &pymarlin::ProcessorConfig::set;
  pymarlin::ProcessorConfig& (pymarlin::ProcessorConfig::*setVF)(const std::string&, const pymarlin::FloatVec&) = &pymarlin::ProcessorConfig::set;
  pymarlin::ProcessorConfig& (pymarlin::ProcessorConfig::*setVS)(const std::string&, const pymarlin::StringVec&) = &pymarlin::ProcessorConfig::set;
  
  class_<pymarlin::IntVec>("IntVec", init<const list&>() ) ;
  class_<pymarlin::FloatVec>("FloatVec", init<const list&>() ) ;
  class_<pymarlin::StringVec>("StringVec", init<const list&>() ) ;
  
  class_<pymarlin::PluginManager>("PluginManager")
      .def("loadLibraries", &pymarlin::PluginManager::loadLibraries, PluginManager_loadLibraries_overloads(args("env")))
      .def("dump", &pymarlin::PluginManager::dump) 
  ;
  
  class_<pymarlin::ProcessorConfig>("ProcessorConfig")
    .def(init<const std::string&, const std::string&>())
    .add_property("type", 
      make_function( &pymarlin::ProcessorConfig::getType, return_internal_reference<>() ),
      make_function( &pymarlin::ProcessorConfig::setType, return_internal_reference<>() )
    )
    .add_property("name", 
      make_function( &pymarlin::ProcessorConfig::getName, return_internal_reference<>() ),
      make_function( &pymarlin::ProcessorConfig::setName, return_internal_reference<>() )
    )
    .def("set", setI, return_value_policy<reference_existing_object>())
    .def("set", setF, return_value_policy<reference_existing_object>())
    .def("set", setD, return_value_policy<reference_existing_object>())
    .def("set", setB, return_value_policy<reference_existing_object>())
    .def("set", setS, return_value_policy<reference_existing_object>())
    .def("set", setVI, return_value_policy<reference_existing_object>())
    .def("set", setVF, return_value_policy<reference_existing_object>())
    .def("set", setVS, return_value_policy<reference_existing_object>())
    .def(self_ns::str(self_ns::self))
    ;
};

