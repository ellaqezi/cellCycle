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
//		cout << curState;
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
	}
	ss >> newState;
//    cout << endl << newState << "%%" << state;
	this->setStates(newState);
	return *this;
}
Network& Network::fixedPoint(std::string strState) {
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
		const char *file;
		stringstream ss;
		string s;
		ss << fpCount << ".gv";
		ss >> s;
		file = s.c_str();
		this->graph(file);
		this->updateStates(); //stepwise
	}
	cout << "\t<<Stationary @ " << fpCount << " " << this->state() << "\n";
	return *this;
}
Network& Network::fixedPointShort(std::string strState) {
	ofstream os;
	os.open("fps.gv");
	os << "digraph " << strState << " {\n";
	int fpsCount = 0;
//	cout << strState; //initial << stationary

	this->reset();
	this->setStates(strState);

	string prevNetState = "";
	while (prevNetState != this->state()) {
		prevNetState = this->state();
		fpsCount++;
		this->updateStates(); //initial << stationary
		if (prevNetState != this->state()) {
			os << "\"" << prevNetState << "\" -> \"" << this->state()
					<< "\";\n";
		}
	}
	os << "\n}";
	os.close();
//	cout << "\t<<Stationary @ " << fpsCount << "\t" << this->state();
	return *this;
}
Network& Network::fixedPointShort(ofstream& ofs, std::string strState) {
	int fpsCount = 0;
//	cout << strState; //initial << stationary

	this->reset();
	this->setStates(strState);

	string prevNetState = "";
	while (prevNetState != this->state()) {
		prevNetState = this->state();
		fpsCount++;
		this->updateStates(); //initial << stationary
		if (prevNetState != this->state()) {
			ofs << "\"" << prevNetState << "\" -> \"" << this->state()
					<< "\";\n";
		}
	}
//	cout << "\t<<Stationary @ " << fpsCount << "\t" << this->state();
	return *this;
}
Network& Network::fixedPointShort(map<string, string> &map,
		std::string strState) {
	int fpsCount = 0;
//	cout << strState; //initial << stationary

	this->reset();
	this->setStates(strState);

	string prevNetState = "";
	while (prevNetState != this->state()) {
		prevNetState = this->state();
		fpsCount++;
		this->updateStates(); //initial << stationary
		if (prevNetState != this->state()) {
			map.insert(pair<string, string>(prevNetState, this->state()));
//			cout << prevNetState << " -> " << this->state() << endl;
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
void Network::print() {
	for (vector<Protein*>::iterator it = this->_proteins->begin();
			it != this->_proteins->end(); it++) {
		cout << (**it);
	}
}

void Network::graph(const char *fileName) {
	ofstream gdot;
	gdot.open(fileName);
	gdot << *this;
	gdot.close();
}
void Network::graph() {
	this->graph("graph.gv");
}
void Network::basins() {
	int networkSize = this->numProteins();
	map<string, int> basins;
	map<string, string> edges;
	ofstream os;
	os.open("basins.gv");
	os << "digraph " << "G" << " {\nnode[shape=point];" << endl
			<< "concentrate=true;\n"/* << "edge[arrowhead=\"none\"];\n"*/;

	for (int i = 0; i < pow(2, networkSize); i++) { //determine fixed point basin size
//		this->fixedPointShort(os, this->binStr(i));
		this->fixedPointShort(edges, this->binStr(i));
		if (basins.count(this->state()) <= 0) {
			basins.insert(pair<string, int>(this->state(), 1));
		} else {
			basins.at(this->state()) += 1;
		}
//		cout << endl;
	}
	for (map<string, string>::iterator it = edges.begin(); it != edges.end();
			it++) {
		os << "\"" << it->first << "\" -> \"" << it->second << "\";\n";
	}
	for (map<string, int>::iterator it = basins.begin(); it != basins.end();
			it++) {
		cout << it->first << " >> " << it->second << endl;
		os << "\"" << it->first << "\" [shape=box];\n";
	}
	os << "\n}";
	os.close();
}
Protein& Network::find(std::string protein) {
	Protein *p;
	for (vector<Protein*>::iterator it = _proteins->begin();
			it != _proteins->end(); it++) {
		if ((**it).name() == protein) {
			p = *it;
		}
	}
	return *p;
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

ostream& operator<<(std::ostream& os, const Network& network) {
	os << network.state() << endl;
	return os;
}

ofstream& operator<<(std::ofstream& ofs, const Network& network) {
	ofs << "digraph " << network.state() << "  {\n";
	for (vector<Protein*>::iterator it = network._proteins->begin();
			it != network._proteins->end(); it++) {
		ofs << (**it);
	}
	ofs << endl << "}";
	return ofs;
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
string Network::binStr(unsigned n) {
	return binStr(n, this->numProteins());
}
