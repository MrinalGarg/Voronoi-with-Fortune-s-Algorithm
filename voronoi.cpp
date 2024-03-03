#include<bits/stdc++.h>
#include<random>
#include <fstream>
using namespace std;
double yl;
#define MAX 10000000000000000
#define MIN -10000000000000000

//dummy function to truncate points in the initial phase of testing
double trunc(double a){
	//return double(int(a*100000.0)/double(100000.0));
	return a;
} 
// given a1x + b1y = c1 and a2x + b2y = c2, this function returns the value of x and y
void linear_solver(double a1,double b1,double c1,double a2, double b2, double c2, double &x, double &y, bool &valid){
	if(b2*a1!=b1*a2){
		valid = true;
	x = trunc((c2*b1 - c1*b2)/(b2*a1 - b1*a2));
	y = trunc((c1*a2 - c2*a1)/(b2*a1-b1*a2));
	}
	else{
		valid = false;
	}
	return;
}
// given a quadratic equation ax^2 + bx + c = 0, this function returns the value of x(handles the case when a = 0)
double quadratic_solver(double a,double b,double c,bool plus){
	if(a==0){
		return (-c/b);
	}
	if(plus){
		return trunc((-b + sqrt(fabs(b*b-4*a*c)))/(2*a));
	}
	else{
		return trunc((-b-sqrt(fabs(b*b - 4*a*c)))/(2*a));
	}

}
void handle(double a[],int n){
	 std::mt19937 gen(std::time(nullptr));
	 std::normal_distribution<double> distribution(0, 1e-4);
	 for (int i = 0; i < n; ++i) {
        a[i] = distribution(gen);
    }
}
	
// class point represents a point in the plane
class point{
	public:
		double x;
		double y;
		point(){
			x = 0.00000;
			y = 0.00000;
		}
		point(double a, double b){
			x = trunc(a);
			y = trunc(b);
		}
		//== comparator for the point(handling precision issues)
		bool operator ==(const point a) const{
			return (fabs(x-a.x)<1e-5 && fabs(y-a.y)<1e-5);
		}
		// < comparator for map functionality(chose a simple order, has no significance theoretically)
		bool operator<(const point& other) const {
			if (x < other.x)
				return true;
			if (x == other.x && y < other.y)
				return true;
			return false;
		}
		void print(){
			cout<<"("<<x<<","<<y<<") ";
		}
};
// prints the point
void pp(const point p){
	cout<<"("<<p.x<<","<<p.y<<") ";
}
// class breakpoint represents a breakpoint in the beachline
// f1,f2 are the focus of the two parabola that meet at the breakpoint 
// and plus is the sign in the quadratic solver required to obtain the intersection
class breakpoint{
	public:
		point f1;
		point f2;
		bool plus;
		breakpoint(point a,point b,bool s){
			f1 =a;
			f2 = b;
			plus = s;
		}
		breakpoint(){
			point f1(0,0);
			point f2(0,0);
			plus  = false;
		}
		//== comparator for the breakpoint 
		bool operator == (const breakpoint a){
			return ((f1 == a.f1) && (f2 == a.f2) && (plus == a.plus));
		}
		//< comparator for the breakpoint(again for functionality purposes, no particular order)
		bool operator < (const breakpoint & other)const{
			if(f1 < other.f1){
				return true;
			}
			if (f1 == other.f1 && f2 < other.f2)
            return true;
			if(f1 == other.f1 && f2 == other.f2){
				return plus < other.plus;
			}
        return false;
		}
};
// returns the square of the modulus of the point
double modd(point p){
	return (p.x*p.x + p.y*p.y);
}
// returns the y coordinate of the parabola given the x coordinate
double parabola_y(point p,double x){
	return trunc((x*x - 2*p.x*x + modd(p) - yl*yl)/(2*(p.y - yl)));
}
// returns the x coordinate of the intersection of the parabolas given the breakpoint
double parabola_solver(pair<pair<point,point>,bool> p){
	point p1 = p.first.first;
	point p2 = p.first.second;
	double a = p2.y - p1.y;
	double b = -2*(p1.x*p2.y - p2.x*p1.y - yl*(p1.x - p2.x));
	double c = modd(p1)*(p2.y - yl) - modd(p2)*(p1.y - yl) -yl*yl*(p2.y - p1.y);
	return quadratic_solver(a,b,c,p.second);
}
// returns the x coordinate of the intersection of the parabolas given the breakpoint
double breakpoint_solver(breakpoint a){
	return parabola_solver({{a.f1,a.f2},a.plus});
}
// prints the breakpoint
void printbp(breakpoint a){
	pp(a.f1);
	pp(a.f2);
	cout<<breakpoint_solver(a)<<"\n";
}
// class Tcompare is a comparator for the multiset T defined below,
// it compares the breakpoints based on the x coordinate of the intersection of the parabolas
// Also, for searching where does exactly a new site event "p" occurs on the beach line,
// we have searched a dummy breakpoint (p,p,0) in this multiset, and the comparator for that operation
// is given by "p1.f1==p1.f2"

