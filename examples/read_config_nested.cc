
class ParentAlgo : public Algorithm {
public:
  ParentAlgo() ;
  void run() ;
  
private:
  /// Single simple property
  marlin::Property<float>        m_weightCut { this, "WeightCut", 
        "The event weigth cut", 0.05 } ;
  /// A property configuring and describing a daughter algorithm
  marlin::AlgorithmProperty      m_daughterAlgorithm { this, "Daughter", 
        "The daughter algorithm name"} ;
};

//--------------------------------------------------------------------------------------

/// Constructor with algorithm type 
void ParentAlgo::ParentAlgo() : Algorithm("ParentAlgo") {}


/// Run method
void ParentAlgo::run() {
  // Get the event from the Marlin API
  auto event = MarlinApi::getEvent<EVENT::LCEvent>( this ) ;
  
  // Run the daughter algorithm depending on the event weigth
  if ( event->getWeigth() > m_weightCut ) {
    marlin::MarlinApi::RunDaughterAlgorithm( this, m_daughterAlgorithm.get() ) ;
  }
}

//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------


class DaughterAlgo : public Algorithm {
public:
  DaughterAlgo() ;
  void run() ;
  
private:
  /// Single simple property
  marlin::Property<unsigned int>        m_nElements { this, "NElements", 
        "The number of tracks to generate", 300 } ;
  /// An output property. In this case an LCIO collection
  marlin::OutputProperty                m_outputCollection { this, EVENT::LCIO::TRACK, "OutputCollection", 
        "The output random track list" } ;
};

//--------------------------------------------------------------------------------------

/// Constructor with algorithm type 
void DaughterAlgo::DaughterAlgo() : Algorithm("DaughterAlgo") {}


/// Run method
void DaughterAlgo::run() {
  // Get the event from the Marlin API
  auto event = MarlinApi::getEvent<EVENT::LCEvent>( this ) ;
  // Create the output collection
  auto collection = std::make_unique<IMPL::LCCollectionVec>( EVENT::LCIO::TRACK ) ;
  
  for ( unsigned int i=0 ; i<m_nElements.get() ; i++ ) {
    auto track = generateTrack() ;
    collection->addElement( track ) ;
  }
  event->addCollection( collection.release() , m_outputCollection.get() ) ;
}
