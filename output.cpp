#include <fstream>
#include <iostream>
#include <string>
#include <cstring>
#include <cmath>
#include <queue>
using namespace std;

enum {ABSOLUTE,INCREMENTAL};
enum {MM,INCH};
enum {MINUTE,REVOLUTION};
enum {XY,ZX,YZ};
enum {CLOCKWISE,COUNTERCLOCKWISE};
enum {G0,G1,G2};
const double pi = 3.141593;

class number{
    public:
        long long int value;
        number(){this->value = 0;}
        number(double x){
            this->value = 1000000 * x;
        }
        number operator+(number x){return (this->value+(double)x.value)/1000000.0;}
        number operator+(double x){return (this->value+(double)round(1000000*x))/1000000.0;}
        number operator-(number x){return (this->value-(double)x.value)/1000000.0;}
        number operator-(double x){return (this->value-(double)round(1000000*x))/1000000.0;}
        number operator*(number x){return (this->value*(double)x.value)/1000000000000.0;}
        number operator*(double x){return (this->value*(double)round(1000000*x))/1000000000000.0;}
        number operator/(number x){return (this->value/(double)x.value)/1.0;}
        number operator/(double x){return (this->value/(double)round(1000000*x))/1.0;}
        void operator+=(number x){this->value+=(double)x.value;}
        void operator+=(double x){this->value+=(double)round(x*1000000);}
        void operator-=(number x){this->value-=(double)x.value;}
        void operator-=(double x){this->value-=(double)round(x*1000000);}
        bool operator==(number x){return this->value==x.value;}
        bool operator!=(number x){return this->value!=x.value;}
        bool operator>=(number x){return this->value>=x.value;}
        bool operator<=(number x){return this->value<=x.value;}
        bool operator>(number x){return this->value>x.value;}
        bool operator<(number x){return this->value<x.value;}
        number operator-(){return (this->value*(double)-1)/1000000.0;}
        
};

number operator+(double x, number y){return ((double)round(1000000*x)+y.value)/1000000.0;}
number operator-(double x, number y){return ((double)round(1000000*x)-y.value)/1000000.0;}
number operator*(double x, number y){return ((double)round(1000000*x)*y.value)/1000000000000.0;}
number operator/(double x, number y){return ((double)round(1000000*x)/y.value)/1.0;}


double to_double(number x){
    return x.value/1000000.0;
}

number pow(number base, number exponent){
    double b = base.value/1000000.0;
    double e = exponent.value/1000000.0;
    return pow(b,e);
}

string to_string(number x){
    x = x/1000;
    if (x>=0){
        string res = to_string(x.value/1000);
        int y = x.value%1000;
        if (y!=0){
            res.append(".");
            if (y<100){
                res.append("0");
            }
            res.append(to_string(y));
        }
        return res;
    }
    else{
        x = -x;
        string res = "-";
        res.append(to_string(x.value/1000));
        int y = x.value%1000;
        if (y!=0){
            res.append(".");
            if (y<100){
                res.append("0");
            }
            res.append(to_string(y));
        }
        return res;
    }
}

class coordinate{
    public:
        number x;
        number y;
        number z;

        coordinate(){
            this->x = 0;
            this->y = 0;
            this->z = 0;
        }
        coordinate(number x, number y, number z){
            this->x = x;
            this->y = y;
            this->z = z;
        }

        coordinate operator+(const coordinate& vec){
            coordinate result;
            result.x = this->x + vec.x;
            result.y = this->y + vec.y;
            result.z = this->z + vec.z;
            return result;
        }

        coordinate operator-(const coordinate& vec){
            coordinate result;
            result.x = this->x - vec.x;
            result.y = this->y - vec.y;
            result.z = this->z - vec.z;
            return result;
        }

        void operator+=(const coordinate& vec){
            this->x += vec.x;
            this->y += vec.y;
            this->z += vec.z;
        }

        void operator-=(const coordinate& vec){
            this->x -= vec.x;
            this->y -= vec.y;
            this->z -= vec.z;
        }

        coordinate operator*(number c){
            coordinate result;
            result.x = this->x * c;
            result.y = this->y * c;
            result.z = this->z * c;
            return result;
        }

