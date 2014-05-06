/*
 Network.cpp
 Izabella Balce$
 i.balce@jacobs-university.de
 */

#include "Network.h"

using namespace std;

Network::Network() {
	_proteins = new vector<Protein*>;
}
Network::Network(int size) {
	_proteins = new vector<Protein*>;
	_proteins->resize(size);
}
Network::Network(Network const &other) {
	_proteins = new vector<Protein*>;
	_proteins->reserve(other._proteins->size());
	for (int i = 0; i < other._proteins->size(); i++) {
		_proteins->push_back(new Protein((other._proteins->at(i))));
	}
}
Network::Network(vector<Protein*> &other) {
	_proteins = new vector<Protein*>;
	_proteins->reserve(other.size());
	for (int i = 0; i < other.size(); i++) {
		_proteins->push_back(new Protein((other.at(i))));
	}
}
Network::~Network() {
	delete _proteins;
}

Network& Network::addProtein(Protein protein) {
	for (vector<Protein*>::iterator it = _proteins->begin();
			it != _proteins->end(); it++) {
		if ((**it).name() == protein.name()) {
//			cout << protein.name() << " is already in network\n";
			return *this;
		}
	}
	_proteins->push_back(&protein);
	return *this;
}
Network& Network::setStates(string binState) {
	int curState;
	string::iterator sit = binState.begin();
	for (vector<Protein*>::iterator it = _proteins->begin();
			it != _proteins->end(); it++) {
		stringstream ss;
		if (*sit == '-') { //negative states
//			sit++;
//			ss << *sit;
			ss << -1;
		} else {
			ss << *sit;
		}
		ss >> curState;
		sit++;
		(**it).state(curState);
	}
	return *this;
}
Network& Network::reset() {
	string offState;
	stringstream ss;
	for (int i = 0; i < _proteins->size(); i++) {
		ss << "-";
	}
	ss >> offState;
	this->setStates(offState);
	return *this;
}
Network& Network::updateStates() {
	string state = this->state();
	string newState;
	stringstream ss;
	for (vector<Protein*>::iterator it = _proteins->begin();
			it != _proteins->end(); it++) {
//        cout << endl << (**it).name() <<" "<< sum((**it).activatedBy()) <<" -"<< sum((**it).deactivatedBy())  ;
//		ss << (sum((**it).activatedBy()) - sum((**it).deactivatedBy()));
		if (sum((**it).activatedBy()) > sum((**it).deactivatedBy())) {
			ss << 1;
		} else if (sum((**it).activatedBy()) < sum((**it).deactivatedBy())) {
			ss << "-";
		} else {
			ss << 0;
		}
//		(**it).expLevel(sum((**it).activatedBy(), (**it).deactivatedBy()));
	}
	ss >> newState;
	this->setStates(newState);
	return *this;
}
Network& Network::fixedPoint(string strState, bool drawGraph) {
	int fpCount = 0;
	cout << "\n" << fpCount << "\t" << *this << " setting state to: "
			<< strState << "\n"; //stepwise

	this->reset();
	this->setStates(strState);

	string prevNetState = "";
	while (prevNetState != this->state()) {
		prevNetState = this->state();
		fpCount++;
		cout << fpCount << "\t" << *this;
		if (drawGraph) {
			this->graph(this->createGV(fpCount));
		}
		this->updateStates(); //stepwise
	}
	cout << "\t<<Stationary @ " << fpCount << " " << this->state() << "\n";
	return *this;
}

