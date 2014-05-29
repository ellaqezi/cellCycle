/*
 320142
 Analysis.h
 Izabella Balce
 i.balce@jacobs-university.de
 */

#ifndef ANALYSIS_H_
#define ANALYSIS_H_

#include "RandomNetwork.h"
#include <numeric>
#include <algorithm>

using namespace std;

class Analysis {
public:
	Analysis(Network *network);
	Analysis(const char *fileName);
	void graphBasins(const char *fileName,
			std::map<std::string, std::map<std::string, std::string> *> basins,
			std::map<std::string, int> edgeCount);

	void findAttractors(
			std::map<std::string, std::map<std::string, std::string> *>& basins,
			std::map<std::string, int> &edgeCount);
	void printAttractors(
			std::map<std::string, std::map<std::string, std::string> *> basins,
			std::ostream& os);

	void setStateTS();
	void setStateAB();
	void attXbasin(
			std::map<std::string, std::map<std::string, std::string> *> basins);

	void basins();
	std::ostream& basins(std::ostream& os);
	std::ostream& basins(const char *fileName, std::ostream& os);

	void printZ();

	Network* countActive();
	int countConnected(Network& network);

	std::vector<int> generateR0(Network *active);
	float generateZ(Network *active, std::vector<int> v);


	virtual ~Analysis();
private:
	Network* _net;
	std::vector<Network*> *_nets;
};

#endif /* ANALYSIS_H_ */
