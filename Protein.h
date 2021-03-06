/*
 Protein.h
 Izabella Balce
 i.balce@jacobs-university.de
 */

#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <string>

#include <boost/tokenizer.hpp>

#ifndef PROTEIN_H_
#define PROTEIN_H_

//typedef boost::tokenizer<boost::escaped_list_separator<char> > Tokenizer;

class Protein {
public:
	Protein();
	Protein(std::string name);
	Protein(std::string name, int state, std::set<std::string> *activatedBy,
			std::set<std::string> *deactivatedBy);
	Protein(const Protein &protein); // copy constructor
	Protein(Protein*);
//	typedef boost::tokenizer<boost::escaped_list_separator<char> > Tokenizer;
//	Protein(Tokenizer tok);
	virtual ~Protein();

	//service methods
	virtual int sum(std::set<std::string> regulators) {
		return 0;
	}
	;
	virtual int sum(std::set<std::string> posRegulators,
			std::set<std::string> negRegulators) {
		return 0;
	}
	;
	void selfDegrade(bool negRegulated);

	//setters
	Protein& name(std::string name);
	Protein& state(int state);
	Protein& state(int activatedBy, int deactivatedBy);
	Protein& tsLevel(float sumOfStates);
	Protein& setTSLevel(float sumOfStates);
    Protein& abLevel(float sumOfStates);
	Protein& setABLevel(float sumOfStates);

	Protein& activatedBy(Protein &regulator);
	Protein& activatedBy(std::vector<std::string> regulators);
	Protein& activatedBy(std::string regulators);
	Protein& deactivatedBy(Protein &regulator);
	Protein& deactivatedBy(std::vector<std::string> regulators);
	Protein& deactivatedBy(std::string regulators);
	Protein& negRegulated(bool onOff);
	Protein& reset();
	void negRegulated();

	//getters
	std::string name();
	int state();
	int prev();
	float tsLevel();
    float abLevel();
	std::set<std::string> activatedBy();
	std::set<std::string> deactivatedBy();

	Protein& operator =(const Protein& protein);
	Protein* operator =(Protein& p);

	bool operator <(const Protein& protein) const;
	friend std::ostream& operator<<(std::ostream& os,
			std::set<std::string> proteins);
	friend std::ostream& operator<<(std::ostream& os, const Protein& protein);
	friend std::ofstream& operator<<(std::ofstream& ofs,
			const Protein& protein);

private:
	std::string _name;
	int _state, _prev;
	float _tsLevel, _abLevel;
	std::set<std::string> *_activatedBy;
	std::set<std::string> *_deactivatedBy;
	bool _negRegulated = false;
};

#endif /* PROTEIN_H_ */