Network& Network::fixedPoint(map<string, string> &edges,
		map<string, int> &edgeCount, string strState) {
	int fpCount = 0;
//	cout << "\n" << fpCount << "\t" << *this << " setting state to: "
//			<< strState << "\n"; //stepwise
	map<string, int> prevGeneExpLevels;
	for (vector<Protein*>::iterator it = _proteins->begin();
			it != _proteins->end(); it++) {
		prevGeneExpLevels.insert(
				pair<string, int>((**it).name(), (**it).tsLevel()));
	}

	this->reset();
	this->setStates(strState);
	string prevNetState = "";
	set<string> setOfStates;
	while (setOfStates.count(this->state()) <= 0) {
		setOfStates.insert(this->state());
		prevNetState = this->state();
		for (vector<Protein*>::iterator pit = _proteins->begin();
				pit != _proteins->end(); pit++) {
			(**pit).tsLevel((**pit).state());
		}
		fpCount++;
		if (edgeCount.count(this->state()) > 0) {
			edgeCount.at(this->state()) += 1;
		} else {
			edgeCount.insert(pair<string, int>(this->state(), 1));
		}
//		cout << fpCount << "\t" << *this;
		this->updateStates(); //stepwise
		if (prevNetState != this->state()) {
			edges.insert(pair<string, string>(prevNetState, this->state()));
		}
	}
	for (vector<Protein*>::iterator it = _proteins->begin();
			it != _proteins->end(); it++) {
		(**it).setTSLevel(
				((**it).tsLevel() - prevGeneExpLevels.at((**it).name()))
						/ fpCount + prevGeneExpLevels.at((**it).name()));
	}

//	cout << "\t<<Stationary @ " << fpCount << " " << this->state() << "\n";
	return *this;
}
//Network& Network::fixedPointShort(string strState) {
//	ofstream os;
//	os.open("fps.gv");
//	os << "digraph " << strState << " {\n";
//	this->fixedPointShort(os, strState);
//	os << "\n}";
//	os.close();
//	return *this;
//}
//Network& Network::fixedPointShort(ofstream& ofs, string strState) {
//	int fpsCount = 0;
////	cout << strState;
//	this->reset();
//	this->setStates(strState);
//
//	string prevNetState = "";
//	while (prevNetState != this->state()) {
//		prevNetState = this->state();
//		fpsCount++;
//		this->updateStates();
//		if (prevNetState != this->state()) {
//			ofs << "\"" << prevNetState << "\" -> \"" << this->state()
//					<< "\";\n";
//		}
//	}
////	cout << "\t<<Stationary @ " << fpsCount << "\t" << this->state();
//	return *this;
//}
Network& Network::fixedPointShort(map<string, string> &map, string strState) {
	int fpsCount = 0;
//	cout << strState;
	this->reset();
	this->setStates(strState);

	string prevNetState = "";
	while (prevNetState != this->state()) {
		prevNetState = this->state();
		fpsCount++;
		this->updateStates(); //initial << stationary
		if (prevNetState != this->state()) {
			map.insert(pair<string, string>(prevNetState, this->state()));
		}
	}
//	cout << "\t<<Stationary @ " << fpsCount << "\t" << this->state();
	return *this;
}
Network& Network::operator =(const Network& network) {
	_proteins = network._proteins;
	return *this;
}
string Network::state() const {
	string state;
	stringstream ss;
	for (vector<Protein*>::iterator it = _proteins->begin();
			it != _proteins->end(); it++) {
		ss << ((**it).state());
	}
	ss >> state;
	return state;
}
int Network::numProteins() {
	return _proteins->size();
}
int Network::sum(set<string> regulators) {
	int sum = 0;
	if (!regulators.empty()) {
		for (set<string>::iterator it = regulators.begin();
				it != regulators.end(); it++) {
			//        cout << " " << (this->find((*it)).name()) << " "<<(this->find((*it)).state());
			sum += (this->find((*it))).state();
		}
	}
	return sum;
}
int Network::sum(set<string> posRegulators, set<string> negRegulators) {
	int sumOfStates = sum(posRegulators) - sum(negRegulators);
	return sumOfStates;
}
Protein& Network::find(string protein) {
	Protein *p = NULL;
	for (vector<Protein*>::iterator it = this->_proteins->begin();
			it != this->_proteins->end(); it++) {
		if ((**it).name() == protein) {
			p = *it;
			return *p;
		}
	}
	return *p;
}
void Network::print() {
	for (vector<Protein*>::iterator it = this->_proteins->begin();
			it != this->_proteins->end(); it++) {
		cout << (**it);
	}
}
ostream& Network::print(ostream& os) {
	for (vector<Protein*>::iterator it = this->_proteins->begin();
			it != this->_proteins->end(); it++) {
		os << (**it);
	}
	return os;
}
void Network::graph(const char *fileName, Network& network) {
	ofstream gdot;
	gdot.open(fileName);
	gdot << network;
	gdot.close();
}
void Network::graph(const char *fileName) {
	graph(fileName, *this);
}
void Network::graph() {
	this->graph("graph.gv");
}
void Network::graphBasins(const char *fileName,
		map<string, map<string, string>*> basins, map<string, int> edgeCount) {
	ofstream ofs;
	ofs.open(fileName);
	ofs << "digraph " << "G" << " {" << endl
			<< "layers=\"basins:edges:singles\";\n";

	//attractors with labels
	for (map<string, map<string, string>*>::iterator it = basins.begin();
			it != basins.end(); it++) {
//		ofs << "\t\t\t\"" << it->first << "\"\t" << it->second->size() + 1 << endl;
		ofs
				<< "subgraph {\nnode[shape=box, style=unfilled, layer=\"basins\"];\n";
		ofs << "\"" << it->first << "\" ;\n";
		//edges that occur > 3 times
		ofs
				<< "node[shape=point, layer=\"edges\", color=dodgerblue2, width=0.15, style=filled];\nedge[color=blue];\n";
		ostringstream o;
		for (map<string, string>::iterator itm = it->second->begin();
				itm != it->second->end(); itm++) {
			if (edgeCount.at(itm->first) > 3) {
				ofs << "\"" << itm->second << "\" [width="
						<< log(edgeCount.at(itm->second)) / 15 << "];\n";
				ofs << "\"" << itm->first << "\" -> \"" << itm->second << "\" "
						<< "[penwidth=" << log(edgeCount.at(itm->first)) * 2
						<</* ", len="<< log(edgeCount.at(itm->first)) <<*/"];\n";
			} else {
				o << "\"" << itm->first << "\" -> \"" << itm->second << "\";\n";
			}
		}
		//edges that occur <= 3 times
		ofs
				<< "node[layer=\"singles\", color=gray25, width=0.05];\nedge[color=gray50, len=1];\n";
		ofs << o.str() << "}\n";
	}
	ofs << "}";
	ofs.close();

}
void Network::findAttractors(map<string, map<string, string> *>& basins,
		map<string, int>& edgeCount) {
	int networkSize = this->numProteins();
	for (int i = 0; i < pow(2, networkSize); i++) { //determine fixed point basin size
		map<string, string>* edges;
		edges = new map<string, string>;
		this->fixedPoint(*edges, edgeCount, this->binStr(i));
		if (basins.count(this->state()) <= 0) {
			basins.insert(
					pair<string, map<string, string>*>(this->state(), edges));
		} else {
			basins.at(this->state())->insert(edges->begin(), edges->end());
		}
	}
}
void Network::printAttractors(map<string, map<string, string> *> basins,
		std::ostream &os) {
	for (map<string, map<string, string>*>::iterator it = basins.begin();
			it != basins.end(); it++) {
		os << "\t\t\t\"" << it->first << "\"\t" << it->second->size() + 1
				<< endl;
	}
}
void Network::setStateTS() {
	float meanExpLevel = 0;
	for (vector<Protein*>::iterator it = this->_proteins->begin();
			it != this->_proteins->end(); it++) {
		meanExpLevel += (**it).tsLevel();
	}
	meanExpLevel /= this->_proteins->size();
	for (vector<Protein*>::iterator it = this->_proteins->begin();
			it != this->_proteins->end(); it++) {
		if ((**it).tsLevel() > meanExpLevel) {
			(**it).state(1);
		} else {
			(**it).state(-1);
		}
	}
}
void Network::setStateAB(map<string, map<string, string> *> basins) {
	for (map<string, map<string, string>*>::iterator bit = basins.begin();
			bit != basins.end(); bit++) {
		this->reset();
		this->setStates((bit)->first);
		for (vector<Protein*>::iterator it = this->_proteins->begin();
				it != this->_proteins->end(); it++) {
			(*it)->abLevel((*it)->state() * ((bit)->second->size() + 1));
		}
	}
	float meanExpLevel = 0;
	for (vector<Protein*>::iterator it = this->_proteins->begin();
			it != this->_proteins->end(); it++) {
		meanExpLevel += (**it).abLevel();
	}
	meanExpLevel /= this->_proteins->size();
	for (vector<Protein*>::iterator it = this->_proteins->begin();
			it != this->_proteins->end(); it++) {
		if ((**it).abLevel() > meanExpLevel) {
			(**it).state(1);
		} else {
			(**it).state(-1);
		}
	}
}
ostream& Network::basins(const char *fileName, ostream& os) {
	map<string, map<string, string> *> basins;
	map<string, int> edgeCount;

	//determine fixed point basin size
	findAttractors(basins, edgeCount);

//    printAttractors(basins, os);

	//graph basins
	graphBasins(fileName, basins, edgeCount);

	//set states according to attractor*basin size
	setStateAB(basins);
	Network *ab = countActive();
	cout << "ab: " << countConnected(*ab) << " / "
			<< ab->numProteins() << endl;

	//graph according to attractor*basin size expression levels
	string abGraph("attractorBasin");
	abGraph.append(this->state());
	this->graph(this->createGV(abGraph));

	//set states according to time series expression levels
	setStateTS();
	Network *ts = countActive();
	cout << "ts: " << countConnected(*ts) << " / "
			<< ts->numProteins() << endl;

	//graph according to time series expression levels
	string tsGraph("timeSeries");
	tsGraph.append(this->state());
	this->graph(this->createGV(tsGraph));

	delete ab;
	delete ts;
	return os;
}
ostream& Network::basins(ostream& os) {
	int networkSize = this->numProteins();
	map<string, map<string, string> *> basins;
	map<string, int> edgeCount;

	//determine fixed point basin size
	findAttractors(basins, edgeCount);

	printAttractors(basins, os);

	//set states according to attractor*basin size
	setStateAB(basins);

	//set states according to time series expression levels
	setStateTS();

	return os;
}
void Network::basins() {
	this->basins(this->createGV("basins"), cout);
}
string Network::binStr(unsigned n, int length) {
	if (n >= pow(2, length)) {
		return "exceeds length";
	}
	const size_t size = sizeof(n) * 8;
	char result[size];
	unsigned index = size;
	do {
		result[--index] = '0' + (n & 1);
	} while (n >>= 1);
	string res = string(result + index, result + size);

	string lead;
	lead.reserve(length);
	for (unsigned int i = 0; i < length - res.length(); i++) {
		lead += '0';
	}
	return lead + res;
}

