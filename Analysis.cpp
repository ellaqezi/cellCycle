/*
 320142
 Analysis.cpp
 Izabella Balce
 i.balce@jacobs-university.de
 */

#include "Analysis.h"

Analysis::Analysis(Network* network) {
	_net = network;
	_nets = new vector<Network*>;
	_nets->push_back(_net);
}
Analysis::Analysis(const char *file) {
	_net = new Network;
	_nets = new vector<Network*>;
	std::ifstream f;
	std::string s;
	f.open(file);
	vector<string> vec;
	typedef tokenizer<escaped_list_separator<char> > Tokenizer;
	escaped_list_separator<char> tab('\\', '\t', '\"');
	string first = "";
	getline(f, s);
	while (!f.eof()) {
		Tokenizer tok(s, tab);
		vec.assign(tok.begin(), tok.end());
		Network *n = new Network;
		if (first == "") {
			first = vec[3];
		}
		string current = "";
		while (first != current) {
			Protein *p = new Protein(vec[3]);
			p->abLevel(::atof(vec[1].c_str()));
			p->tsLevel(::atof(vec[2].c_str()));
			p->activatedBy(vec[4]);
			p->deactivatedBy(vec[5]);
			n->_proteins->push_back(new Protein((*p)));

			getline(f, s);
			Tokenizer tok(s, tab);
			vec.assign(tok.begin(), tok.end());
			current = vec[3];
//
			if (vec.size() < 3)
				continue;
		}
		_net = n;
		printZ();
		n->print();
		_nets->push_back(n);
	}
	f.close();
}