class Tcompare {
	public:
	typedef breakpoint value_type;
    bool operator()(breakpoint p1,breakpoint p2)const {	
    	double x1,x2;
		if(p1.f1 == p1.f2){
			x1 = p1.f1.x;
		}
		else{
			x1 = parabola_solver({{p1.f1,p1.f2},p1.plus});
		}
		if(p2.f1 == p2.f2){
			x2 = p2.f1.x;
		}
		else{
			x2 = parabola_solver({{p2.f1,p2.f2},p2.plus});
		}
		if(fabs(x1-x2)<1e-5){
			yl-=1;
			x1 = parabola_solver({{p1.f1,p1.f2},p1.plus});
			x2 = parabola_solver({{p2.f1,p2.f2},p2.plus});
			yl+=1;
		}
		return x1 < x2;
    }
};
// class EQcompare is a comparator for the set EQ
// it compares the points based on the y coordinate
class EQcompare {
	public:
	typedef std::pair<point, bool> value_type;
    bool operator()(pair<point,bool> p1, pair<point,bool> p2) const
    {
        if (p1.first.y >= p2.first.y) {
            return true;
        }
        return false;
    }
};
// class arc represents an arc in the beachline(not used much finally)
class arc{
	public:
		breakpoint B;
		bool next;
		arc(breakpoint a,bool b){
			B = a;
			next =b;
		}
		arc(){
			breakpoint B;
			next = false;
		}
};
// class edge represents an edge in the voronoi diagram
class edge{
	public:
		bool isstart; // has the edge started
		bool isend; // has the edge ended
		point start; //start point of edge
		point end;//end point of edge
		edge(){
			isstart = false;
			isend = false;
			point start(MAX,MAX);//dummy...all the edges are handled in the code finally
			point end(MAX,MAX); //'''
		} 
};
multiset<breakpoint,Tcompare> T;//beachline...stored as a sorted list of breakpoints
set<pair<point,bool>,EQcompare> EQ;//event queue stored as a sorted list of points with a bool representing the type of event
map<breakpoint, edge> vd;//voronoi diagram stored as a map of breakpoints to edges
map<point,arc> m;//circle event(not used much finally)
// prints the beachline < focus 1, focus 2, x coordinate of the breakpoint>
void printtree(){
	cout<<endl;
	cout<<"The Elements of Tree are\n";
	for(auto i  =T.begin();i!=T.end();i++){
		pp((*i).f1);
		pp((*i).f2);
		cout<<parabola_solver({{(*i).f1,(*i).f2},(*i).plus})<<endl;
	}
}
// prints the event queue
void printEQ(){
	cout<<endl;
	cout<<"The Elements of EQ are\n";
	for(auto i = EQ.begin();i!=EQ.end();i++){
		pp((*i).first);
	}
}
// given the focus of the three parabolas, this function adds the circle event to the event queue
void add_circle_event(point p1,point p2,point p3,breakpoint a, bool next){
	if(p1==p2 || p2==p3 || p1==p3){
		return;
	}
	double xa,ya;
	// pp(p1);
	// pp(p2);
	// pp(p3);
	bool valid; // if the perpendicular bisectors are not parallel, we can only then have a circle event
	linear_solver(2*(p1.x-p2.x),2*(p1.y-p2.y),modd(p2)-modd(p1),2*(p3.x - p2.x),2*(p3.y - p2.y),modd(p2) - modd(p3),xa,ya,valid); // simple computes the intersection of the perpendicular bisectors
	if(!valid){
		return;
	}
	double r = trunc(sqrt(fabs(modd({xa - p3.x, ya - p3.y}))));
	ya-=r; // subtractinf the radius
	if(ya <yl){ // check is the supposed circle event occuring above the sweep line(if no then add)
	point e(xa,ya);
	arc x(a,next);
	m[e] = x;
	EQ.insert(make_pair(e,false));}
}
// adds the edge to the voronoi diagram map
void add_edge(breakpoint a){
	if(vd.find(a) == vd.end()){
		edge e;
		e.isstart = true;
		e.start = point(breakpoint_solver(a),parabola_y(a.f1,breakpoint_solver(a)));
		vd[a] = e;
	}
	else{
		vd[a].isend = true;
		vd[a].end = point(breakpoint_solver(a),parabola_y(a.f1,breakpoint_solver(a)));
	}
}
//One of the main functions....handles the site event
void handle_site_event(point p){
	breakpoint a(p,p,false);
	auto b = T.lower_bound(a); // as explained above, we are searching for the two breakpoints in between which the new site event occurs
	if(b == T.end()){ // edge cases where the new site event occurs at the end of the beachline
		breakpoint a2 = *(--b);
		point o2 = a2.f1;
		point o3 = a2.f2;
		int min = 0;
		if(parabola_y(o2,p.x) < parabola_y(o3,p.x)){ // compute the parabola that is lowest, i.e under which the new site event occurs
			min = 2;
		}
		else{
			min = 3;
		}
		if(min==2){
			point d(p.x,parabola_y(o2,p.x));// adding of the two new breakpoints
			T.insert(breakpoint(p,o2,true));
			vd[(breakpoint(p,o2,true))].isstart = true;
			vd[(breakpoint(p,o2,true))].start = d;
			T.insert(breakpoint(p,o2,false));
			vd[(breakpoint(p,o2,false))].isstart = true;
			vd[(breakpoint(p,o2,false))].start = d;
			add_circle_event(o2,o3,p,a2,true); // adding of the circle event(only one)
		}
		else{
			point d(p.x,parabola_y(o3,p.x));// adding of the two new breakpoints
			T.insert(breakpoint(p,o3,true));
			vd[(breakpoint(p,o3,true))].isstart = true;
			vd[(breakpoint(p,o3,true))].start = d;
			T.insert(breakpoint(p,o3,false));
			vd[(breakpoint(p,o3,false))].isstart = true;
			vd[(breakpoint(p,o3,false))].start = d;
			add_circle_event(o2,o3,p,a2,true);// adding of the circle event(only one)
		}
	}// edge case where the new site event occurs at the beginning of the beachline
	else if(b == T.begin()){
		breakpoint a2 = *b;
		point o2 = a2.f1;
		point o3 = a2.f2;
		int min = 0;
		if(parabola_y(o2,p.x) < parabola_y(o3,p.x)){
			min = 2;
		}
		else{
			min = 3;
		}
		if(min==2){
			point d(p.x,parabola_y(o2,p.x));
			T.insert(breakpoint(p,o2,true));
			vd[(breakpoint(p,o2,true))].isstart = true;
			vd[(breakpoint(p,o2,true))].start = d;
			T.insert(breakpoint(p,o2,false));
			vd[(breakpoint(p,o2,false))].isstart = true;
			vd[(breakpoint(p,o2,false))].start = d;
			add_circle_event(o2,o3,p,a2,false);
		}
		else{
			point d(p.x,parabola_y(o3,p.x));
			T.insert(breakpoint(p,o3,true));
			vd[(breakpoint(p,o3,true))].isstart = true;
			vd[(breakpoint(p,o3,true))].start = d;
			T.insert(breakpoint(p,o3,false));
			vd[(breakpoint(p,o3,false))].isstart = true;
			vd[(breakpoint(p,o3,false))].start = d;
			add_circle_event(o2,o3,p,a2,false);
		}
	}// general case where the new site event occurs in between two breakpoints
	else{
		breakpoint a1 = *b;
		b--;
		breakpoint a2 = *b;
		vector<point> o(4); // 4 focuses of the parabola where two are same(the actual lowest arc and the minimum)
		o[0] = a1.f1;
		o[1] = a1.f2;
		o[2] = a2.f1;
		o[3] = a2.f2;
		int min =0;
		for(int i=1;i<4;i++){
			if(parabola_y(o[i],p.x)<parabola_y(o[min],p.x)){
				min = i;
			}  
		}
		point d(p.x,parabola_y(o[min],p.x));
		T.insert(breakpoint(p,o[min],true));
		vd[(breakpoint(p,o[min],true))].isstart = true;
		vd[(breakpoint(p,o[min],true))].start = d;
		T.insert(breakpoint(p,o[min],false));
		vd[(breakpoint(p,o[min],false))].isstart = true;
		vd[(breakpoint(p,o[min],false))].start = d;
		add_circle_event(o[0],o[1],p,a1,false);// two possible new circle events
		add_circle_event(o[2],o[3],p,a2,true);
	}
	return;
}
// a good function which makes life easy, given two adjacent breakpoints, 
//it checks if the circle event is possible and adds it to the event queue
void check_and_add(breakpoint a,breakpoint b){
	vector<point> o(4);
	o[0] = a.f1;
	o[1] = a.f2;
	o[2] = b.f1;
	o[3] = b.f2;
	if(o[0]==o[2] || o[0]==o[3]){
		add_circle_event(o[1],o[2],o[3],a,true);
	}
	else if(o[1] == o[2] || o[1]==o[3]){
		add_circle_event(o[0],o[2],o[3],a,true);
	}
}
void handle_circle_event(point p){
	breakpoint a1; // one of the breakpoints that are disappearing(earlier one)
	breakpoint a2; // the other breakpoint that is disappearing
	breakpoint a3; // breakpoint that is being added
	breakpoint prev; // breakpoint prev to a1
	breakpoint next; //... next to a2
	bool end = false; // if the breakpoint a2 is the last breakpoint in the beachline
	bool begin = false; // if the breakpoint a1 is the first breakpoint in the beachline
	auto it = T.lower_bound(breakpoint(p,p,true)); // iterator which will finally point to the earlier of the two breakpoints disappearing
	//important edge cases
	//the lower bound can return iterator to three possible points in the beachline due to precision error
	// either a1 or a2 or the breakpoint next to a2
	if(it==T.end()){
		it--;
		it--;
		a1 = *it;
		a2 = *(++it);
		it--;
	}
	else if(fabs(p.x - breakpoint_solver(*it))<1e-5){
		if(fabs(p.x - breakpoint_solver(*(--it)))<1e-5){//lower_bound points to a2
			a1 = *it;
			it++;
			a2 = *it;
			it--;
		}
		else{//lower_bound points to a1
			it++;
			a1 = *it;
			it++;
			a2 = *it;
			it--;
		}
	}
	else{//lower_bound points to next to a2
		it--;
		it--;
		a1 = *it;
		a2 = *(++it);
		it--;
	}
	//if breakpoints arent converging, this is a fake circle event
	if(!((fabs(p.x - breakpoint_solver(a1))<1e-6)&&(fabs(p.x - breakpoint_solver(a2))<1e-5))){//fake circle event(shoudn't occur)
		return;
	}
	//if size is two then obv a1 and a2 are the only breakpoints
	if(T.size()==2){
		end = true;
		begin = true;
	}
	else{
		if(it==T.begin()){
			begin = true;
		}
		if(it == --T.end()){
			end = true;
		}
	}
	if(!begin){
		prev = *(--it);
		it++;
	}
	if(!end){
		it++;
		it++;
		next = *it;
		it--;
		it--;
	}
	vector<point> o(4);
	o[0] = a1.f1;
	o[1] = a1.f2;
	o[2] = a2.f1;
	o[3] = a2.f2;
	double k = parabola_solver({{a1.f1,a1.f2},a1.plus}); // x coordinate of point of convergence
	point f; // focus of the arc disappearing is the common focus in the breakpoints
	//check which is the common point
	if(o[1] == o[2]){
		f = o[1];//erase the two breakpoints and add the new breakpoint
		T.erase(a1);
		add_edge(a1);
		T.erase(a2);
		add_edge(a2);
		//if else for checking the sign in quadratic solver
		if(fabs(k-parabola_solver(make_pair(make_pair(o[0],o[3]),true)))<1e-5){
			breakpoint a4(o[0],o[3],true);
			a3 = a4;
			T.insert(a4);
			add_edge(a4);
		}
		else{
			breakpoint a4(o[0],o[3],false);
			a3 = a4;
			T.insert(a4);
			add_edge(a4);
		}
	}
	else if(o[1]== o[3]){
		f = o[1];
		T.erase(a1);
		add_edge(a1);
		T.erase(a2);
		add_edge(a2);
		if(fabs(k-parabola_solver(make_pair(make_pair(o[0],o[2]),true)))<1e-5){
			breakpoint a4(o[0],o[2],true);
			a3 = a4;
			T.insert(a4);
			add_edge(a4);
		}
		else{
			breakpoint a4(o[0],o[2],false);
			a3 = a4;
			T.insert(a4);
			add_edge(a4);
		}
	}
	else if(o[0]==o[2]){
		f = o[0];
		T.erase(a1);
		add_edge(a1);
		T.erase(a2);
		add_edge(a2);
		if(fabs(k-parabola_solver(make_pair(make_pair(o[1],o[3]),true)))<1e-5){
			breakpoint a4(o[1],o[3],true);
			a3=a4;
			T.insert(a4);
			add_edge(a4);
		}
		else{
			breakpoint a4(o[1],o[3],false);
			a3=a4;
			T.insert(a4);
			add_edge(a4);
		}
	}
	else if(o[0]==o[3]){
		f = o[3];
		T.erase(a1);
		add_edge(a1);
		T.erase(a2);
		add_edge(a2);
		if(fabs(k-parabola_solver(make_pair(make_pair(o[1],o[2]),true)))<1e-5){
			breakpoint a4(o[1],o[2],true);
			a3=a4;
			T.insert(a4);
			add_edge(a4);
		}
		else{
			breakpoint a4(o[1],o[2],false);
			a3=a4;
			T.insert(a4);
			add_edge(a4);
		}
	}
	//check and add the two possible new circle events
	if(!begin){
		check_and_add(prev,a3);

	}
	if(!end){
		check_and_add(next,a3);
	}
	return;
}

