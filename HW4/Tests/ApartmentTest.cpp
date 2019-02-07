#include <cstdlib>
#include "Apartment.h"
#include "MtmTst.h"
#include <iostream>
using namespace std;


static void deleteSquares(Apartment::SquareType** squares, int length) {
    for (int i = 0; i < length; ++i) {
        delete[] squares[i];
    }
    delete[] squares;
}

 bool apartmentTestExample() {
     const int length = 3;
     const int width = 4;
     const int price = 1;
     Apartment::SquareType** squares = new Apartment::SquareType*[length];
     for (int i = 0; i < length; ++i) {
         squares[i] = new Apartment::SquareType[width];
             for (int j = 0; j < width; ++j) {
                 squares[i][j] = (i%2==0)?Apartment::EMPTY:Apartment::WALL;
         }
     }
     ASSERT_NO_THROW(Apartment(squares, length, width, price));
     const Apartment apt(squares, length, width, price);
     ASSERT_EQUALS(length, apt.getLength());
     Apartment apt2(apt);
     ASSERT_NO_THROW(apt2 += apt);
     ASSERT_EQUALS(true, apt < apt2);
     deleteSquares(squares, length);
     return true;
 }

bool constructorTest1() {
    const int length = 15;
    const int width = 15;
    const int price = 1000;
    Apartment::SquareType** squares = new Apartment::SquareType*[length];
    for (int i = 0; i < length; ++i) {
        squares[i] = new Apartment::SquareType[width];
        for (int j = 0; j < width; ++j) {
                squares[i][j] = (i%3==0)?Apartment::EMPTY:Apartment::WALL;
        }
    }
    ASSERT_NO_THROW(Apartment(squares, length, width, price));
    deleteSquares(squares, length);
    return true;
}

bool constructorTest2() {
    const int length = 0;
    const int width = 0;
    const int price = 0;
    Apartment::SquareType** squares = new Apartment::SquareType*[length];
    for (int i = 0; i < length; ++i) {
        squares[i] = new Apartment::SquareType[width];
        for (int j = 0; j < width; ++j) {
                squares[i][j] = (i%3==0)?Apartment::EMPTY:Apartment::WALL;
        }
    }
    // ------- Exceptions ----------------------------------------------------//
    ASSERT_THROW(Apartment::IllegalArgException,Apartment(squares, length, width, price));
    ASSERT_THROW(Apartment::IllegalArgException,Apartment(squares, length-5, width, price));
    ASSERT_THROW(Apartment::IllegalArgException,Apartment(squares, length, width-5, price));
    ASSERT_THROW(Apartment::IllegalArgException,Apartment(squares, length, width, price-10000));
    ASSERT_THROW(Apartment::IllegalArgException,Apartment(squares, length-5, width+3, price));
    // ------- End of exceptions ---------------------------------------------//
    deleteSquares(squares, length);
    return true;
}

bool getPriceTest() {
    const int length = 4;
    const int width = 4;
    const int price = 1234;
    Apartment::SquareType** squares = new Apartment::SquareType*[length];
    for (int i = 0; i < length; ++i) {
        squares[i] = new Apartment::SquareType[width];
        for (int j = 0; j < width; ++j) {
                squares[i][j] = (i%3==0)?Apartment::EMPTY:Apartment::WALL;
        }
    }
    ASSERT_NO_THROW(Apartment(squares, length, width, price));
    const Apartment apt1(squares,length,width,price);
    ASSERT_EQUALS(price, apt1.getPrice());
    Apartment apt2(apt1);
    ASSERT_EQUALS(price, apt2.getPrice());
    ASSERT_THROW(Apartment::IllegalArgException,Apartment(squares,length,width,-999)); // Negative Price [Invalid]
    ASSERT_NO_THROW(Apartment(squares,length,width,0)); // Zero Price [Valid]
    deleteSquares(squares, length);
    return true;
}

