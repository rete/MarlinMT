#################################
# cmake file for building DD4hep component
# @author Remi Ete, DESY
#################################

# From v01-11, DD4hep uses imported targets
FIND_PACKAGE( DD4hep 01.11 COMPONENTS DDRec DDParsers REQUIRED )

# create library
AUX_SOURCE_DIRECTORY( plugins MarlinDD4hep_sources )
ADD_SHARED_LIBRARY( MarlinDD4hep ${MarlinDD4hep_sources} )
TARGET_LINK_LIBRARIES( MarlinDD4hep Marlin DD4hep::DDCore DD4hep::DDRec DD4hep::DDParsers )
INSTALL_SHARED_LIBRARY( MarlinDD4hep DESTINATION lib )
