/*
 RandomNetwork.cpp
 Izabella Balce
 i.balce@jacobs-university.de
 */

#include "RandomNetwork.h"
using namespace std;

RandomNetwork::RandomNetwork() {
    new (this) Network;
}
RandomNetwork::RandomNetwork(Network const &network) {
	new (this) Network(network);
    for (vector<Protein*>::iterator it = this->_proteins->begin(); it!= this->_proteins->end(); it++) {
		(*it)->reset();
	}
}
RandomNetwork::~RandomNetwork() {
}
RandomNetwork& RandomNetwork::randomise() {
    srand (time(NULL));
    int p = 0;
    for (int i = 0; i < this->numProteins()/4; i++) { //TODO: regulate connectivity
        for (vector<Protein*>::iterator it = this->_proteins->begin(); it!= this->_proteins->end(); it++) {
            p = rand() % _proteins->size();
            while (_proteins->at(p)->name() == (*it)->name()) {
                p = rand() % _proteins->size();
            }
            if (!_proteins->at(p)->activatedBy().count((*it)->name()) && !_proteins->at(p)->deactivatedBy().count((*it)->name())) {
                if (rand() % 2 == 0) {
                    _proteins->at(p)->activatedBy(**it);
                } else {
                    _proteins->at(p)->deactivatedBy(**it);
                }
            }
        }
    }
	return *this;
}