int main(){
	int n;
	cin>>n;
	double w[2*n];
	handle(w,2*n);
	vector<point> v(n);
	for(int i=0;i<n;i++){
		double x,y;
		cin>>x>>y;
		point p(x+w[i],y+w[2*n-1-i]);
		v[i] = p;
	}
	for(int i=0;i<n;i++){
		EQ.insert({v[i],true});
	}
	if(EQ.size()<=1){
		return 0;
	}
	//handle the first two points, seperately
	point p1 = (*(EQ.begin())).first;
	EQ.erase(EQ.begin());
	point p2 = (*(EQ.begin())).first;
	EQ.erase(EQ.begin());
	
	breakpoint b1(p1,p2,true);
	breakpoint b2(p1,p2,false);
	yl = p2.y;
	T.insert(b1);
	edge a;
	a.isstart = true;
	a.start.x = p2.x;
	a.start.y = parabola_y(p1,p2.x);
	edge b;
	b.isstart = true;
	b.start.x = p2.x;
	b.start.y = parabola_y(p1,p2.x);
	T.insert(b2);
	vd[b1] = a;
	vd[b2] = b;
	//***UNCOMMENT/COMMENT THE FOLLOWING COUT AND PRINT LINES TO GENERATE/NOT-GENERATE THE LOG FILE 
	//WHICH IS A NUMERICAL SIMULATION OF THE PROGRAM***//
	//NOTE: THE LOG FILE IS GENERATED IN THE SAME DIRECTORY AS THE PROGRAM AND IS HEAVY, SO GENERATION TAKES TIME
	//CODE COMPLEXITY WITHOUT LOG FILE GENERATION IN O(nlogn)
	cout<<"-------\n";
	cout<<"After Handling first two points:";
	printEQ();
	printtree();
	while(!EQ.empty()){
		point E;
		E = (*(EQ.begin())).first;
		yl = E.y;
		cout<<"-------"<<endl;
		if((*(EQ.begin())).second){
			cout<<"Handling Site Event: ";
			E.print();
			cout<<"\nBefore Handling:";
			printEQ();
			printtree();
			EQ.erase(EQ.begin());
			handle_site_event(E);
		}
		else{
			cout<<"Handling Circle Event: ";
			E.print();
			cout<<"\nBefore Handling:";
			printEQ();
			printtree();
			EQ.erase(EQ.begin());
			handle_circle_event(E);
		}
		cout<<"\nAfter Handling:";
		printEQ();
		printtree();
	}
	vector<pair<point,point>> v2;
	std::ofstream outputFile;
	outputFile.open("output.txt");
	yl = yl-1;
	//IMPORTANT, check what brteakpoints are left in  the tree...
	// these are the ones that continue forever and correspond to infinite half edges
	for(auto i = T.begin();i!=T.end();i++){
		vd[*i].end.x = breakpoint_solver(*i);
		vd[*i].end.y = parabola_y((*i).f1,breakpoint_solver(*i));
	}
	if (!outputFile.is_open()) {
        std::cerr << "Error: Unable to open the file for writing!\n";
        return 1; // Return an error code
    }
	//output the edges into output.txt
	for(auto it = vd.begin();it !=vd.end();it++){
	 	outputFile<<(*it).second.start.x<<" "<<(*it).second.start.y<<" ";
		outputFile<<(*it).second.end.x<<" "<<(*it).second.end.y<<endl;
	}
	outputFile.close();
	return 0;
}