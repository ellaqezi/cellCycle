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
RandomNetwork::RandomNetwork(Network &network) {
	new (this) Network(network);
}

RandomNetwork::~RandomNetwork() {
	delete this;
}
RandomNetwork& RandomNetwork::randomise() {
	for (vector<Protein*>::iterator it = this->_proteins->begin(); it!=this-> _proteins->end(); it++) {
		(*it)->reset();
	}
	return *this;
}

