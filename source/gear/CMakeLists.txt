#################################
# cmake file for building Gear component
# @author Remi Ete, DESY
#################################

FIND_PACKAGE( Gear COMPONENTS )

# create library
AUX_SOURCE_DIRECTORY( plugins MarlinGear_sources )
ADD_SHARED_LIBRARY( MarlinGear ${MarlinGear_sources} )
TARGET_LINK_LIBRARIES( MarlinGear Marlin ${Gear_LIBRARIES} )
TARGET_INCLUDE_DIRECTORIES( MarlinGear SYSTEM ${Gear_INCLUDE_DIRS} )
INSTALL_SHARED_LIBRARY( MarlinGear DESTINATION lib )
