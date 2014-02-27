/*
 Network.h
 Izabella Balce
 i.balce@jacobs-university.de
 */
#include <iostream>
#include <map>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include "math.h"

#ifndef NETWORK_H_
#define NETWORK_H_

#include "Protein.h"

class Network: public Protein {
public:
	Network();
	Network(int size);
	Network(Network const &other);
	virtual ~Network();

	Network& addProtein(Protein protein);
	Network& setStates(std::string binState);
	Network& reset();
	Network& updateStates();

	Network& fixedPoint(std::string strState, bool drawGraph=false);
	Network& fixedPoint(std::map<std::string, std::string> &edges, std::map<std::string, int> &edgeCount, std::string strState);

//	Network& fixedPointShort(std::string strState);
//	Network& fixedPointShort(std::ofstream &ofs, std::string strState);
	Network& fixedPointShort(std::map<std::string, std::string> &edges,
			std::string strState);

	Network& operator=(const Network& network);
	std::string state() const; //getter of Network state
	int numProteins();
	int sum(std::set<std::string> regulators);
	int sum(std::set<std::string> posRegulators, std::set<std::string> negRegulators);
    Protein& find(std::string protein);
	void print();
	std::ostream& print(std::ostream& os);
	void graph();
	void graph(const char *fileName);
	void basins();
    std::ostream& basins(const char *fileName, std::ostream& os);

    std::string binStr(unsigned n, int length);
	std::string binStr(unsigned n);
    const char* createGV(int count);
    const char* createGV(std::string s);

	friend std::ostream& operator<<(std::ostream& os, const Network& proteins);
	friend std::ofstream& operator<<(std::ofstream& ofs,
			const Network& proteins);

	std::vector<Protein*> *_proteins;
};

#endif /* NETWORK_H_ */