Network* Network::countActive() {
	vector<Protein*> active;
	for (vector<Protein*>::iterator it = this->_proteins->begin();
			it != this->_proteins->end(); it++) {
		if ((**it).state() == 1) {
			active.push_back(*it);
		}
	}
	Network *n = new Network(active);
	return n;
}
int Network::countConnected(Network& network) {
	int count = 0;
	bool connected = false;
	set<string> links;
	set<string> active;
	for (vector<Protein*>::iterator it = network._proteins->begin();
			it != network._proteins->end(); it++) {
		active.insert((*it)->name());
		set<string> act((**it).activatedBy());
		set<string> deact((**it).deactivatedBy());
		links.insert(act.begin(), act.end());
		links.insert(deact.begin(), deact.end());
	}
	for (set<string>::iterator si = links.begin(); si != links.end(); si++) {
		if (active.count(*si) != 0) {
			count += 1;
		}
	}
	return count;
}
string Network::binStr(unsigned n) {
	return binStr(n, this->numProteins());
}
const char* Network::createGV(int count) {
	stringstream ss;
	string s;
	ss << count << ".gv";
	ss >> s;
	return s.c_str();
}
const char* Network::createGV(string s) {
	string name;
	stringstream ss;
	ss << s << ".gv";
	ss >> name;
//	cout << name;
	return name.c_str();
}
const char* Network::createGV(string st, int count) {
	stringstream ss;
	string s;
	ss << st << count << ".gv";
	ss >> s;
	return s.c_str();
}
ostream& operator<<(ostream& os, const Network& network) {
	os << network.state() << endl;
	return os;
}
ofstream& operator<<(ofstream& ofs, const Network& network) {
	ofs << "digraph \"" << network.state() << "\"  {\n";
	for (vector<Protein*>::iterator it = network._proteins->begin();
			it != network._proteins->end(); it++) {
		ofs << (**it);
	}
	ofs << endl << "}";
	return ofs;
}
