/*
 main.cpp
 Izabella Balce
 i.balce@jacobs-university.de
 */

#include <iostream>
#include <limits>
#include <set>
#include <string>
#include <bitset>
#include <map>
#include "math.h"
#include "Protein.h"
#include "Network.h"
#include "RandomNetwork.h"
#include "Analysis.h"

#include <boost/tokenizer.hpp>
using namespace std;

int main(int argc, const char * argv[]) {
	srand(time(NULL));

	Protein a;
	a.name("Sic1").state(0);
	Protein b("Clb1,2");
	Protein c("Mcm1/SFF");
	Protein d("Cdc20&Cdc14");
	Protein e("Swi5");
	Protein f("Cln3");
	Protein g("SBF");
	Protein h("MBF");
	Protein i("Clb5,6");
	Protein j("Cdh1");
	Protein k("Cln1,2");

	f.deactivatedBy();
	h.activatedBy(f).deactivatedBy(b);
	g.activatedBy(f).deactivatedBy(b);
	k.activatedBy(g);
	j.activatedBy(d).deactivatedBy(k).deactivatedBy(i).deactivatedBy(b);
	e.activatedBy(c).activatedBy(d).deactivatedBy(b);
	d.activatedBy(b).activatedBy(c);
	i.activatedBy(h).deactivatedBy(a).deactivatedBy(d);
	a.activatedBy(d).activatedBy(e).deactivatedBy(b).deactivatedBy(k).deactivatedBy(
			i);
	b.activatedBy(c).activatedBy(i).deactivatedBy(a).deactivatedBy(d).deactivatedBy(
			j);
	c.activatedBy(b).activatedBy(i).deactivatedBy();
	e.negRegulated(false); //self-degrade Swi5

	Network n;
	n.addProtein(f);
	n.addProtein(h).addProtein(g).addProtein(k).addProtein(j).addProtein(e).addProtein(
			d).addProtein(i).addProtein(a).addProtein(b).addProtein(c).addProtein(
			d).addProtein(e).addProtein(b);

//	n.print();
//	cout << "\nTemporal Evolution";
//	n.fixedPoint("10001000100", true); //start phase Cln3, Cdh1 and Sic1 are ON.
//	n.fixedPointShort("10001000100");
//	n.fixedPoint(n.binStr(1092));
//	n.print();
//	n.reset();
//	n.setStates("00001000100");
	n.graph("new.gv");

//	cout << "\nFixed points and basin sizes\n";
	Analysis cellCycle(&n);
	cellCycle.basins();
	n.print();

	cout << endl;
//	ofstream csv("random100.csv");
//	ofstream txt("random100.txt");
//    std::streambuf * bufc = csv.rdbuf();
//    std::streambuf * buft = txt.rdbuf();
//    ostream out(bufc);
//    ostream tout(buft);
//    out << "prevState\tattractor*basin\ttimeSeries\n";
//	srand(time(NULL));
//	for (int i = 0; i < 100; i++) {
	RandomNetwork r(11);
	Analysis random(&r);
	random.basins();
	r.print();
//		r.basins(tout);
//		tout << endl << i + 1;
//		r.print(out);
//			r.fixedPointShort(r.binStr(1092));
//		r.graph(r.createGV("rnd", i));
//	}
//    csv.close();
//    txt.close();
//
//    cout<< endl;
//	RandomNetwork r2(n);
//	r2.basins(cout);
//	r2.print(cout);

	Network csv(*(n.fromCSV("random5.csv")));
//	csv.print();

	return 0;
}

/*
 * TODO: control strength >> number of linked active nodes:number of active nodes; generate R0
 * TODO: generate 100 11-nodes random networks, run 10 basins() times
 * convert binary on/off states to gene expression data
 * 	^-- snapshot, average of transient states (from initial on but prior to fixed point), other options??
 * create random network, ensure that all nodes are either +/- regulating another
 * VISUALIZATION of network
 * TODO: graph #attractors x edges // x activating/inhibitory links ratio
 * TODO: describe gene expression generation
 */
