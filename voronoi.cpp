#include<bits/stdc++.h>
#include <fstream>
using namespace std;
double yl;
#define MAX 10000000000000000
#define MIN -1000000000000000
double trunc(double a){
	//return double(int(a*100000.0)/double(100000.0));
	return a;
}
void linear_solver(double a1,double b1,double c1,double a2, double b2, double c2, double &x, double &y){
	x = trunc((c2*b1 - c1*b2)/(b2*a1 - b1*a2));
	y = trunc((c1*a2 - c2*a1)/(b2*a1-b1*a2));
	return;
}
double quadratic_solver(double a,double b,double c,bool plus){
	if(plus){
		return trunc((-b + sqrt(b*b-4*a*c))/(2*a));
	}
	else{
		return trunc((-b-sqrt(b*b - 4*a*c))/(2*a));
	}
}
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
		bool operator ==(const point a) const{
			return (fabs(x-a.x)<0.0001 && fabs(y-a.y)<0.0001);
		}
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
void pp(const point p){
	cout<<"("<<p.x<<","<<p.y<<") ";
}
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
		bool operator == (const breakpoint a){
			return ((f1 == a.f1) && (f2 == a.f2) && (plus == a.plus));
		}
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

double modd(point p){
	return (p.x*p.x + p.y*p.y);
}
double parabola_y(point p,double x){
	return trunc((x*x - 2*p.x*x + modd(p) - yl*yl)/(2*(p.y - yl)));
}
double parabola_solver(pair<pair<point,point>,bool> p){
	point p1 = p.first.first;
	point p2 = p.first.second;
	double a = p2.y - p1.y;
	double b = -2*(p1.x*p2.y - p2.x*p1.y - yl*(p1.x - p2.x));
	double c = modd(p1)*(p2.y - yl) - modd(p2)*(p1.y - yl) -yl*yl*(p2.y - p1.y);
	return quadratic_solver(a,b,c,p.second);
}
double breakpoint_solver(breakpoint a){
	return parabola_solver({{a.f1,a.f2},a.plus});
}
void printbp(breakpoint a){
	pp(a.f1);
	pp(a.f2);
	cout<<breakpoint_solver(a)<<"\n";
}
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
		if(fabs(x1-x2)<0.0000001){
			yl-=1;
			x1 = parabola_solver({{p1.f1,p1.f2},p1.plus});
			x2 = parabola_solver({{p2.f1,p2.f2},p2.plus});
			yl+=1;
		}
		return x1 < x2;
    }
};
class EQcompare {
	public:
	typedef std::pair<point, bool> value_type;
    bool operator()(pair<point,bool> p1, pair<point,bool> p2) const
    {
        if (p1.first.y > p2.first.y) {
            return true;
        }
        return false;
    }
};
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
class edge{
	public:
		bool isstart;
		bool isend;
		point start;
		point end;
		edge(){
			isstart = false;
			isend = false;
			point start(MAX,MAX);
			point end(MAX,MAX);
		} 
};
multiset<breakpoint,Tcompare> T;
set<pair<point,bool>,EQcompare> EQ;
map<breakpoint, edge> vd;
map<point,arc> m;
void printtree(){
	cout<<endl;
	cout<<"The Elements of Tree are\n";
	for(auto i  =T.begin();i!=T.end();i++){
		pp((*i).f1);
		pp((*i).f2);
		cout<<parabola_solver({{(*i).f1,(*i).f2},(*i).plus})<<endl;
	}
}
void printEQ(){
	cout<<endl;
	cout<<"The Elements of EQ are\n";
	for(auto i = EQ.begin();i!=EQ.end();i++){
		pp((*i).first);
	}
}
point create_circle_event(point p1,point p2,point p3){
	double xa,ya;
	linear_solver(2*(p1.x-p2.x),2*(p1.y-p2.y),modd(p2)-modd(p1),2*(p3.x - p2.x),2*(p3.y - p2.y),modd(p2) - modd(p3),xa,ya);
	double r = modd({xa - p3.x, ya - p3.y});
	ya-=r;
	point C(xa,ya);
	return C;
}
void add_circle_event(point p1,point p2,point p3,breakpoint a, bool next){
	if(p1==p2 || p2==p3 || p1==p3){
		return;
	}
	double xa,ya;
	linear_solver(2*(p1.x-p2.x),2*(p1.y-p2.y),modd(p2)-modd(p1),2*(p3.x - p2.x),2*(p3.y - p2.y),modd(p2) - modd(p3),xa,ya);
	double r = trunc(sqrt(modd({xa - p3.x, ya - p3.y})));
	ya-=r;
	if(ya <yl){
	point e(xa,ya);
	arc x(a,next);
	m[e] = x;
	EQ.insert(make_pair(e,false));}
}
void add_edge(breakpoint a){
	cout<<"\nADDING EDGE\n";
	if(vd.find(a) == vd.end()){
		edge e;
		e.isstart = true;
		e.start = point(breakpoint_solver(a),parabola_y(a.f1,breakpoint_solver(a)));
		pp(e.start);
		vd[a] = e;
	}
	else{
		vd[a].isend = true;
		vd[a].end = point(breakpoint_solver(a),parabola_y(a.f1,breakpoint_solver(a)));
		pp(vd[a].end);
	}
}
void handle_site_event(point p){
	breakpoint a(p,p,false);
	auto b = T.lower_bound(a);
	if(b == T.end()){
		breakpoint a2 = *(--b);
		point o2 = a2.f1;
		point o3 = a2.f2;
		// cout<<"\n((((((((((((((";
		// pp(o2);
		// pp(o3);
		// cout<<"))))))))))))))\n";
		// cout<<"\n(((((("<<parabola_y(o2,p.x)<<" "<<parabola_y(o3,p.x)<<")))))))))))\n";
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
			// pp(o3);
			// pp(o2);
			// pp(p);
			add_circle_event(o2,o3,p,a2,true);
		}
		else{
			point d(p.x,parabola_y(o3,p.x));
			T.insert(breakpoint(p,o3,true));
			vd[(breakpoint(p,o3,true))].isstart = true;
			vd[(breakpoint(p,o3,true))].start = d;
			T.insert(breakpoint(p,o3,false));
			vd[(breakpoint(p,o3,false))].isstart = true;
			vd[(breakpoint(p,o3,false))].start = d;
			// pp(o3);
			// pp(o2);
			// pp(p);
			add_circle_event(o2,o3,p,a2,true);
		}
	}
	else if(b == T.begin()){
		breakpoint a2 = *b;
		// cout<<"\n((((((((((((((";
		// pp(a2.f1);
		// pp(a2.f2);
		// cout<<"))))))))))))))\n";
		point o2 = a2.f1;
		point o3 = a2.f2;
		int min = 0;
		// cout<<"\n(((((("<<parabola_y(o2,p.x)<<" "<<parabola_y(o3,p.x)<<")))))))))))\n";
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
			// pp(o3);
			// pp(o2);
			// pp(p);
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
			// pp(o3);
			// pp(o2);
			// pp(p);
			add_circle_event(o2,o3,p,a2,false);
		}
	}
	else{
		breakpoint a1 = *b;
		b--;
		breakpoint a2 = *b;
		vector<point> o(4);
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
		add_circle_event(o[0],o[1],p,a1,false);
		add_circle_event(o[2],o[3],p,a2,true);
		if(o[min]==o[0]){
			EQ.erase({create_circle_event(o[1],o[2],o[3]),false});
		}
		else{
			EQ.erase({create_circle_event(o[0],o[2],o[3]),false});
		}
		//updating voronoi diagram left
	}
	return;
}
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
	// printEQ();
	// cout<<"***";
	breakpoint a1(m[p].B.f1,m[p].B.f2,m[p].B.plus);
	// pp(a1.f1);
	// pp(a1.f2);
	// cout<<parabola_solver({{a1.f1,a1.f2},a1.plus})<<endl;

	breakpoint a2;
	breakpoint a3;
	breakpoint a1prev;
	breakpoint a1next;
	breakpoint a1dprev;
	breakpoint a1dnext;
	bool end = false;
	bool begin = false;
	bool front = false;
	bool doubleend = false;
	bool doublebegin = false;
	auto it = T.begin();
	if(a1 == *(T.begin())){
		it = T.begin();
		a2 = *(++it);
		it--;
		a1next = *(++it);
		it--;
		begin = true;
		if(T.size()==2){
			doubleend = true;
		}
	}
	else if(  a1 == *(--T.end())){
		front = true;
		it = --T.end();
		a2 = *(--it);
		it++;
		a1prev = *(--it);
		it++;
		end = true;
		if(T.size()==2){
			doublebegin = true;
		}
	}
	else{
		it = T.lower_bound(a1);
		//  cout<<"debug1";
		// pp(it->f1);
		// pp(it->f2);
		// cout<<parabola_solver({{it->f1,it->f2},it->plus});
		auto b = it;
		if(!(a1==*it)){
			while((!(a1==*b))&&!(b==--T.begin())){
				b--;
			}
		}
		if(!(a1==*b)){
			b = it;
			if(!(a1==*(++b))){
				return;
			}
		}
		it = b;
		//cout<<"debug:";
		// pp(a1.f1);
		// pp(a1.f2);
		// cout<<parabola_solver({{a1.f1,a1.f2},a1.plus});
		// pp(it->f1);
		// pp(it->f2);
		// cout<<parabola_solver({{it->f1,it->f2},it->plus});
		a2 = *(++it);
		it--;
		a3 = *(--it); 
		it++;
		if(++it == (--T.end())){
			doubleend = true;
		}
		it--;
		if(--it == T.begin()){
			doublebegin = true;
		}
		it++;
		a1next = *(++it); //.end()
		it--;
		a1prev = *(--it); // (2,2) (6,6) .begin()
		it++;
		// cout<<"owjdc";
		// pp(a2.f1);
		// pp(a2.f2);
		// cout<<parabola_solver({{a2.f1,a2.f2},a2.plus});
		// cout<<"wejc";
		if(fabs(parabola_solver({{a2.f1,a2.f2},a2.plus})-parabola_solver({{a1.f1,a1.f2},a1.plus}))>0.001){
			front = true;
			a2 = a3;
		}	
	}
	// cout<<"eojof";
	// pp(a1.f1);
	// pp(a1.f2);
	// cout<<parabola_solver({{a1.f1,a1.f2},a1.plus});
	// cout<<"eojkffc";
	// pp(a2.f1);
	// pp(a2.f2);
	// cout<<parabola_solver({{a2.f1,a2.f2},a2.plus});
	if(!doublebegin){
		a1dprev = *(--(--it));
		it++;
		it++;
	}
	if(!doubleend){
		a1dnext = *(++(++it));
		it--;
		it--;
	}
	vector<point> o(4);
	o[0] = a1.f1;
	o[1] = a1.f2;
	o[2] = a2.f1;
	o[3] = a2.f2;
	double k = parabola_solver({{a1.f1,a1.f2},a1.plus});
	double k1 = parabola_y(o[0],k);
	point z(k,k1);
	point f;
	
	if(o[1] == o[2]){
		f = o[1];
		T.erase(a1);
		add_edge(a1);
		T.erase(a2);
		add_edge(a2);
		if(fabs(k-parabola_solver(make_pair(make_pair(o[0],o[3]),true)))<0.001){
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
		if(fabs(k-parabola_solver(make_pair(make_pair(o[0],o[2]),true)))<0.001){
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
		if(fabs(k-parabola_solver(make_pair(make_pair(o[1],o[3]),true)))<0.001){
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
		//cout<<"jernf"<<endl;
		if(fabs(k-parabola_solver(make_pair(make_pair(o[1],o[2]),true)))<0.001){
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
	// printEQ();
	// cout<<"oenoe";
	// printtree();
	// cout<<"wpokdnc";
	// pp(f);
	if(front){
		if(!end){
			if(!(f==a1next.f1) && !(f==a1next.f2)){
				point u(create_circle_event(f,a1next.f1,a1next.f2));
				m.erase(u);
				EQ.erase({u,false});
			}
		}
		if(!doublebegin){
			if(!(f==a1dprev.f1) && !(f==a1dprev.f2)){
				point u(create_circle_event(f,a1dprev.f1,a1dprev.f2));
				m.erase(u);
				EQ.erase({u,false});
			}
		}
		
	}
	else{
		if(!doubleend){
			if(!(f==a1dnext.f1) && !(f==a1dnext.f2)){
				point u(create_circle_event(f,a1dnext.f1,a1dnext.f2));
				m.erase(u);
				EQ.erase({u,false});
			}
		}
		
		if(!begin){
			if(!(f==a1prev.f1) && !(f==a1prev.f2)){
				point u(create_circle_event(f,a1prev.f1,a1prev.f2));
				m.erase(u);
				EQ.erase({u,false});
			}
		}
	}
	if(!end){
		if(front){
			check_and_add(a3,a1next);
			if(!doublebegin){
				check_and_add(a1dprev,a3);
			}
		}
		else{
			if(!doubleend){
				check_and_add(a3,a1dnext);
			}
			if(!begin){
				check_and_add(a1prev,a3);
			}
		}
	}
	else{
		if(!doublebegin){
			check_and_add(a1dprev,a3);
		}
	}
	return;
}

int main(){
	int n;
	cin>>n;
	vector<point> v(n);
	for(int i=0;i<n;i++){
		double x,y;
		cin>>x>>y;
		point p(x,y);
		v[i] = p;
	}
	for(int i=0;i<n;i++){
		//cout<<"ijn"<<endl;
		EQ.insert({v[i],true});
	}
	if(EQ.size()<=1){
		return 0;
	}
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
	cout<<"-------\n";
	cout<<"After Handling first two points:";
	printEQ();
	printtree();
	while(!EQ.empty()){
		point E;
		E = (*(EQ.begin())).first;
		yl = E.y;
		cout<<"-------\n";
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
	for(auto i = T.begin();i!=T.end();i++){
		vd[*i].end.x = breakpoint_solver(*i);
		vd[*i].end.y = parabola_y((*i).f1,breakpoint_solver(*i));
	}
	if (!outputFile.is_open()) {
        std::cerr << "Error: Unable to open the file for writing!\n";
        return 1; // Return an error code
    }
	for(auto it = vd.begin();it !=vd.end();it++){
	 	outputFile<<(*it).second.start.x<<" "<<(*it).second.start.y<<" ";
		outputFile<<(*it).second.end.x<<" "<<(*it).second.end.y<<endl;
	}
	outputFile.close();
	return 0;
}