        void operator*=(number c){
            this->x = this->x * c;
            this->y = this->y * c;
            this->z = this->z * c;
        }

        coordinate operator/(number c){
            coordinate result;
            result.x = this->x / c;
            result.y = this->y / c;
            result.z = this->z / c;
            return result;
        }

        void operator/=(number c){
            this->x = this->x/c;
            this->y = this->y/c;
            this->z = this->z/c;
        }

        bool operator==(coordinate p){
            if (this->x!=p.x) return false;
            if (this->y!=p.y) return false;
            if (this->z!=p.z) return false;
            return true;
        }

        bool operator!=(coordinate p){
            if (this->x!=p.x) return true;
            if (this->y!=p.y) return true;
            if (this->z!=p.z) return true;
            return false;
        }

        coordinate operator-(){
            coordinate p = {this->x*-1,this->y*-1,this->z*-1};
            return p;
        }

        string getX(){
            string coord = "X";
            coord.append(to_string(this->x));
            return coord;
        }

        string getY(){
            string coord = "Y";
            coord.append(to_string(this->y));
            return coord;
        }

        string getZ(){
            string coord = "Z";
            coord.append(to_string(this->z));
            return coord;
        }
        
        string getCoordinate(){
            string coord = "X";
            coord.append(to_string(this->x));
            coord.append(" Y");
            coord.append(to_string(this->y));
            coord.append(" Z");
            coord.append(to_string(this->z));
            return coord;
        }
};

number dot(coordinate vec1, coordinate vec2){
    return (vec1.x * vec2.x + vec1.y * vec2.y + vec1.z * vec2.z);
}

number magnitude(coordinate vec){
    return pow(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z,0.5);
}

double angle(coordinate vec1, coordinate vec2){
    return acos(to_double(dot(vec1,vec2)/(magnitude(vec1)*magnitude(vec2))));
}

class toolPath{
    private:
        string name;
        number toolRadius;
        string fileName;
        vector<string> instructions;
        vector<string> end;
        coordinate current;
        coordinate entry;
        number safety;
        number top;
        int coordinateMode; //0 for absolute, 1 for incremental
        int unit; //0 for mm, 1 for inch
        int feedrateMode; //0 for per minute, 1 for per revolution
        int feedrate;
        int plane; //0 for XY, 1 for ZX, 2 for YZ
        number vertSpeed;
        number horiSpeed;
        number currentSpeed;
        int currentMovement; //0 for G0,1 for G1, 2 for G2

    public:
        toolPath(string name,number toolWidth,string fileType,float feedrate,int vSpeed,int hSpeed){
            this->name = name;
            this->toolRadius = toolWidth/2;
            this->fileName = name;
            this->fileName.push_back('.');
            this->fileName.append(fileType);
            this->entry = {0,0,0};
            this->coordinateMode = ABSOLUTE;
            this->unit = MM;
            this->feedrateMode = MINUTE;
            this->feedrate = feedrate;
            this->plane = XY;
            this->safety = 3;
            this->top = 1;
            this->vertSpeed = vSpeed;
            this->horiSpeed = hSpeed;
            this->currentSpeed = 0;
            this->currentMovement = G0;
            this->current = {0,0,this->safety};
        }

        int& getCoordinateMode(){
            return this->coordinateMode;
        }
        int& getUnit(){
            return this->unit;
        }
        int& getFeedrateMode(){
            return this->feedrateMode;
        }
        int& getFeedrate(){
            return this->feedrate;
        }
        int& getPlane(){
            return this->plane;
        }
        number& getSafety(){
            return this->safety;
        }
        number& getTop(){
            return this->top;
        }
        void setEntry(coordinate target){
            this->entry = target;
            this->current = target;
        }
        number& getVertSpeed(){
            return this->vertSpeed;
        }
        number& getHoriSpeed(){
            return this->horiSpeed;
        }
        coordinate getCurrent(){
            return this->current;
        }
        int& getCurrentMovement(){
            return this->currentMovement;
        }