bool getTotalAreaTest() {
    const int length = 10;
    const int width = 10;
    const int price = 5000;
    Apartment::SquareType** squares = new Apartment::SquareType*[length];
    for (int i = 0; i < length; ++i) {
        squares[i] = new Apartment::SquareType[width];
        for (int j = 0; j < width; ++j) {
                squares[i][j] = (i%2==0)?Apartment::EMPTY:Apartment::WALL;
        }
    }
    ASSERT_NO_THROW(Apartment(squares, length, width, price));
    const Apartment apt1(squares,length,width,price);
    ASSERT_EQUALS(50, apt1.getTotalArea());
    deleteSquares(squares, length);
    return true;
}

bool getLengthTest() {
    const int length = 8;
    const int width = 8;
    const int price = 200;
    Apartment::SquareType** squares = new Apartment::SquareType*[length];
    for (int i = 0; i < length; ++i) {
        squares[i] = new Apartment::SquareType[width];
        for (int j = 0; j < width; ++j) {
                squares[i][j] = (i%4==0)?Apartment::EMPTY:Apartment::WALL;
        }
    }
    ASSERT_NO_THROW(Apartment(squares, length, width, price));
    const Apartment apt1(squares,length,width,price);
    ASSERT_EQUALS(16, apt1.getTotalArea());
    deleteSquares(squares, length);
    return true;
}

bool getWidthTest() {
    const int length = 18;
    const int width = 18;
    const int price = 2300;
    Apartment::SquareType** squares = new Apartment::SquareType*[length];
    for (int i = 0; i < length; ++i) {
        squares[i] = new Apartment::SquareType[width];
        for (int j = 0; j < width; ++j) {
                squares[i][j] = (i%5==0)?Apartment::EMPTY:Apartment::WALL;
        }
    }
    ASSERT_NO_THROW(Apartment(squares, length, width, price));
    const Apartment apt1(squares,length,width,price);
    ASSERT_EQUALS(18*4, apt1.getTotalArea());
    deleteSquares(squares, length);
    return true;
}

bool plusEqualOperatorTest1() {
    // ----- Apartment 1 -----------------------------------------------------//
    const int length1 = 5;
    const int width1 = 5;
    const int price1 = 12000;
    Apartment::SquareType** squares1 = new Apartment::SquareType*[length1];
    for (int i = 0; i < length1; ++i) {
        squares1[i] = new Apartment::SquareType[width1];
        for (int j = 0; j < width1; ++j) {
               squares1[i][j] = ((i%2==0)&&(j%2==0))?Apartment::EMPTY:Apartment::WALL;
        }
    }
    Apartment apt1(squares1,length1,width1,price1);

    // ----- Apartment 2 -----------------------------------------------------//
    const int length2 = 2;
    const int width2 = 5;
    const int price2 = 13000;
    Apartment::SquareType** squares2 = new Apartment::SquareType*[length2];
    for (int i = 0; i < length2; ++i) {
        squares2[i] = new Apartment::SquareType[width2];
        for (int j = 0; j < width2; ++j) {
                squares2[i][j] = ((i%2==0)&&(j%2==0))?Apartment::EMPTY:Apartment::WALL;
        }
    }
    const Apartment apt2(squares2,length2,width2,price2);

    // ------- Operator usage ------------------------------------------------//
    ASSERT_NO_THROW(apt1 += apt2);
  //  ASSERT_EQUALS(length1+length2, apt1.getLength());
  //  ASSERT_EQUALS(width1, apt1.getWidth());
  //  ASSERT_EQUALS(price1+price2, apt1.getPrice());

    // -------- Squares destroying -------------------------------------------//
    deleteSquares(squares1, length1);
    deleteSquares(squares2, length2);
    return true;
}

