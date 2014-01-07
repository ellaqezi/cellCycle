/*
 Protein.h
 Izabella Balce
 i.balce@jacobs-university.de
 */

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#ifndef PROTEIN_H_
#define PROTEIN_H_

class Protein {
public:
	Protein();
	Protein(std::string name);
	Protein(std::string name, int state, std::vector<std::string> *activatedBy,
			std::vector<std::string> *deactivatedBy);
	Protein(const Protein &protein); // copy constructor
    Protein(Protein*);
	virtual ~Protein();

	//service methods
	virtual int sum(std::vector<std::string> regulators) {
		return 0;
	}
	;
	virtual int sum(std::vector<std::string> posRegulators,
			std::vector<std::string> negRegulators) {
		return 0;
	}
	;
	void selfDegrade(bool negRegulated);

	//setters
	Protein& name(std::string name);
	Protein& state(int state);
	Protein& state(int activatedBy, int deactivatedBy);
	Protein& activatedBy(Protein &regulator);
	Protein& deactivatedBy(Protein &regulator);
	Protein& negRegulated(bool onOff);
	Protein& reset();
	void negRegulated();

	//getters
	std::string name();
	int state();
	int prev();
	std::vector<std::string> activatedBy();
	std::vector<std::string> deactivatedBy();

	Protein& operator =(const Protein& protein);
    Protein* operator =(Protein& p);

	bool operator <(const Protein& protein) const;
	friend std::ostream& operator<<(std::ostream& os,
			std::vector<std::string> proteins);
	friend std::ostream& operator<<(std::ostream& os, const Protein& protein);
	friend std::ofstream& operator<<(std::ofstream& ofs,
			const Protein& protein);

private:
	std::string _name;
	int _state = 0, _prev = 0;
	std::vector<std::string> *_activatedBy;
	std::vector<std::string> *_deactivatedBy;
	bool _negRegulated = false;
};

#endif /* PROTEIN_H_ */
