#include <bits/stdc++.h>
using namespace std;

//A struct to hold the x and y coordinates of a point
struct Point{
    double x,y;
};

// Function to calculate the Euclidean distance between two points
double distance(Point p1, Point p2){
    return sqrt((p1.x-p2.x)*(p1.x-p2.x)+(p1.y-p2.y)*(p1.y-p2.y));
}

//Brute force method to find the closest pair of points
double bforce(const vector<Point>& points, Point& p1, Point& p2) {
    double mindist=DBL_MAX;

    for(int i=0; i<points.size(); i++){
        for(int j=i+1; j<points.size(); j++){
            double dist=distance(points[i],points[j]);
            if(dist<mindist){
                mindist=dist;
                p1=points[i];
                p2=points[j];
            }
        }
    }
    return mindist;
}

//a comparator to sort points based on x-coordinate
bool cX(Point& a, Point& b){
    return a.x<b.x;
}
//a comparator to sort points based on y-coordinate
bool cY(Point& a, Point& b){
    return a.y<b.y;
}

//Function to find the closest pair in a strip
double clstrip(vector<Point>& strip, double d, Point& p1, Point& p2) {
    double mindist = d;
    sort(strip.begin(),strip.end(),cY);

    for(int i=0; i<strip.size(); i++){
        for (int j=i+1; j<strip.size() && (strip[j].y-strip[i].y)<mindist; j++) {
            double dist=distance(strip[i],strip[j]);
            if(dist<mindist){
                mindist=dist;
                p1=strip[i];
                p2=strip[j];
            }
        }
    }
    return mindist;
}

// Recursive function to find the closest pair of points
double recurcls(const vector<Point>&Px,const vector<Point>&Py,Point& p1, Point& p2) {
    if (Px.size() <= 3) {
        return bforce(Px,p1,p2);
    }

    int mid=Px.size()/2;
    Point midPoint=Px[mid];

    vector<Point>Pyl,Pyr;
    for(int i=0; i<Py.size(); i++){
        if(Py[i].x < midPoint.x || (Py[i].x == midPoint.x && Py[i].y <= midPoint.y))
            Pyl.push_back(Py[i]);
        else
            Pyr.push_back(Py[i]);
    }

    Point lP1, lP2, rP1, rP2;
    double lmindist = recurcls(vector<Point>(Px.begin(), Px.begin()+mid), Pyl, lP1, lP2);
    double rmindist = recurcls(vector<Point>(Px.begin()+mid, Px.end()), Pyr, rP1, rP2);

    double mindist = (lmindist < rmindist) ? lmindist : rmindist;
    if (lmindist < rmindist) {
        p1 = lP1;
        p2 = lP2;
    } else {
        p1 = rP1;
        p2 = rP2;
    }

    vector<Point> strip;
    for(int i=0; i<Py.size(); i++){
        if(fabs(Py[i].x-midPoint.x) < mindist) {
            strip.push_back(Py[i]);
        }
    }

    return min(mindist, clstrip(strip,mindist,p1,p2));
}

// Function to find the closest pair using Divide and Conquer
double divcon(vector<Point>& points, Point& p1, Point& p2) {
    vector<Point>Px=points,Py=points;
    sort(Px.begin(),Px.end(),cX);
    sort(Py.begin(),Py.end(),cY);

    return recurcls(Px,Py,p1,p2);
}

//this function generates random points
vector<Point> genpoint(int n){
    vector<Point>points;
    set<pair<int, int>> uniquePoints;
    while(points.size()<n){
        int x, y;
        x = rand() % 100000;
        y = rand() % 100000;
        if(uniquePoints.find({x,y})==uniquePoints.end()){
            uniquePoints.insert({x,y});
            points.push_back({double(x),double(y)});
        }
    }
    return points;
}

int main() {
    srand(time(0));

    for(int n=1000; n<=15000; n+=1000){ // Number of points
    vector<Point> points=genpoint(n);

    // Brute Force Approach
    Point p1Brute, p2Brute;
    auto start = chrono::high_resolution_clock::now();
    double brutemin = bforce(points, p1Brute, p2Brute);
    auto finish=chrono::high_resolution_clock::now();
    double t1 = chrono::duration<double>(finish-start).count();

    // Divide and Conquer Approach
    Point p1DC, p2DC;
    start=chrono::high_resolution_clock::now();
    double divconmin = divcon(points, p1DC, p2DC);
    finish= chrono::high_resolution_clock::now();
    double t2=chrono::duration<double>(finish - start).count();

    cout<<"Number of points:"<<n<<endl;
    cout<<"Brute Force:"<<endl;
    cout<<"Closest Pair: ("<<p1Brute.x<<", "<<p1Brute.y<<") and ("
        <<p2Brute.x<<", "<<p2Brute.y<<")"<<endl;
    cout<<"Distance: "<<brutemin<<", Time: "<<t1<<" seconds"<<endl;

    cout<<"Divide and Conquer:\n";
    cout<<"Closest Pair: ("<<p1DC.x<<", "<<p1DC.y<<") and ("
        <<p2DC.x <<", "<<p2DC.y<< ")"<<endl;
    cout<<"Distance: "<<divconmin<<", Time: "<<t2<<" seconds"<<endl;
    cout<<"------------------------------------------------------"<<endl;
    }

}
