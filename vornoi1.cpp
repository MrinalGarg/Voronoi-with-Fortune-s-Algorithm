#include<bits/stdc++.h>
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
			return ((x==a.x) &&(y == a.y));
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
point create_circle_event(point p1,point p2,point p3){
	double xa,ya;
	linear_solver(2*(p1.x-p2.x),2*(p1.y-p2.y),modd(p2)-modd(p1),2*(p3.x - p2.x),2*(p3.y - p2.y),modd(p2) - modd(p3),xa,ya);
	double r = modd({xa - p3.x, ya - p3.y});
	ya-=r;
	point C(xa,ya);
	return C;
}
void add_circle_event(point p1,point p2,point p3,breakpoint a, bool next){
	double xa,ya;
	linear_solver(2*(p1.x-p2.x),2*(p1.y-p2.y),modd(p2)-modd(p1),2*(p3.x - p2.x),2*(p3.y - p2.y),modd(p2) - modd(p3),xa,ya);
	double r = trunc(sqrt(modd({xa - p3.x, ya - p3.y})));
	ya-=r;
	if(ya <=yl){
	point e(xa,ya);
	arc x(a,next);
	m[e] = x;
	EQ.insert(make_pair(e,false));}
}
void add_edge(breakpoint a, point p){
	if(vd.find(a) == vd.end()){
		edge e;
		e.isstart = true;
		e.start = p;
		vd[a] = e;
	}
	else{
		vd[a].isend = true;
		vd[a].end = p;
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
			add_edge(breakpoint(p,o2,true),d);
			T.insert(breakpoint(p,o2,false));
			add_edge(breakpoint(p,o2,false),d);
			// pp(o3);
			// pp(o2);
			// pp(p);
			add_circle_event(o2,o3,p,a2,true);
		}
		else{
			point d(p.x,parabola_y(o3,p.x));
			T.insert(breakpoint(p,o3,true));
			add_edge(breakpoint(p,o3,true),d);
			T.insert(breakpoint(p,o3,false));
			add_edge(breakpoint(p,o3,false),d);
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
			add_edge(breakpoint(p,o2,true),d);
			T.insert(breakpoint(p,o2,false));
			add_edge(breakpoint(p,o2,false),d);
			// pp(o3);
			// pp(o2);
			// pp(p);
			add_circle_event(o2,o3,p,a2,false);
		}
		else{
			point d(p.x,parabola_y(o3,p.x));
			T.insert(breakpoint(p,o3,true));
			add_edge(breakpoint(p,o3,true),d);
			T.insert(breakpoint(p,o3,false));
			add_edge(breakpoint(p,o3,false),d);
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
		add_edge(breakpoint(p,o[min],true),d);
		T.insert(breakpoint(p,o[min],false));
		add_edge(breakpoint(p,o[min],false),d);
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
	breakpoint a1(m[p].B.f1,m[p].B.f2,m[p].B.plus);
	cout<<"\nThe circle event corresponds to the arc: ";
	cout<<"Breakpoint( ";
	pp(m[p].B.f1);
	cout<<" ";
	pp(m[p].B.f2);
	cout<<" ";
	cout<<parabola_solver({{m[p].B.f1,m[p].B.f2},m[p].B.plus});
	cout<<" ) ";
	cout<<m[p].next;
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
	if(a1 == *(T.begin())){
		a2 = *(++T.lower_bound(a1));
		a1next = *(++T.lower_bound(a1));
		begin = true;
	}
	else if(  a1 == *(--T.end())){
		front = true;
		a2 = *(--T.lower_bound(a1));
		a1prev = *(--T.lower_bound(a1));
		end = true;
	}
	else{
		a2 = *(++T.lower_bound(a1)); //(6,6) (5,3)
		a3 = *(--T.lower_bound(a1)); //(2,2) (6,6)
		a1next = *(++T.lower_bound(a1)); //.end()
		a1prev = *(--T.lower_bound(a1)); // (2,2) (6,6) .begin()
		if(parabola_solver({{a2.f1,a2.f2},a2.plus})!=parabola_solver({{a1.f1,a1.f2},a1.plus})){
			front = true;
			a2 = a3;
		}
	}
	cout<<"**";
	pp(a2.f1);
	pp(a2.f2);
	cout<<parabola_solver({{a2.f1,a2.f2},a2.plus});
	if(!end){
		if(a1next==*(--T.end())){
			doubleend = true;
		}
	}
	if(!begin){
		if(a1prev == *(--T.begin())){
			doublebegin = true;
		}
	}
	if(!doublebegin){
		a1dprev = *(--T.lower_bound(a1prev));
	}
	if(!doubleend){
		a1dnext = *(++T.lower_bound(a1next));
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
		add_edge(a1,p);
		T.erase(a2);
		add_edge(a2,p);
		if(k==parabola_solver(make_pair(make_pair(o[0],o[3]),true))){
			breakpoint a4(o[0],o[3],true);
			a3 = a4;
			T.insert(a4);
			add_edge(a4,p);
		}
		else{
			breakpoint a4(o[0],o[3],false);
			a3 = a4;
			T.insert(a4);
			add_edge(a4,p);
		}
		
	}
	else if(o[1]== o[3]){
		f = o[1];
		T.erase(a1);
		add_edge(a1,p);
		T.erase(a2);
		add_edge(a2,p);
		if(k==parabola_solver(make_pair(make_pair(o[0],o[2]),true))){
			breakpoint a4(o[0],o[2],true);
			a3 = a4;
			T.insert(a4);
			add_edge(a4,p);
		}
		else{
			breakpoint a4(o[0],o[2],false);
			a3 = a4;
			T.insert(a4);
			add_edge(a4,p);
		}
	}
	else if(o[0]==o[2]){
		f = o[0];
		T.erase(a1);
		add_edge(a1,p);
		T.erase(a2);
		add_edge(a2,p);
		if(k==parabola_solver(make_pair(make_pair(o[1],o[3]),true))){
			breakpoint a4(o[1],o[3],true);
			a3=a4;
			T.insert(a4);
			add_edge(a4,p);
		}
		else{
			breakpoint a4(o[1],o[3],false);
			a3=a4;
			T.insert(a4);
			add_edge(a4,p);
		}
	}
	else if(o[0]==o[3]){
		f = o[3];
		T.erase(a1);
		add_edge(a1,p);
		T.erase(a2);
		add_edge(a2,p);
		if(k==parabola_solver(make_pair(make_pair(o[1],o[2]),true))){
			breakpoint a4(o[1],o[2],true);
			a3=a4;
			T.insert(a4);
			add_edge(a4,p);
		}
		else{
			breakpoint a4(o[1],o[2],false);
			a3=a4;
			T.insert(a4);
			add_edge(a4,p);
		}
	}
	if(front){
		if(!end){
			point u(create_circle_event(f,a1next.f1,a1next.f2));
			m.erase(u);
			EQ.erase({u,false});
		}
		if(!doublebegin){
			point u(create_circle_event(f,a1dprev.f1,a1dprev.f2));
			m.erase(u);
			EQ.erase({u,false});
		}
		
	}
	else{
		if(!doubleend){
			point u(create_circle_event(f,a1dnext.f1,a1dnext.f2));
			m.erase(u);
			EQ.erase({u,false});
		}
		
		if(!begin){
			point u(create_circle_event(f,a1prev.f1,a1prev.f2));
			m.erase(u);
			EQ.erase({u,false});
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
void printtree(){
	cout<<"\n";
	cout<<"The Elements of Tree are\n";
	for(auto i  =T.begin();i!=T.end();i++){
		pp((*i).f1);
		pp((*i).f2);
		cout<<parabola_solver({{(*i).f1,(*i).f2},(*i).plus})<<"\n";
	}
}
void printEQ(){
	cout<<"\n";
	cout<<"The Elements of EQ are\n";
	for(auto i = EQ.begin();i!=EQ.end();i++){
		pp((*i).first);
	}
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
	T.insert(b2);
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
	for(auto it = vd.begin();it !=vd.end();it++){
	 	cout<<(*it).second.start.x<<" "<<(*it).second.start.y<<"\n";
		cout<<(*it).second.end.x<<" "<<(*it).second.end.y<<"\n";
	 }
	return 0;
}