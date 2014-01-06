/*
 RandomNetwork.h
 Izabella Balce
 i.balce@jacobs-university.de
 */

#include <iostream>
#include <string>

#ifndef RANDOMNETWORK_H_
#define RANDOMNETWORK_H_

#include "Network.h"


class RandomNetwork: public Network {
public:
	RandomNetwork();
	RandomNetwork(Network &network);
	virtual ~RandomNetwork();

	RandomNetwork& randomise();
};

#endif /* RANDOMNETWORK_H_ */
