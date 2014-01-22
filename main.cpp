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
using namespace std;

int main(int argc, const char * argv[]) {

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
	n.addProtein(f).addProtein(h).addProtein(g).addProtein(k).addProtein(j).addProtein(
			e).addProtein(d).addProtein(i).addProtein(a).addProtein(b).addProtein(
			c).addProtein(d).addProtein(e).addProtein(b);

//	n.print();
	cout << "\nTemporal Evolution";
//	n.fixedPoint("10001000100"); //start phase Cln3, Cdh1 and Sic1 are ON.
	n.fixedPointShort("10001000100");
//	n.fixedPoint(n.binStr(1092));
	n.print();
	n.reset();
//	n.setStates("00001000100");
	n.graph();

//	cout << "\nFixed points and basin sizes\n";
	n.basins();

	RandomNetwork r(n);
//	r.addProtein(new Protein("blah"));
	r.randomise();

	r.print();
	r.basins(r.createGV("random"));
//	r.fixedPointShort(r.binStr(1092));
	//	r.graph();
	return 0;
}

/*
 * TODO: control strength >> number of linked active nodes:number of active nodes
 * TODO: convert binary on/off states to gene expression data
 * 	^-- snapshot, average of transient states (from initial on but prior to fixed point), other options??
 * TODO: create random network, ensure that all nodes are either +/- regulating another
 * VISUALIZATION of network
 */
