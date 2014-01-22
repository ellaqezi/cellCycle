/*
 RandomNetwork.h
 Izabella Balce
 i.balce@jacobs-university.de
 */

#include <iostream>
#include <string>
#include <stdlib.h>
#include <time.h>

#ifndef RANDOMNETWORK_H_
#define RANDOMNETWORK_H_

#include "Network.h"


class RandomNetwork: public Network {
public:
	RandomNetwork();
	RandomNetwork(Network const &network);
	virtual ~RandomNetwork();
    
	RandomNetwork& randomise();
    int _edges;
};

#endif /* RANDOMNETWORK_H_ */