Analysis::~Analysis() {
	// TODO Auto-generated destructor stub
}
void Analysis::graphBasins(const char *fileName,
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
						<< /* ", len="<< log(edgeCount.at(itm->first)) <<*/"];\n";
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
void Analysis::findAttractors(map<string, map<string, string> *>& basins,
		map<string, int>& edgeCount) {
	for (int i = 0; i < _nets->size(); i++) {
		_net = _nets->at(i);
		int networkSize = _net->numProteins();
		for (int i = 0; i < pow(2, networkSize); i++) { //determine fixed point basin size
			map<string, string>* edges;
			edges = new map<string, string>;
			_net->fixedPoint(*edges, edgeCount, _net->binStr(i));
			if (basins.count(_net->state()) <= 0) {
				basins.insert(
						pair<string, map<string, string>*>(_net->state(),
								edges));
			} else {
				basins.at(_net->state())->insert(edges->begin(), edges->end());
			}
		}
	}
}
void Analysis::printAttractors(map<string, map<string, string> *> basins,
		std::ostream &os) {
	for (map<string, map<string, string>*>::iterator it = basins.begin();
			it != basins.end(); it++) {
		os << "\t\t\t\"" << it->first << "\"\t" << it->second->size() + 1
				<< "\n";
	}
}
void Analysis::setStateTS() {
	float meanExpLevel = 0;
	for (vector<Protein*>::iterator it = _net->_proteins->begin();
			it != _net->_proteins->end(); it++) {
		meanExpLevel += (**it).tsLevel();
	}
	meanExpLevel /= _net->_proteins->size();
	for (vector<Protein*>::iterator it = _net->_proteins->begin();
			it != _net->_proteins->end(); it++) {
		if ((**it).tsLevel() > meanExpLevel) {
			(**it).state(1);
		} else {
			(**it).state(-1);
		}
	}
}
void Analysis::setStateAB() {
	float meanExpLevel = 0;
	for (vector<Protein*>::iterator it = _net->_proteins->begin();
			it != _net->_proteins->end(); it++) {
		meanExpLevel += (**it).abLevel();
	}
	meanExpLevel /= _net->_proteins->size();
	for (vector<Protein*>::iterator it = _net->_proteins->begin();
			it != _net->_proteins->end(); it++) {
		if ((**it).abLevel() > meanExpLevel) {
			(**it).state(1);
		} else {
			(**it).state(-1);
		}
	}
}
void Analysis::attXbasin(map<string, map<string, string> *> basins) {
	for (map<string, map<string, string>*>::iterator bit = basins.begin();
			bit != basins.end(); bit++) {
		_net->reset();
		_net->setStates((bit)->first);
		for (vector<Protein*>::iterator it = _net->_proteins->begin();
				it != _net->_proteins->end(); it++) {
			(*it)->abLevel((*it)->state() * ((bit)->second->size() + 1));
		}
	}
}
ostream& Analysis::basins(const char *fileName, ostream& os) {
	map<string, map<string, string> *> basins;
	map<string, int> edgeCount;

	//determine fixed point basin size
	findAttractors(basins, edgeCount);

//    printAttractors(basins, os);

	//graph basins
//	graphBasins(fileName, basins, edgeCount);

	attXbasin(basins);

	setStateAB();
	//graph according to attractor*basin size expression levels
	string abGraph("attractorBasin");
	abGraph.append(_net->state());
	_net->graph(_net->createGV(abGraph));

	setStateTS();	//graph according to time series expression levels
	string tsGraph("timeSeries");
	tsGraph.append(_net->state());
	_net->graph(_net->createGV(tsGraph));

	printZ();
	return os;
}
ostream& Analysis::basins(ostream& os) {
	int networkSize = _net->numProteins();
	map<string, map<string, string> *> basins;
	map<string, int> edgeCount;

	//determine fixed point basin size
	findAttractors(basins, edgeCount);

	printAttractors(basins, os);

	attXbasin(basins);
	//set states according to attractor*basin size
	setStateAB();

	//set states according to time series expression levels
	setStateTS();

	return os;
}
void Analysis::basins() {
	this->basins(_net->createGV("basins"), cout);
}
void Analysis::printZ() {
	//set states according to attractor*basin size
	setStateAB();
	cout << "z = ((connected - R0) / active) / sdev\n";
	Network *ab = countActive();
	vector<int> v = this->generateR0(ab);
	float r0 = accumulate(v.begin(), v.end(), 0.0) / v.size();
	float sq_sum = inner_product(v.begin(), v.end(), v.begin(), 0.0);
	float stdev = sqrt(sq_sum / v.size() - r0 * r0);
	cout << "ab: ((" << countConnected(*ab) << " - " << r0 << ") / "
			<< ab->numProteins() << ") / " << stdev << " = "
			<< this->generateZ(ab, v) << endl;

	//set states according to time series expression levels
	setStateTS();
	Network *ts = countActive();
	v = this->generateR0(ts);
	r0 = accumulate(v.begin(), v.end(), 0.0) / 100;
	sq_sum = inner_product(v.begin(), v.end(), v.begin(), 0.0);
	stdev = sqrt(sq_sum / v.size() - r0 * r0);
	cout << "ts:\t\t\t z = " << this->generateZ(ts, v) << endl;
	delete ab;
	delete ts;
}
Network* Analysis::countActive() {
	vector<Protein*> active;
	for (vector<Protein*>::iterator it = _net->_proteins->begin();
			it != _net->_proteins->end(); it++) {
		if ((**it).state() == 1) {
			active.push_back(*it);
		}
	}
	Network *n = new Network(active);
	return n;
}
int Analysis::countConnected(Network& network) {
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

vector<int> Analysis::generateR0(Network *active) {
	int a = active->numProteins();
	int size = _net->numProteins();
	vector<int> r0;
	for (int i = 0; i < 100; i++) {
		_net->reset();
		for (int j = 0; j < a; j++) {
			int random = rand() % size;
			while (_net->_proteins->at(random)->state() == 1) {
				random = rand() % size;
			}
			_net->_proteins->at(random)->state(1);
		}
		Network *ractive = countActive();
		r0.push_back(countConnected(*ractive));
	}
	return r0;
}
float Analysis::generateZ(Network *active, vector<int> v) {
	float r0 = accumulate(v.begin(), v.end(), 0.0) / v.size();
	float sq_sum = inner_product(v.begin(), v.end(), v.begin(), 0.0);
	float stdev = sqrt(sq_sum / v.size() - r0 * r0);
	float z = ((countConnected(*active) - r0) / active->numProteins()) / stdev;
	return z;
}
