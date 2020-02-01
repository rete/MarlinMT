#!/usr/bin/python

from pymarlin import *

# load libraries
pluginMgr = PluginManager()
pluginMgr.loadLibraries()
pluginMgr.dump()

# Example of processor configuration
conf = ProcessorConfig( "PandoraPFANewProcessor", "MyPandoraPFA" )
conf.set( "SteeringFile", "PandoraSettings.xml" )
conf.set( "AbortOnFail", False )
conf.set( "IntValue", 42 )
conf.set( "Integers", IntVec([42, 78, 89]) )
conf.set( "Strings", StringVec(["toto", "tata", "tutu"]) )
conf.set( "Numbers", FloatVec([423.01, 10e4, 78]) )

print (conf)

