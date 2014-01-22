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
    _edges = 0;
    for (vector<Protein*>::iterator it = this->_proteins->begin(); it!= this->_proteins->end(); it++) {
        _edges += (*it)->activatedBy().size() + (*it)->deactivatedBy().size();
		(*it)->reset();
	}
    _edges -= _proteins->size();
}
RandomNetwork::~RandomNetwork() {
}
RandomNetwork& RandomNetwork::randomise() {
    srand (time(NULL));
    int p = 0, op = 0;
    for (vector<Protein*>::iterator it = this->_proteins->begin(); it!= this->_proteins->end(); it++) { // connect each node to another
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
    for (int i = 0; i < this->_edges + 1; i++) { // create random edges between nodes
        p = rand() % _proteins->size();
        op = rand() % _proteins->size();
        while (p == op) {
            p = rand() % _proteins->size();
        }
        if (!_proteins->at(p)->activatedBy().count(_proteins->at(op)->name()) && !_proteins->at(p)->deactivatedBy().count(_proteins->at(op)->name())) {
            if (rand() % 2 == 0) {
                _proteins->at(p)->activatedBy(*_proteins->at(op));
            } else {
                _proteins->at(p)->deactivatedBy(*_proteins->at(op));
            }
        }
    }
	return *this;
}