bool plusEqualOperatorTest2() {
    // ----- Apartment 1 -----------------------------------------------------//
    const int length1 = 15;
    const int width1 = 9;
    const int price1 = 12000;
    Apartment::SquareType** squares1 = new Apartment::SquareType*[length1];
    for (int i = 0; i < length1; ++i) {
        squares1[i] = new Apartment::SquareType[width1];
        for (int j = 0; j < width1; ++j) {
                squares1[i][j] = ((i%3==0)&&(j%2==0))?Apartment::EMPTY:Apartment::WALL;
        }
    }
    Apartment apt1(squares1,length1,width1,price1);

    // ----- Apartment 2 -----------------------------------------------------//
    const int length2 = 15;
    const int width2 = 11;
    const int price2 = 13000;
    Apartment::SquareType** squares2 = new Apartment::SquareType*[length2];
    for (int i = 0; i < length2; ++i) {
        squares2[i] = new Apartment::SquareType[width2];
        for (int j = 0; j < width2; ++j) {
                squares2[i][j] = ((i%2==0)&&(j%4==0))?Apartment::EMPTY:Apartment::WALL;
        }
    }
    const Apartment apt2(squares2,length2,width2,price2);

    // ------- Operator usage ------------------------------------------------//
    ASSERT_NO_THROW(apt1 += apt2);
    ASSERT_EQUALS(length1, apt1.getLength());
    ASSERT_EQUALS(width1+width2, apt1.getWidth());
    ASSERT_EQUALS(price1+price2, apt1.getPrice());

    // -------- Squares destroying -------------------------------------------//
    deleteSquares(squares1, length1);
    deleteSquares(squares2, length2);
    return true;
}

bool plusEqualOperatorTest3() {
    // ----- Apartment 1 -----------------------------------------------------//
    const int length1 = 3;
    const int width1 = 4;
    const int price1 = 100;
    Apartment::SquareType** squares1 = new Apartment::SquareType*[length1];
    for (int i = 0; i < length1; ++i) {
        squares1[i] = new Apartment::SquareType[width1];
        for (int j = 0; j < width1; ++j) {
                squares1[i][j] = ((i%2==0)&&(j%3==0))?Apartment::EMPTY:Apartment::WALL;
        }
    }
    Apartment apt1(squares1,length1,width1,price1);

    // ----- Apartment 2 -----------------------------------------------------//
    const int length2 = 5;
    const int width2 = 5;
    const int price2 = 200;
    Apartment::SquareType** squares2 = new Apartment::SquareType*[length2];
    for (int i = 0; i < length2; ++i) {
        squares2[i] = new Apartment::SquareType[width2];
        for (int j = 0; j < width2; ++j) {
                squares2[i][j] = ((i%2==0)&&(j%2==0))?Apartment::EMPTY:Apartment::WALL;
        }
    }
    const Apartment apt2 (squares2,length2,width2,price2);

    // ------- Operator usage ------------------------------------------------//
    ASSERT_NO_THROW(apt1 += apt2);
    ASSERT_EQUALS(width2, apt1.getWidth());
    ASSERT_EQUALS(length1+length2, apt1.getLength());
    ASSERT_EQUALS(price1+price2, apt1.getPrice());

    // -------- Squares destroying -------------------------------------------//
    deleteSquares(squares1, length1);
    deleteSquares(squares2, length2);
    return true;
}

bool assignmentOperatorTest() {

    // ----- Apartment 1 -----------------------------------------------------//
    const int length1 = 8;
    const int width1 = 8;
    const int price1 = 300;
    Apartment::SquareType** squares1 = new Apartment::SquareType*[length1];
    for (int i = 0; i < length1; ++i) {
        squares1[i] = new Apartment::SquareType[width1];
        for (int j = 0; j < width1; ++j) {
            squares1[i][j] = ((i%2==0)&&(j%3==0))?Apartment::EMPTY:Apartment::WALL;
        }
    }
    Apartment apt1(squares1,length1,width1,price1);

    // ----- Apartment 2 -----------------------------------------------------//
    const int length2 = 5;
    const int width2 = 5;
    const int price2 = 200;
    Apartment::SquareType** squares2 = new Apartment::SquareType*[length2];
    for (int i = 0; i < length2; ++i) {
        squares2[i] = new Apartment::SquareType[width2];
        for (int j = 0; j < width2; ++j) {
                squares2[i][j] = ((i%2==0)&&(j%2==0))?Apartment::EMPTY:Apartment::WALL;
        }
    }
    Apartment apt2(squares2,length2,width2,price2);

    // ------- Operator usage ------------------------------------------------//
    apt1 = apt2;
    ASSERT_EQUALS(apt1.getLength(),length2);
    ASSERT_EQUALS(apt1.getWidth(),width2);
    ASSERT_EQUALS(apt1.getPrice(),price2);
    ASSERT_EQUALS(apt1.getTotalArea(),apt2.getTotalArea());
    // -------- Squares destroying -------------------------------------------//
    deleteSquares(squares1, length1);
    deleteSquares(squares2, length2);
    return true;
}

