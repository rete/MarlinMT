#pragma once

// -- std headers
#include <iostream>
#include <string>
#include <sstream>
#include <typeinfo>
#include <memory>
#include <vector>
#include <functional>
#include <typeindex>

// -- marlin headers
#include <marlin/Utils.h>

namespace marlin {
  
  template <typename T>
  class Property ;
  
  
  enum class EPropertyType {
    eSimple,              /// Simple property
    eAlgorithm,           /// Daughter algorithm
    eAlgorithmTool        /// Algorithm tool
  };
      
  class PropertyImpl {
  public:
    PropertyImpl() = delete ;
    ~PropertyImpl() = default ;
    
    template <typename T>
    inline PropertyImpl( EPropertyType propType, const std::string &na, const std::string & desc, std::shared_ptr<T> addr ) :
      _type(propType),
      _name(na),
      _description(desc),
      _value(addr),
      _typeIndex(typeid(T)) {
      _typeFunction = [] { return details::type_info<T>::type ; };
      _resetFunction = [this] { *std::static_pointer_cast<T>( _value ).get() = T() ; };
      _strFunction = [this] { return details::to_string( get<T>() ) ; };
    }
    
    /**
     *  @brief  Get the property name
     */
    inline EPropertyType type() const {
      return _type ;
    }    
        
    /**
     *  @brief  Get the property name
     */
    inline const std::string& name() const {
      return _name ;
    }
    
    /**
     *  @brief  Get the property description
     */
    inline const std::string& description() const {
      return _description ;
    }
    
    /**
     *  @brief  Whether the property has been set
     */
    inline bool isSet() const {
      return _isSet ;
    }
    
    inline std::string str() const {
      return _strFunction() ;
    }
    
    inline std::string typeStr() const {
      return _typeFunction() ;
    }
    
    inline const std::type_index &typeIndex() const {
      return _typeIndex ;
    }
    
    template <typename T>
    inline bool isType() const {
      return ( std::type_index(typeid(T)) == _typeIndex ) ;
    }
    
    template <typename T>
    inline void checkType() const {
      if( not isType<T>() ) {
        MARLIN_THROW( "checkType failure. Given: " + std::string(details::type_info<T>::type) + ", stored: " + typeStr() ) ;
      }
    }
    
    template <typename T>
    inline void set( const T &val ) {
      checkType<T>() ;
      *std::static_pointer_cast<T>( _value ).get() = val ;
      _isSet = true ;
    }
    
    template <typename T>
    inline T get() const {
      checkType<T>() ;
      if( not isSet() ) {
        MARLIN_THROW( "Property '" + name() +  "' not set" ) ;
      }
      return *std::static_pointer_cast<T>( _value ).get() ;
    }
    
    template <typename T>
    inline T get( const T &fallback ) const {
      checkType<T>() ;
      if( not isSet() ) {
        return fallback ;
      }
      return *std::static_pointer_cast<T>( _value ).get() ;
    }
    
    inline void reset() {
      _resetFunction() ;
      _isSet = false ;
    }
    
  private:
    std::string                      _name {} ;
    EPropertyType                    _type {} ;
    std::string                      _description {} ;
    std::function<std::string()>     _typeFunction {} ;
    std::function<std::string()>     _strFunction {} ;
    std::function<void()>            _resetFunction {} ;
    bool                             _isSet {false} ;
    std::shared_ptr<void>            _value {nullptr} ;
    std::type_index                  _typeIndex ;
  };
  
  //--------------------------------------------------------------------------
  //--------------------------------------------------------------------------
  
  class Configurable {
  public:
    Configurable() = default ;
    virtual ~Configurable() = default ;
    
    using PropertyMap = std::map<std::string, std::shared_ptr<PropertyImpl>> ;
    using iterator = PropertyMap::iterator ;
    using const_iterator = PropertyMap::const_iterator ;
    
    template <typename T>
    inline std::weak_ptr<PropertyImpl> addProperty( EPropertyType propType, const std::string &name, const std::string &desc, std::shared_ptr<T> value ) {
      checkProperty( name ) ;
      auto prop = std::make_shared<PropertyImpl>( propType, name, desc, value ) ;
      _properties[ name ] = prop ;
      return prop ;
    }
    
    template <typename T>
    inline T property( const std::string &name ) const {
      checkProperty( name ) ;
      return _properties.find( name )->second->get<T>()  ;
    }
    
    template <typename T>
    inline T property( const std::string &name, const T &fallback ) const {
      checkProperty( name ) ;
      return _properties.find( name )->second->get<T>( fallback )  ;
    }
    
    inline void checkProperty( const std::string &name ) const {
      if( exists( name ) ) {
        MARLIN_THROW( "Property '" + name +  "' already present" ) ;
      }
    }
    
    inline bool exists( const std::string &name ) const {
      return _properties.find( name ) != _properties.end() ;
    }
    
