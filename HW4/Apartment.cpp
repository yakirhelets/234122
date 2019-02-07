#include "Apartment.h"
#include <stdio.h>
// -------------------------------------------------------------------------- //
// -------------------------------AUX FUNCTIONS------------------------------ //
// -------------------------------------------------------------------------- //
Apartment::SquareType** Apartment::allocateSquares(const int length, const int width) {
    Apartment::SquareType** squares =  new Apartment::SquareType*[length];
    for (int i=0 ; i<length ; ++i) {
        squares[i] = new Apartment::SquareType[width];
    }
    return squares;
}

void Apartment::handleApartmentArgs(Apartment::SquareType** squares, const int length,
                                    const int width, const int price){
    if (!squares) {
        throw Apartment::IllegalArgException();
    }
    if (length <= 0) {
        throw Apartment::IllegalArgException();
    }
    if (width <= 0) {
        throw Apartment::IllegalArgException();
    }
    if (price < 0) {
        throw Apartment::IllegalArgException();
    }
    for (int i=0 ; i<length ; ++i) {
        for (int j=0; j<width ; ++j) {
            if (squares[i][j]!=Apartment::WALL && squares[i][j]!=Apartment::EMPTY) {
                throw Apartment::IllegalArgException();
            }
        }
    }
}

void Apartment::expandByLength(Apartment::SquareType** squares, const int length,
                               const int width, const Apartment& a, const Apartment& b) {
    for (int i=0; i < length ; ++i) {
        for (int j=0; j < width ; ++j) {
            if (i<a.getLength() && j<a.getWidth()) {
                squares[i][j] = a(i, j);
            }
            if (i>=a.getLength() && j<b.getWidth()) {
                int row = i-a.getLength();
                squares[i][j] = b(row , j);
            }
        }
    }
}

void Apartment::expandByWidth(Apartment::SquareType** squares, const int length,
                              const int width, const Apartment& a, const Apartment& b) {
    for (int i=0; i < length ; ++i) {
        for (int j=0; j < width ; ++j) {
            squares[i][j] = ( j<a.getWidth() ? a(i,j) : b(i, j-a.getWidth()) );
        }
    }
}
// -------------------------------------------------------------------------- //
// --------------------------------CONSTRUCTORS------------------------------ //
// -------------------------------------------------------------------------- //


Apartment::Apartment (SquareType** squares, int length, int width, int price){

    handleApartmentArgs(squares, length, width, price);

    this->length = length;
    this->width = width;
    this->price = price;

    Apartment::SquareType** copied_squares = allocateSquares(length, width);

    for (int i=0 ; i<length ; ++i) {
        for (int j=0; j<width ; ++j) {
            copied_squares[i][j] = squares[i][j];
        }
    }
    this->squares = copied_squares;
}

Apartment::Apartment (const Apartment& a) {
    this->length = a.getLength();
    this->width = a.getWidth();
    this->price = a.getPrice();
    Apartment::SquareType** squares = new Apartment::SquareType*[length];
    for (int i=0 ; i<length ; ++i) {
        squares[i] = new SquareType[width];
    }
    for (int i=0 ; i < length ; ++i) {
        for (int j=0; j < width ; ++j) {
            squares[i][j] = a(i,j);
        }
    }
    this->squares = squares;
}

Apartment::~Apartment () {
    for (int i=0 ; i<length ; ++i) {
        delete[] squares[i];
    }
    delete[] squares;
}

// -------------------------------------------------------------------------- //
// -----------------------------GETTERS/SETTERS------------------------------ //
// -------------------------------------------------------------------------- //

int Apartment::getTotalArea() const {
    int total_area = 0;
    for (int i=0 ;i<length; ++i) {
        for (int j=0; j<width; ++j) {
            if (squares[i][j] == Apartment::EMPTY) {
                total_area++;
            }
        }
    }
    return total_area;
}

int Apartment::getPrice() const {
    return price;
}

int Apartment::getLength() const {
    return length;
}

int Apartment::getWidth() const {
    return width;
}

// -------------------------------------------------------------------------- //
// --------------------------------OPERATORS--------------------------------- //
// -------------------------------------------------------------------------- //


// -----------------------        =          ---------------------------------//

Apartment& Apartment::operator=(const Apartment& apartment) {
    int new_length = apartment.getLength();
    int new_width = apartment.getWidth();
    if (this == &apartment) { // Self check
        return *this;
    }

    for (int i=0 ; i<length ; ++i) { // Cleans old data
        delete[] squares[i];
    }
    delete[] squares;
    // Reallocation
    Apartment::SquareType** squares = new Apartment::SquareType*[new_length];
    for (int i=0 ; i < new_length ; ++i) {
        squares[i] = new SquareType[new_width];
    }
    // Hard copy of data
    length = new_length;
    width = new_width;
    price = apartment.getPrice();

    for (int i=0 ; i < length ; ++i) {
        for (int j=0; j < width ; ++j) {
            squares[i][j] = apartment(i,j);
        }
    }
    this->squares = squares;
    return *this;
}

// ----------------------          +=         --------------------------------//

Apartment& Apartment::operator+=(const Apartment& apartment) {
    int new_length = length;
    int new_width = width;
    int new_price = price + apartment.getPrice();
    if ( width == apartment.getWidth() ) {
        new_length += apartment.getLength();
    } else if ( length == apartment.getLength() ) {
        new_width += apartment.getWidth();
    } else {
        new_length += apartment.getLength();
        new_width = (width>apartment.getWidth()?width:apartment.getWidth());
    }
    SquareType** new_squares = allocateSquares(new_length, new_width);
    for (int i=0; i < new_length ; ++i) {
        for (int j=0; j < new_width ; ++j) {
            new_squares[i][j] = WALL;
        }
    }
    if ( width == apartment.getWidth() ) {
        expandByLength(new_squares, new_length, new_width, *this, apartment);
    } else if ( length == apartment.getLength() ) {
        expandByWidth(new_squares, new_length, new_width, *this, apartment);
    } else {
        expandByLength(new_squares, new_length, new_width, *this, apartment);
    }

    this->squares = new_squares;
    this->length = new_length;
    this->width = new_width;
    this->price = new_price;

    return *this;
}

// ---------------------------------- () -------------------------------------//

const Apartment::SquareType& Apartment::operator()(const int row, const int col) const {
    if (row < 0 || col < 0 || row >= length || col >= width) {
        throw Apartment::OutOfApartmentBoundsException();
    }
    return squares[row][col];
}

Apartment::SquareType& Apartment::operator()(const int row, const int col) {
    if (row < 0 || col < 0 || row >= length || col >= width) {
        throw Apartment::OutOfApartmentBoundsException();
    }
    return squares[row][col];
}

// ---------------------------------- < --------------------------------------//

bool operator<(const Apartment& l_apartment,
               const Apartment& r_apartment) {
    int apt1_price = l_apartment.getPrice();
    int apt1_area = l_apartment.getTotalArea();
    int apt2_price = r_apartment.getPrice();
    int apt2_area = r_apartment.getTotalArea();

    return (apt1_price * apt2_area == apt2_price * apt1_area) ?
           apt1_price<apt2_price : apt1_price * apt2_area < apt2_price * apt1_area;
}

// --------------------------       +           ------------------------------//

Apartment operator+(const Apartment& l_apartment,
                    const Apartment& r_apartment) {
    return Apartment(l_apartment) += r_apartment;
}

//}
