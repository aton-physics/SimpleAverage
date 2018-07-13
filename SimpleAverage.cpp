#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <numeric>
#include <algorithm> //transform
#include <functional> //bind, placeholders, multiplies

//simple averaging program. 
//INPUT: files named "C_t/C_t".variable1->50.".data". NumFilesToAverage is the number of files with identical parameters. Data has header for the first line and data for all subsequent lines. Data should be coordinate pairs, f(x) and x.
//OUTPUT: for given x, output is the header line followed by coordinate pairs sum(f(x))/#files and sum(x)/#files. #output files should be reduced by a factor of NumFilesToAverage


// Class to represent points.
class Point {
private:
	double xval, yval;
public:
	// Constructor uses default arguments to allow calling with zero, one,
	// or two values.
	Point(double x = 0.0, double y = 0.0) {
		xval = x;
		yval = y;
	}

	// Extractors.
	double x() { return xval; }
	double y() { return yval; }

	// Add or subtract two points.
	Point operator+(Point b) {
		return Point(xval + b.xval, yval + b.yval);
	}
	Point operator-(Point b) {
		return Point(xval - b.xval, yval - b.yval);
	}
};

int main() {
	int NumFilesToAverage = 100;	//How many curves to average
	int NumVariables = 25;	//How many different "types" of curves, i.e. different sets of simulation parameters
	std::vector<std::string> InputFilename = { "C_t/C_t", "rdf/rdf", "RotOrder/RotOrder" };
	std::vector<std::string> OutputFilename = { "C_t/Average", "rdf/Average", "RotOrder/Average" };
	std::cout << "is this thing on?" << '\n';
	for (int a = 0; a < InputFilename.size(); a++) {
		for (int n = 0; n < NumVariables; n++) {
			std::vector<Point> XY;
			std::string dummyline;	//go into loop, grab the header.
			for (int i = 0; i < NumFilesToAverage; i++) {
				std::ifstream CStream(InputFilename[a] + std::to_string(i + NumFilesToAverage * n + 1) + ".data");
				std::getline(CStream, dummyline);	//columns have headers. throw away header before recording values
				double a = 0, b = 0, c = 0, d = 0;
				if (i == 0) {	//if first file, use that file to determine the proper length of the vector
					while (CStream >> a >> b) {
						Point mypoint = { a, b };
						XY.push_back(mypoint);
					}
				}
				else {
					int counter = 0;
					while (CStream >> a >> b) {
						Point mypoint = { a,b };
						XY[counter] = XY[counter] + mypoint;
						counter += 1;
					}
				}
			}
			std::ofstream mystream(OutputFilename[a] + std::to_string(n + 1) + ".data");
			mystream << dummyline << '\n'; //give the column headers back
			for (auto& i : XY) {
				mystream << i.x() / NumFilesToAverage << '\t' << i.y() / NumFilesToAverage << '\n';
			}
			std::vector<Point>().swap(XY);	//swap with temporary empty vector to delete, deallocate
		}
	}
	std::cout << "testing testing " << '\n';
}