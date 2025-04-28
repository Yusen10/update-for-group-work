#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <iomanip>
#include "lineType.h"
using namespace std;

// Struct to store a 2D point
struct Point {
    double x, y;
};

// Compute distance between two points
double distance(const Point& p1, const Point& p2) {
    return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
}

/*Add this function to check the Angle between the two sides of the corner*/
bool isRightAngle(const Point& A, const Point& B, const Point& C) {
    double vx1 = B.x - A.x;
    double vy1 = B.y - A.y;
    double vx2 = C.x - B.x;
    double vy2 = C.y - B.y;

    double dot = vx1 * vx2 + vy1 * vy2;
    return fabs(dot) < 1e-6;
}


// Classify the shape based on lines and points
void classifyQuadrilateral(const vector<lineType>& lines) {

    /*Display the lines*/
    cout << "Lines forming the quadrilateral:" << endl;
    for (int i = 0; i < 4; i++) {
        cout << "Line " << i + 1 << ": ";
        lines[i].display();
    }

    double x1, y1, x2, y2, x3, y3, x4, y4;
   /*According to the setting of the Part 2, the first two lines are opposite sides, and the last two lines are also opposite sides.*/
    /*So I modified this part*/
    bool inter1 = lines[0].getIntersection(lines[2], x1, y1);
    bool inter2 = lines[2].getIntersection(lines[1], x2, y2);
    bool inter3 = lines[1].getIntersection(lines[3], x3, y3);
    bool inter4 = lines[3].getIntersection(lines[0], x4, y4);

    if (!(inter1 && inter2 && inter3 && inter4)) {
        cout << "Result: Invalid quadrilateral (lines don't form a closed shape)" << endl;
        return;
    }

    Point p1{ x1, y1 }, p2{ x2, y2 }, p3{ x3, y3 }, p4{ x4, y4 };

    // Sides
    double s1 = distance(p1, p2);
    double s2 = distance(p2, p3);
    double s3 = distance(p3, p4);
    double s4 = distance(p4, p1);

    // Parallelism
    bool opp1 = lines[0].isParallel(lines[1]);
    bool opp2 = lines[2].isParallel(lines[3]);

    /*Perpendicular(for rectangle)*/
    bool right1 = isRightAngle(p4, p1, p2);
    bool right2 = isRightAngle(p1, p2, p3);
    bool right3 = isRightAngle(p2, p3, p4);
    bool right4 = isRightAngle(p3, p4, p1);

    bool allPerpendicular = right1 && right2 && right3 && right4;

    // Equal sides?
    bool allSidesEqual = fabs(s1 - s2) < 1e-6 && fabs(s1 - s3) < 1e-6 && fabs(s1 - s4) < 1e-6;



    /*For details*/
    cout << "Properties:" << endl;
    cout << "- Opposite sides parallel: " << (opp1 ? "Pair1 " : "") << (opp2 ? "Pair2" : "") << (opp1 || opp2 ? "" : "None") << endl;
    cout << "- Sides equal: " << (allSidesEqual ? "Yes" : "No") << endl;
    cout << "- Angles 90 degrees: " << (allPerpendicular ? "Yes" : "No") << endl;


    // Classification logic
    if (opp1 && opp2) {
        if (allSidesEqual && allPerpendicular)
            cout << "Square" << endl;
        else if (allPerpendicular)
            cout << "Rectangle" << endl;
        else if (allSidesEqual)
            cout << "Rhombus" << endl;
        else
            cout << "Parallelogram" << endl;
    }
    else if (opp1 || opp2) {
        cout << "Trapezoid" << endl;
    }
    else {
        cout << "Irregular quadrilateral" << endl;
    }
}

int main() {
    ifstream infile("lines.txt");  // your data file
    if (!infile) {
        cerr << "File could not be opened." << endl;
        return 1;
    }

    vector<lineType> lines;
    double a, b, c;
    int setCount = 0;

    cout << "Quadrilateral Classification Results:\n" << endl;

    while (infile >> a >> b >> c) {
        lines.emplace_back(a, b, c);
        if (lines.size() == 4) {
            /*Make the output a little more concise*/
            setCount++;
            cout << "===== Set " << setCount << " =====" << endl;
            classifyQuadrilateral(lines);
            lines.clear();  // reset for next group
            cout << endl;
        }
    }

    /*Just for warning*/
    if (lines.size() > 0 && lines.size() != 4) {
        cout << "Warning: Incomplete set of lines (needs 4 lines per quadrilateral)" << endl;
    }

    infile.close();
    return 0;
}