        void output(){
            ofstream file;
            file.open(this->fileName);

            //setup
            file<<"(setup)"<<endl;
            file<<((this->coordinateMode==ABSOLUTE)?"G90 ":"G91 ")<<((this->feedrateMode==MINUTE)?"G94":"G95")<<endl;
            switch (this->plane){
                case XY:
                    file<<"G17"<<endl;
                    break;
                case ZX:
                    file<<"G18"<<endl;
                    break;
                case YZ:
                    file<<"G19"<<endl;
                    break;
            }
            file<<(this->unit==MM?"G21":"G20")<<endl;

            //main operation
            file<<"\n(operation)\nG0 Z"<<to_string(this->safety)<<endl;
            file<<"G0 "<<this->entry.getX()<<" "<<this->entry.getY()<<"\nS"<<to_string(this->feedrate)<<" M3\nG54"<<endl;
            for (auto i:this->instructions){
                file<<i<<endl;
            }

            //end operation
            file<<"\n(end)\nG0 Z"<<to_string(this->safety)<<"\nM5\nM30"<<endl;
            file.close();
            
        }

        void push(string line){
            this->instructions.push_back(line);
        }

        void print(){
            //setup
            cout<<"(setup)"<<endl;
            cout<<((this->coordinateMode==ABSOLUTE)?"G90 ":"G91 ")<<((this->feedrateMode==MINUTE)?"G94":"G95")<<endl;
            switch (this->plane){
                case XY:
                    cout<<"G17"<<endl;
                    break;
                case ZX:
                    cout<<"G18"<<endl;
                    break;
                case YZ:
                    cout<<"G19"<<endl;
                    break;
            }
            cout<<(this->unit==MM?"G21":"G20")<<endl;

            //main operation
            cout<<"\n(operation)\nG0 Z"<<to_string(this->safety)<<endl;
            cout<<"G0 "<<this->entry.getX()<<" "<<this->entry.getY()<<"\nS"<<to_string(this->feedrate)<<" M3\nG54"<<endl;
            for (auto i:this->instructions){
                cout<<i<<endl;
            }

            //end operation
            cout<<"\n(end)\nG0 Z"<<to_string(this->safety)<<"\nM5\nM30"<<endl;
        }
        
        void go(number x, number y){
            string instruction;
            bool valid = 0;
            if (this->currentMovement!=G0){
                instruction.append("G0 ");
                this->currentMovement = G0;
            }
            if (x!=this->current.x){
                instruction.append("X");
                instruction.append(to_string(x));
                instruction.append(" ");
                valid = 1;
            }
            if (y!=this->current.y){
                instruction.append("Y");
                instruction.append(to_string(y));
                instruction.append(" ");
                valid = 1;
            }
            if (valid){
                this->instructions.push_back(instruction);
                this->current.x = x;
                this->current.y = y;
            }
        }

        void line(coordinate relativeTarget,number speed = -1){
            coordinate temp = this->current + relativeTarget;
            string instruction;
            bool valid = 0;
            if (this->currentMovement!=G1){
                instruction.append("G1 ");
                this->currentMovement = G1;
            }
            if (speed==-1) speed = this->horiSpeed;
            if (temp.x!=this->current.x){
                instruction.append(temp.getX());
                instruction.append(" ");
                valid = 1;
            }
            if (temp.y!=this->current.y){
                instruction.append(temp.getY());
                instruction.append(" ");
                valid = 1;
            }
            if (temp.z!=this->current.z){
                instruction.append(temp.getZ());
                valid = 1;
            }
            if ((speed!=this->currentSpeed)&&valid==1){
                instruction.append(" ");
                instruction.append("F");
                instruction.append(to_string(speed));
                this->currentSpeed = speed;
            }
            if (valid){
                this->instructions.push_back(instruction);
                this->current += relativeTarget;
            }
        }

        void lift(){
            string instruction = "G0 Z";
            this->currentMovement = G0;
            instruction.append(to_string(this->safety));
            this->instructions.push_back(instruction);
            this->current.z = this->safety;
        }