    inline bool isSet( const std::string &name ) const {
      auto iter = _properties.find( name ) ;
      if( iter == _properties.end() ) {
        return false ;
      }
      iter->second->isSet() ;
    }
    
    iterator begin() {
      return _properties.begin() ;
    }
    
    const_iterator begin() const {
      return _properties.begin() ;
    }
    
    iterator end() {
      return _properties.end() ;
    }
    
    const_iterator end() const {
      return _properties.end() ;
    }
    
    void clear() {
      _properties.clear() ;
    }
    
  private:
    PropertyMap               _properties {} ;
  };
  
  //--------------------------------------------------------------------------
  //--------------------------------------------------------------------------
  
  template <typename T>
  class PropertyBase {
  public:
    virtual ~PropertyBase() = default ;
    
    inline PropertyBase( Configurable &conf, EPropertyType propType, const std::string &na, const std::string &desc ) {
      _impl = conf.addProperty( propType, na, desc, _value ) ;
    }
    
    inline PropertyBase( Configurable &conf, EPropertyType propType, const std::string &na, const std::string &desc, const T &defVal ) :
      _defaultValue( defVal ) {
      _impl = conf.addProperty( propType, na, desc, _value ) ;
    }

    /**
     *  @brief  Get the property type
     */
    inline EPropertyType type() const {
      return _impl.lock()->type() ;
    }
    
    /**
     *  @brief  Get the property name
     */
    inline const std::string& name() const {
      return _impl.lock()->name() ;
    }
    
    /**
     *  @brief  Get the property description
     */
    inline const std::string& description() const {
      return _impl.lock()->description() ;
    }
    
    /**
     *  @brief  Whether the property has been set
     */
    inline bool isSet() const {
      return _impl.lock()->isSet() ;
    }
    
    inline std::string str() const {
      return _impl.lock()->str() ;
    }
    
    inline std::string typeStr() const {
      return _impl.lock()->typeStr() ;
    }
    
    inline const std::type_index &typeIndex() const {
      return _impl.lock()->typeIndex() ;
    }
    
    inline operator T() const {
      return *_value.get() ;
    }
    
    inline T get() const {
      if( isSet() ) {
        return *_value.get() ;
      }
      return _defaultValue.value() ;
    }
    
    inline void reset() {
      _impl.lock()->reset() ;
    }
    
  protected:
    std::shared_ptr<T>             _value { std::make_shared<T>() } ;
    std::optional<T>               _defaultValue {} ;
    std::weak_ptr<PropertyImpl>    _impl {} ;
  };
  
  template <typename T>
  class Property : public PropertyBase<T> {
  public:
    inline Property( Configurable &conf, const std::string &na, const std::string &desc ) :
      PropertyBase<T>( EPropertyType::eSimple, na, desc ) {
      /* nop */
    }
    
    inline Property( Configurable &conf, const std::string &na, const std::string &desc, const T &defVal ) :
      PropertyBase<T>( EPropertyType::eSimple, na, desc, defVal ) {
      /* nop */
    }
  };
  
  //--------------------------------------------------------------------------
  
  // helper types
  using IntProperty = Property<int> ;
  using UIntProperty = Property<unsigned int> ;
  using FloatProperty = Property<float> ;
  using DoubleProperty = Property<double> ;
  using BoolProperty = Property<bool> ;
  using StringProperty = Property<std::string> ;
  using IntVectorProperty = Property<std::vector<int>> ;
  using UIntVectorProperty = Property<std::vector<unsigned int>> ;
  using FloatVectorProperty = Property<std::vector<float>> ;
  using DoubleVectorProperty = Property<std::vector<double>> ;
  using BoolVectorProperty = Property<std::vector<bool>> ;
  using StringVectorProperty = Property<std::vector<std::string>> ;
  
  //--------------------------------------------------------------------------
  //--------------------------------------------------------------------------
  
  class Algorithm ;
  class AlgorithmTool ;
  
  class AlgorithmProperty : public PropertyBase<std::string> {
  public:
    AlgorithmProperty( Algorithm *alg, const std::string &key, const std::string &desc ) ;
  };
  
  //--------------------------------------------------------------------------
  //--------------------------------------------------------------------------
  
  class AlgorithmListProperty : public PropertyBase<std::vector<std::string>> {
  public:
    AlgorithmListProperty( Algorithm *alg, const std::string &key, const std::string &desc ) ;
  };
  
  //--------------------------------------------------------------------------
  //--------------------------------------------------------------------------

  class ToolProperty : public PropertyBase<std::string> {
  public:
    ToolProperty( AlgorithmTool *t, const std::string &key, const std::string &desc ) ;
    
    template <typename T>
    const T *tool() const {
      return std::static_pointer_cast<const T>(_tool.lock()).get() ;
    }
    
  private:
    std::weak_ptr<AlgorithmTool>     _tool {} ;
  };

} // end namespace marlin