bool roundBracketsOperatorTest() {
    // ----- Apartment 1 -----------------------------------------------------//
    const int length1 = 3;
    const int width1 = 2;
    const int price1 = 200;
    Apartment::SquareType** squares1 = new Apartment::SquareType*[length1];
    for (int i = 0; i < length1; ++i) {
        squares1[i] = new Apartment::SquareType[width1];
    }
    squares1[0][0] = Apartment::WALL;
    squares1[0][1] = Apartment::EMPTY;
    squares1[1][0] = Apartment::WALL;
    squares1[1][1] = Apartment::EMPTY;
    squares1[2][0] = Apartment::WALL;
    squares1[2][1] = Apartment::EMPTY;

    Apartment apt1(squares1,length1,width1,price1);

    // ------- Operator usage ------------------------------------------------//
    ASSERT_NO_THROW(apt1(0,0) );
    ASSERT_NO_THROW(apt1(0,1) );
    ASSERT_EQUALS(Apartment::WALL, apt1(0,0) );
    ASSERT_EQUALS(Apartment::EMPTY, apt1(2,1) );
    ASSERT_THROW(Apartment::OutOfApartmentBoundsException, apt1(4,4));
    ASSERT_THROW(Apartment::OutOfApartmentBoundsException, apt1(1,9));
    ASSERT_THROW(Apartment::OutOfApartmentBoundsException, apt1(-4,4));
    ASSERT_THROW(Apartment::OutOfApartmentBoundsException, apt1(4,-4));
    ASSERT_THROW(Apartment::OutOfApartmentBoundsException, apt1(0,400));

    // -------- Squares destroying -------------------------------------------//
    deleteSquares(squares1, length1);
    return true;
}

bool smallerThanOperatorTest1() {
    // ----- Apartment 1 -----------------------------------------------------//
    const int length1 = 4;
    const int width1 = 4;
    const int price1 = 300;
    Apartment::SquareType** squares1 = new Apartment::SquareType*[length1];
    for (int i = 0; i < length1; ++i) {
        squares1[i] = new Apartment::SquareType[width1];
        for (int j = 0; j < width1; ++j) {
            squares1[i][j] = ((i%2==0)&&(j%3==0))?Apartment::EMPTY:Apartment::WALL;
        }
    }
    Apartment apt1(squares1,length1,width1,price1);

    // ----- Apartment 2 -----------------------------------------------------//
    const int length2 = 4;
    const int width2 = 4;
    const int price2 = 700;
    Apartment::SquareType** squares2 = new Apartment::SquareType*[length2];
    for (int i = 0; i < length2; ++i) {
        squares2[i] = new Apartment::SquareType[width2];
        for (int j = 0; j < width2; ++j) {
            squares2[i][j] = ((i%2==0)&&(j%3==0))?Apartment::EMPTY:Apartment::WALL;
        }
    }
    Apartment apt2(squares2,length2,width2,price2);

    // ------- Operator usage ------------------------------------------------//
    ASSERT_EQUALS(apt1<apt2, true );

    // -------- Squares destroying -------------------------------------------//
    deleteSquares(squares1, length1);
    deleteSquares(squares2, length2);
    return true;
}

