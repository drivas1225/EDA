#include <vector>
#include<GL/glut.h>
#include<iostream>
#include<string>
#include<sstream>


using namespace std;

struct Point{
    double x;
    double y;

    Point(double px,double py ){
        x = px;
        y = py;
    }
    Point(){
        x=0;
        y=0;
    }

};

struct Rectan{
    double x;
    double y;
    double w;
    double h;

    Rectan(Point po , double w_, double h_){
        x=po.x;
        y=po.y;
        w=w_;
        h=h_;
    }
    Rectan(){
        double x=0;
        double y=0;
        double w=0;
        double h=0;


    }
    Rectan(const Rectan &po) {
        x=po.x;
        y=po.y;
        w=po.w;
        h=po.h;
        }

    bool contains(Point po){
        return (po.x > x - w &&
            po.x < x + w &&
            po.y > y - h &&
            po.y < y + h );
    }

    bool intersects( Rectan range){
    return !(range.x - range.w > x +w||
      range.x + range.w < x -w||
      range.y - range.h > y +h||
      range.y + range.h < y -h);
  }

};

class QuadTree
{
    public:
    Rectan boundary;
    int capacity;
    int used_capacity = 0;
    vector <Point> points;
    bool divided = false;
    vector <QuadTree> trees;

        QuadTree( Rectan boundary_ , int n){
            boundary = boundary_;
            capacity = n;
            used_capacity = 0;
            divided = false;
        }

        void  subdivide(){
           double x = boundary.x;
           double y = boundary.y;
           double w = boundary.w;
           double h = boundary.h;
           Rectan ne = Rectan(Point(x + w/2,y - h/2), w / 2, h / 2);
           QuadTree northeast = QuadTree(ne,capacity);
           Rectan nw = Rectan(Point(x - w/2,y - h/2), w / 2, h / 2);
           QuadTree northwest = QuadTree(nw,capacity);
           Rectan se = Rectan(Point(x + w/2,y + h/2), w / 2, h / 2);
           QuadTree southeast = QuadTree(se,capacity);
           Rectan sw = Rectan(Point(x - w/2,y + h/2), w / 2, h / 2);
           QuadTree southwest = QuadTree(sw,capacity);
           trees.push_back(northeast);
           trees.push_back(northwest);
           trees.push_back(southeast);
           trees.push_back(southwest);

        }
        bool buscar(Point point){
            bool ret = false;

            if( (!divided) &&boundary.contains(point)){
                for(int i = 0 ; i < points.size();i++){
                    if(point.x==points[i].x && point.y==points[i].y){
                        return true;
                    }
                }
                return false;
            }

            if(divided){
                for(int i = 0 ; i < 4;i++){
                    if(trees[i].boundary.contains(point)){
                            trees[i].buscar(point);
                }
                }
            }
                return ret;



        }


        bool insert_p(Point point){

            if(!boundary.contains(point)){
                return false;
            }
            if(buscar(point)){
                return false;
            }

            if(used_capacity < capacity ){
                points.push_back(point);
                used_capacity = used_capacity + 1;
                return true;
            } else {
                if (!divided) {
                    subdivide();
                    divided = true;
                    //cout<<"dividido";
                    for(int i = 0; i <points.size();i++){
                        for(int j = 0; j < 4 ;j++){
                                trees[j].insert_p(points[i]);
                        }
                    }

                }
                //points.clear();

                    for(int j = 0; j < 4 ;j++){
                            if(trees[j].insert_p(point)){
                                points.clear();
                                return true;
                            }
                    }



        }
}



string found_path(Point p){
    string ret = "";
    string aux;
    if(divided){
        for(int i = 0 ; i < 4 ; i++){
            if(trees[i].boundary.contains(p)){
                std::stringstream sstm;
                sstm << ret << ((3-i)+1);
                ret = sstm.str();
                //ret += str;
                ret = ret + "/";
                aux = trees[i].found_path(p);
                ret +=aux;
            }
        }
    }

    return ret;
}

    bool dentro( Rectan range ){
        return (boundary.x-boundary.w < range.x-range.w&&
                boundary.x+boundary.w < range.x+range.w&&
                boundary.y-boundary.h < range.y-range.h&&
                boundary.y+boundary.h < range.y+range.h);
    }

    vector<Point> puntosRange(){
        if(divided){
            vector<Point> founds;
            vector<Point> aux;
            for(int j = 0; j < 4  ;j++){
                    aux = trees[j].puntosRange();
                    founds.insert(founds.end(), aux.begin(), aux.end() );
                }
            return founds;
        }

        return points;
    }

    vector<Point> consulta( Rectan range ){
        vector<Point> founds;
        vector<Point> aux;
        if(!boundary.intersects(range)){
                //cout<<"retornando"<<endl;
            return founds;
        }else{
            //cout<<"insertando puntos "<<points.size()<<endl;
            if(!divided){

                for(int i = 0; i < points.size();i++){
                if(range.contains(points[i])){
                    founds.push_back(points[i]);
                }
            }
            }

            if(divided){
                //if(dentro(range)){
                 //   return puntosRange();
                //}

                for(int j = 0; j < 4  ;j++){
                    aux = trees[j].consulta(range);
                    founds.insert(founds.end(), aux.begin(), aux.end() );
                }
            }
            //cout<<"founds "<<founds.size()<<endl;
            /*for(int i = 0 ; i < founds.size();i++){
                //found_path(founds[i]);
                cout<<founds[i].x<<" "<<founds[i].y<<": "<<found_path(founds[i])<<endl;;
            }*/
            return founds;

        }
    }

    void draw(){

        glColor3f(250,0,0);

        glPointSize(3.0f);
        glBegin(GL_POINTS);
        for(int i = 0; i <points.size();i++){
            glVertex2i(points[i].x,points[i].y);
        }
        glEnd();

        glColor3f(250,250,250);
        glBegin(GL_LINE_LOOP);
        glVertex2f(boundary.x - boundary.w , boundary.y-boundary.h);
        glVertex2f(boundary.x + boundary.w , boundary.y-boundary.h);
        glVertex2f(boundary.x + boundary.w , boundary.y+boundary.h);
        glVertex2f(boundary.x - boundary.w , boundary.y+boundary.h);
        glEnd();

        if(divided){
            for(int j = 0; j < 4  ;j++){
                 trees[j].draw();
            }
        }

    }


};