        void drill(number depth){
            string instruction = "";
            if (this->currentMovement!=G1){
                instruction.append("G1 ");
                this->currentMovement = G1;
            }
            instruction.append("Z");
            instruction.append(to_string(depth));
            if (this->vertSpeed!=this->currentSpeed){
                instruction.append(" ");
                instruction.append("F");
                instruction.append(to_string(this->vertSpeed));
                this->currentSpeed = this->vertSpeed;
            }
            this->instructions.push_back(instruction);
            this->current.z = depth;
        }

        void circularArc(coordinate relativeCenter, coordinate relativeEnd = {0,0,0},int direction = 1, number speed = -1){//0 for clockwise, 1 for counterclockwise
            string instruction;
            this->currentMovement = G2;
            coordinate end = this->current + relativeEnd;
            if (speed==-1) speed = this->horiSpeed;
            if (direction==1){
                instruction.append("G3 ");
                instruction.append(end.getX());
                instruction.append(" ");
                instruction.append(end.getY());
                instruction.append(" ");
                instruction.append("I");
                instruction.append(to_string(relativeCenter.x));
                instruction.append(" J");
                instruction.append(to_string(relativeCenter.y));
            }
            else{
                instruction.append("G2 ");
                instruction.append(end.getX());
                instruction.append(" ");
                instruction.append(end.getY());
                instruction.append(" ");
                instruction.append("I");
                instruction.append(to_string(relativeCenter.x));
                instruction.append(" J");
                instruction.append(to_string(relativeCenter.y));
            }
            if ((speed!=this->currentSpeed)){
                instruction.append(" ");
                instruction.append("F");
                instruction.append(to_string(speed));
                this->currentSpeed = speed;
            }
            this->instructions.push_back(instruction);
            this->current += relativeEnd;
        }

        void rectangle(number x, number y, double angle, number speed = -1){
            number x1 = x * cos(angle);
            number y1 = x * sin(angle);
            number x2 = y * sin(angle) * -1;
            number y2 = y * cos(angle);
            this->line({x1,y1,0});
            this->line({x2,y2,0});
            this->line({-x1,-y1,0});
            this->line({-x2,-y2,0});
        }

        void slot(number x1, number y1, number xWidth, number yWidth, number depth, double angle, number gap, number speed = -1){
            coordinate toolOffset = {this->toolRadius*sqrt(2)*cos(angle+pi/4),this->toolRadius*sqrt(2)*sin(angle+pi/4),0};
            coordinate originOffset = {toolOffset.x+x1,toolOffset.y+y1,depth};
            coordinate gapOffset = {gap*sqrt(2)*cos(angle+pi/4),gap*sqrt(2)*sin(angle+pi/4),0};
            xWidth -= 2 * this->toolRadius;
            yWidth -= 2 * this->toolRadius;
            while (xWidth>=0&&yWidth>=0){
                this->go(originOffset.x,originOffset.y);
                this->drill(depth);
                this->rectangle(xWidth,yWidth,angle);
                this->drill(this->top);
                originOffset += gapOffset;
                xWidth -= 2 * gap;
                yWidth -= 2 * gap;
            }
            
        }

        void disk(coordinate relativeCenter, number depth, number gap, number speed = -1){
            number x = relativeCenter.x;
            number y = relativeCenter.y;
            coordinate toolOffset = relativeCenter * this->toolRadius / magnitude(relativeCenter);
            coordinate gapOffset = relativeCenter * gap / magnitude(relativeCenter);
            relativeCenter -= toolOffset;
            this->go(this->current.x+toolOffset.x,this->current.y+toolOffset.y);
            this->drill(depth);
            this->circularArc(relativeCenter);
            this->drill(this->top);
            relativeCenter -= gapOffset;
            while (relativeCenter.y*y>=0&&relativeCenter.x*x>=0){
                this->go(this->current.x+gapOffset.x,this->current.y+gapOffset.y);
                this->drill(depth);
                this->circularArc(relativeCenter);
                this->drill(this->top);
                relativeCenter -= gapOffset;
            }
        }

};

int main(){
    toolPath test("test",6,"nc",8000,200,800);
    test.go(0,0);
    test.slot(0,0,30,45,0,0,1);
    test.go(0,0);
    test.disk({20,0,0},-2,2);
    test.disk({0,103,0},-3,2.5);
    test.disk({-50,-75,0},-5,1);
    test.output();
    return 0;
}