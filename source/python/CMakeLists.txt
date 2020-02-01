
set( Python_USE_STATIC_LIBS FALSE )
find_package( Python REQUIRED COMPONENTS Interpreter Development  )
set( PythonBoost python${Python_VERSION_MAJOR}${Python_VERSION_MINOR} )
find_package( Boost COMPONENTS ${PythonBoost} REQUIRED )

# Generate the python module
Python_add_library( pymarlin MODULE src/test.cc )
target_link_libraries( pymarlin PUBLIC Marlin::Core Python::Python Boost::${PythonBoost} )

# Install the module along the python sources
install( TARGETS pymarlin LIBRARY DESTINATION python )
install( DIRECTORY python DESTINATION . )


