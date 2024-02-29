#include<bits/stdc++.h>
using namespace std;
#define point pair<double,double>
#define T_element pair<pair<pair<point,point>,bool>,bool>
double yl;
double modd(point p){
	return (p.first*p.first + p.second*p.second);
}
double quadratic_solver(double a,double b,double c,bool plus){
	if(plus){
		return (-b + sqrt(b*b-4*a*c))/(2*a);
	}
	else{
		return (-b-sqrt(b*b - 4*a*c))/(2*a);
	}
}
void linear_solver(double a1,double b1,double c1,double a2, double b2, double c2, double &x, double &y){
	x = (c2*a1 - c1*a2)/(b1*a2-b2*a1);
	y = (c2*b1 - c1*b2)/(b2*a1-b1*a2);
	return;
}
double parabola_y(point p,double x){
	return ((x*x - 2*p.first*x + modd(p) - yl*yl)/2*(p.second - yl));
}
double parabola_solver(pair<pair<point,point>,bool> p){
	point p1 = p.first.first;
	point p2 = p.first.second;
	double a = p2.second - p1.second;
	double b = -2*(p1.first*p2.second - p2.first*p1.second - yl*(p1.first - p2.first));
	double c = modd(p1)*(p2.second - yl) - modd(p2)*(p1.second - yl) -yl*yl*(p2.second - p1.second);
	return quadratic_solver(a,b,c,p.second);
}
class Compare2 {
public:
	typedef T_element value_type;
    bool operator()(const T_element p1,const T_element p2)const {	
    	double x1,x2;
    	if(p1.second == true){
    		x1 = p1.first.first.first.first;
    	}
    	else{
        	x1 = parabola_solver(p1.first);
        }
        if(p1.second == true){
    		x2 = p2.first.first.first.first;
    	}
    	else{
        	x2 = parabola_solver(p2.first);
        }
        return x1 < x2;
    }
};
set<T_element,Compare2> T;

class Compare {
public:
	typedef std::pair<point, bool> value_type;
    bool operator()(pair<point,bool> p1, pair<point,bool> p2) const
    {
        if (p1.first.second < p2.first.second) {
            return true;
        }
        return false;
    }
};
set<pair<point,bool>,Compare> EQ;
map<point,pair<T_element,bool>> m;
point create_circle_event(point p1,point p2,point p3){
	double xa,ya;
	linear_solver(2*(p1.first-p2.first),2*(p1.second-p2.second),modd(p2)-modd(p1),2*(p3.first - p2.first),2*(p3.second - p2.second),modd(p2) - modd(p3),xa,ya);
	double r = modd({xa - p3.first, ya - p3.second});
	ya-=r;
	return make_pair(xa,ya);
}
void add_circle_event(point p1,point p2,point p3,T_element a, bool next){
	double xa,ya;
	linear_solver(2*(p1.first-p2.first),2*(p1.second-p2.second),modd(p2)-modd(p1),2*(p3.first - p2.first),2*(p3.second - p2.second),modd(p2) - modd(p3),xa,ya);
	double r = modd({xa - p3.first, ya - p3.second});
	ya-=r;
	if(ya >=yl){
	auto q = make_pair(xa,ya);
	m[q] = make_pair(a,next);
	EQ.insert(make_pair(q,false));}
}
void handle_site_event(point p){
	T_element a = make_pair(make_pair(make_pair(p,p),false),true);
	auto b = T.lower_bound(a);
	if(b == T.end()){
		//have to write
	}
	else if(b == T.begin()){
		//have to write
	}
	else{
		T_element a1 = *b;
		b--;
		T_element a2 = *b;
		vector<point> o(4);
		o[0] = a1.first.first.first;
		o[1] = a1.first.first.second;
		o[2] = a2.first.first.first;
		o[3] = a2.first.first.second;
		int min =0;
		for(int i=1;i<4;i++){
			if(parabola_y(o[i],p.first)<parabola_y(o[min],p.first)){
				min = i;
			}  
		}
		
		T.insert(make_pair(make_pair(make_pair(o[min],p),true),false));
		T.insert(make_pair(make_pair(make_pair(o[min],p),false),false));
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
void handle_circle_event(point p){
	T_element a1 = m[p].first;
	T_element a2;
	if(m[p].second){
		a2 = *(T.lower_bound(a1)++);
	}
	else{
		a2 = *(T.lower_bound(a1)--);
	}
	vector<point> o(4);
	o[0] = a1.first.first.first;
	o[1] = a1.first.first.second;
	o[2] = a2.first.first.first;
	o[3] = a2.first.first.second;
	if(o[1] == o[2]){
		T.erase(a1);
		T.erase(a2);
		if(parabola_solver(a1.first)==parabola_solver(make_pair(make_pair(o[0],o[3]),true))){
			T_element a3 = make_pair(make_pair(make_pair(o[0],o[3]),true));
			T.insert(a3);
		}
		else{
			T_element a3 = make_pair(make_pair(make_pair(o[0],o[3]),false));
			T.insert(a3);
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
		v[i].first = x;
		v[i].second = y;
	}

	map<point,vector<point>> vd;

	for(int i=0;i<n;i++){
		EQ.insert({v[i],true});
	}
	if(EQ.size()<=1){

		return 0;
	}
	point p1 = (*(EQ.begin())).first;
	EQ.erase(EQ.begin());
	point p2 = (*(EQ.begin())).first;
	EQ.erase(EQ.begin());
	T_element w1 = make_pair(make_pair(make_pair(p1,p2),true),false);
	T_element w2 = make_pair(make_pair(make_pair(p1,p2),false),false);
	T.insert(w1);
	T.insert(w2);
	while(!EQ.empty()){
		point E;
		E = (*(EQ.begin())).first;
		yl = E.second;
		if((*(EQ.begin())).second){
			EQ.erase(EQ.begin());
			handle_site_event(E);
		}
		else{
			EQ.erase(EQ.begin());
			handle_circle_event(E);
		}
	}
	return 0;
}