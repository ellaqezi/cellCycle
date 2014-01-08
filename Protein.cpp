/*
 Protein.cpp
 Izabella Balce
 i.balce@jacobs-university.de
 */

#include "Protein.h"

using namespace std;

Protein::Protein() {
	_name = "";
	_activatedBy = new set<string>;
	_deactivatedBy = new set<string>;
	_negRegulated = false;
}
Protein::Protein(string name) :
		_name(name) {
            _activatedBy = new set<string>;
            _deactivatedBy = new set<string>;
            _negRegulated = false;
}
Protein::Protein(string name, int state, set<string> *activatedBy,
		set<string> *deactivatedBy) :
		_name(name), _state(state), _activatedBy(activatedBy), _deactivatedBy(
				deactivatedBy) {
	this->negRegulated();
}
Protein::Protein(const Protein &protein) :
		_name(protein._name), _state(protein._state), _negRegulated(protein._negRegulated) {
            _activatedBy = new set<string>(*protein._activatedBy);
            _deactivatedBy = new set<string>(*protein._deactivatedBy);
        }
Protein::Protein(Protein* protein) :
_name(protein->_name), _state(protein->_state),  _negRegulated(                                                                                                                          protein->_negRegulated) {
    _activatedBy = new set<string>(protein->_activatedBy->begin(), protein->_activatedBy->end());
    _deactivatedBy = new set<string>(protein->_deactivatedBy->begin(), protein->_deactivatedBy->end());
}
Protein::~Protein() {
}

//service methods
void Protein::selfDegrade(bool negRegulated) {
	if (!negRegulated and _prev == 1) {
		_state = 0;
	}
	return;
}

//setters
Protein& Protein::name(string name) {
	_name = name;
	return *this;
}
Protein& Protein::state(int state) {
	_prev = _state;
	if (state > 0) {
		_state = 1;
	} else if (state < 0) {
		_state = 0;
	} else {
		this->selfDegrade(_negRegulated);
	}
	return *this;
}
Protein& Protein::state(int activatedBy, int deactivatedBy) {
	_prev = _state;
	int sumOfStates;
	if (activatedBy > deactivatedBy) {
		sumOfStates = 1;
	} else if (activatedBy < deactivatedBy) {
		sumOfStates = -1;
	} else {
		sumOfStates = 0;
	}
//	= activatedBy - deactivatedBy;
	this->state(sumOfStates);
	return *this;
}
Protein& Protein::activatedBy(Protein &regulator) {
	_activatedBy->insert(_activatedBy->end(), regulator.name());
	return *this;
}
Protein& Protein::deactivatedBy(Protein &regulator) {
	_deactivatedBy->insert(_deactivatedBy->end(), regulator.name());
	this->negRegulated();
	return *this;
}
Protein& Protein::negRegulated(bool onOff) {
	_negRegulated = onOff;
	if (onOff == false) {
//        cout << "apply self-degrade loop on " << this->name() << endl;
	}
	return *this;
}
Protein& Protein::reset() {
	this->state(0);
    _activatedBy = new set<string>;
    _deactivatedBy = new set<string>;
	return *this;
}
void Protein::negRegulated() {
	if (!_deactivatedBy->empty()) {
		_negRegulated = true;
	}
}

//getters
string Protein::name() {
	return _name;
}
int Protein::state() {
	return _state;
}
int Protein::prev() {
	return _prev;
}
set<string> Protein::activatedBy() {
	return *_activatedBy;
}
set<string> Protein::deactivatedBy() {
	return *_deactivatedBy;
}
Protein& Protein::operator =(const Protein& protein) {
	this->_state = protein._state;
	this->_negRegulated = protein._negRegulated;
	this->_name = protein._name;
	this->_deactivatedBy = protein._deactivatedBy;
	this->_activatedBy = protein._activatedBy;
	return *this;
}
Protein* Protein::operator =(Protein &protein) {
    *this = protein;
	return this;
}
bool Protein::operator <(const Protein& protein) const {
	if (_name == protein._name) {
		return true;
	} else {
		return false;
	}
}
ostream& operator<<(std::ostream& os, set<string> *proteins) {
	if (!proteins->empty()) {
		for (set<string>::iterator it = proteins->begin();
				it != proteins->end(); it++) {
			Protein p(*it);
			if (it == proteins->begin()) {
				os << p.name();
			} else {
				os << ", " << p.name();
			}
		}
	}
	return os;
}
ostream& operator<<(std::ostream& os, const Protein& protein) {
	os << protein._prev << protein._state << "\t"/*<< "\n"*/<< protein._name
			<< " \t+ " << protein._activatedBy << "\t- "
			<< protein._deactivatedBy << endl;
	return os;
}
ofstream& operator<<(std::ofstream& ofs, const Protein& protein) {
	if (protein._state == 1) {
		ofs << "\"" << protein._name << "\" [shape=box, color=green, style=filled];\n";
	} else {
		ofs << "\"" << protein._name << "\" [shape=box, color=gray, style=filled];\n";
	}
	if (!protein._negRegulated) {
		ofs << "\tedge [color=yellow]; \n";
		ofs << "\t\"" << protein._name << "\" -> \"" << protein._name << "\";"
				<< endl;
	}
	if (!protein._activatedBy->empty()) {
		ofs << "\tedge [color=green]; \n";
		for (set<string>::iterator it = protein._activatedBy->begin();
				it != protein._activatedBy->end(); it++) {
			ofs << "\t\"" << (*it) << "\" -> \"" << protein._name << "\";"
					<< endl;
		}
	}
	if (!protein._deactivatedBy->empty()) {
		ofs << "\tedge [color=red]; \n";
		for (set<string>::iterator it = protein._deactivatedBy->begin();
				it != protein._deactivatedBy->end(); it++) {
			ofs << "\t\"" << (*it) << "\" -> \"" << protein._name << "\";"
					<< endl;
		}
	}

	return ofs;
}
