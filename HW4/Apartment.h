#ifndef MTM4_APARTMENT_H
#define MTM4_APARTMENT_H

#include <exception>

//namespace Apartment {

class Apartment {
public:
	class ApartmentException: public std::exception {};
	class IllegalArgException: public ApartmentException {};
	class OutOfApartmentBoundsException: public ApartmentException {};

	enum SquareType {EMPTY, WALL, NUM_SQUARE_TYPES};


	Apartment(SquareType** squares, int length, int width, int price); // C'tor
	Apartment(const Apartment& apartment); // Copy C'tor
	~Apartment(); // D'tor

	int getTotalArea() const;
	int getPrice() const;
	int getLength() const;
	int getWidth() const;


	Apartment& operator=(const Apartment& apartment); // Assignment Operator
	Apartment& operator+=(const Apartment& apartment); // += Operator
	SquareType& operator()(int row, int col); // () Operator
	const SquareType& operator()(int row, int col) const; // () Operator

//	friend bool operator<(const Apartment& l_apartment, const Apartment& r_apartment);
//	friend Apartment& operator+(const Apartment& l_apartment,const Apartment& r_apartment);

private:
	SquareType** squares;
	int length;
	int width;
	int price;

	SquareType** allocateSquares(int other_length, int other_width);
	void handleApartmentArgs(SquareType** squares, const int length,
							 const int width, const int price);
	void expandByLength(SquareType** squares, const int length, const int width,
						const Apartment& a, const Apartment& b);
	void expandByWidth(SquareType** squares, const int length, const int width,
					   const Apartment& a, const Apartment& b);

};

/*****************************
 * External operator function
 ****************************/
bool operator<(const Apartment& l_apartment, const Apartment& r_apartment);
Apartment operator+(const Apartment& l_apartment,const Apartment& r_apartment);
//}
#endif //MTM4_APARTMENT_H