bool smallerThanOperatorTest2() {
    // ----- Apartment 1 -----------------------------------------------------//
    const int length1 = 12;
    const int width1 = 6;
    const int price1 = 300000;
    Apartment::SquareType** squares1 = new Apartment::SquareType*[length1];
    for (int i = 0; i < length1; ++i) {
        squares1[i] = new Apartment::SquareType[width1];
        for (int j = 0; j < width1; ++j) {
            squares1[i][j] = ((i%2==0)&&(j%3==0))?Apartment::EMPTY:Apartment::WALL;
        }
    }
    Apartment apt1(squares1,length1,width1,price1);

    // ----- Apartment 2 -----------------------------------------------------//
    const int length2 = 4;
    const int width2 = 4;
    const int price2 = 700;
    Apartment::SquareType** squares2 = new Apartment::SquareType*[length2];
    for (int i = 0; i < length2; ++i) {
        squares2[i] = new Apartment::SquareType[width2];
        for (int j = 0; j < width2; ++j) {
            squares2[i][j] = ((i%2==0)&&(j%3==0))?Apartment::EMPTY:Apartment::WALL;
        }
    }
    Apartment apt2(squares2,length2,width2,price2);

    // ------- Operator usage ------------------------------------------------//
    ASSERT_EQUALS(apt1<apt2, false );

    // -------- Squares destroying -------------------------------------------//
    deleteSquares(squares1, length1);
    deleteSquares(squares2, length2);
    return true;
}

bool plusOperatorTest() {
    // ----- Apartment 1 -----------------------------------------------------//
    const int length1 = 2;
    const int width1 = 2;
    const int price1 = 10000;
    Apartment::SquareType** squares1 = new Apartment::SquareType*[length1];
    for (int i = 0; i < length1; ++i) {
        squares1[i] = new Apartment::SquareType[width1];
        for (int j = 0; j < width1; ++j) {
                squares1[i][j] = ((i%2==0)&&(j%3==0))?Apartment::EMPTY:Apartment::WALL;
        }
    }
    const Apartment apt1(squares1,length1,width1,price1);

    // ----- Apartment 2 -----------------------------------------------------//
    const int length2 = 3;
    const int width2 = 3;
    const int price2 = 200;
    Apartment::SquareType** squares2 = new Apartment::SquareType*[length2];
    for (int i = 0; i < length2; ++i) {
        squares2[i] = new Apartment::SquareType[width2];
        for (int j = 0; j < width2; ++j) {
                squares2[i][j] = ((i%2==0)&&(j%2==0))?Apartment::EMPTY:Apartment::WALL;
        }
    }
    const Apartment apt2 (squares2,length2,width2,price2);

    // ------- Operator usage ------------------------------------------------//
    Apartment resultApt = apt1+apt2;
    ASSERT_EQUALS(3, resultApt.getWidth());
    ASSERT_EQUALS(5, resultApt.getLength());
    ASSERT_EQUALS(price1+price2, resultApt.getPrice());

    // -------- Squares destroying -------------------------------------------//
    deleteSquares(squares1, length1);
    deleteSquares(squares2, length2);

    return true;
}

bool apartmentTest() {
	setvbuf(stdout, NULL, _IONBF, 0);
	setvbuf(stderr, NULL, _IONBF, 0);
    RUN_TEST(constructorTest1);
    RUN_TEST(constructorTest2);
    RUN_TEST(getTotalAreaTest);
    RUN_TEST(getPriceTest);
    RUN_TEST(getLengthTest);
    RUN_TEST(getWidthTest);
    RUN_TEST(assignmentOperatorTest);
    RUN_TEST(plusEqualOperatorTest1);
    RUN_TEST(plusEqualOperatorTest2);
    RUN_TEST(plusEqualOperatorTest3);
    RUN_TEST(roundBracketsOperatorTest);
    RUN_TEST(smallerThanOperatorTest1);
    RUN_TEST(smallerThanOperatorTest2);
    RUN_TEST(plusOperatorTest);
    return true;
}
