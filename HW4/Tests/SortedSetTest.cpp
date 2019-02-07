#include <cstdlib>
#include "SortedSet.h"
#include "MtmTst.h"

struct Int {
    int i;

    Int(int i) : i(i) {}

    operator int() const { return i; }
};
class IntCompare {
public:
    bool operator()(const Int& i1, const Int& i2) const {
        return static_cast<int>(i1) < static_cast<int>(i2);
    }
};

bool sortedSetConstructorsTest1(){
	ASSERT_NO_THROW((SortedSet<Int , IntCompare>()));
	SortedSet<Int , IntCompare> set;

	ASSERT_NO_THROW((SortedSet<Int , IntCompare>(set)));

	return true;
}

bool sortedSetInsertTest(){
	ASSERT_NO_THROW((SortedSet<Int , IntCompare>()));
	SortedSet<Int , IntCompare> set;
	ASSERT_EQUALS(true , set.insert(3));
	ASSERT_EQUALS(true , set.insert(2));
	ASSERT_EQUALS(true , set.insert(1));
	ASSERT_EQUALS(false , set.insert(2));

	return true;
}

bool sortedSetRemoveTest(){
	ASSERT_NO_THROW((SortedSet<Int , IntCompare>()));
	SortedSet<Int , IntCompare> set;
	ASSERT_EQUALS(true , set.insert(3));
	ASSERT_EQUALS(true , set.insert(2));
	ASSERT_EQUALS(true , set.insert(1));
	ASSERT_EQUALS(false , set.insert(2));
	ASSERT_EQUALS(true , set.remove(2));
	ASSERT_EQUALS(false , set.remove(2));

	return true;
}

bool sortedSetConstructorsTest2(){
	ASSERT_NO_THROW((SortedSet<Int , IntCompare>()));
	SortedSet<Int , IntCompare> set;
	ASSERT_EQUALS(true , set.insert(3));
	ASSERT_EQUALS(true , set.insert(2));
	ASSERT_EQUALS(true , set.insert(1));

	ASSERT_NO_THROW((SortedSet<Int , IntCompare>(set)));


	SortedSet<Int , IntCompare>* setCopy = new SortedSet<Int , IntCompare>(set);
	SortedSet<Int , IntCompare>::iterator it = setCopy->begin();
	ASSERT_EQUALS(1 , *(it++));
	ASSERT_EQUALS(2 , *(it++));
	ASSERT_EQUALS(3 , *(it));

	delete setCopy;
	return true;
}

bool sortedSetAssignmentOperatorTest(){
	SortedSet<Int , IntCompare> set;
	ASSERT_EQUALS(true , set.insert(3));
	ASSERT_EQUALS(true , set.insert(2));
	ASSERT_EQUALS(true , set.insert(1));

	SortedSet<Int , IntCompare> set2;
	ASSERT_EQUALS(true , set2.insert(87));
	ASSERT_EQUALS(true , set2.insert(106));
	ASSERT_EQUALS(true , set2.insert(3));

	set2 = set;

	ASSERT_EQUALS(true , set.remove(1));
	ASSERT_EQUALS(1 , *(set2.begin()));

	return true;
}

bool sortedSetIteratorTest(){
	SortedSet<Int , IntCompare> set;
	ASSERT_EQUALS(set.end() , set.begin());
	ASSERT_EQUALS(true ,set.end() == set.begin());
	ASSERT_EQUALS(true , set.insert(77));
	SortedSet<Int , IntCompare>::iterator it = set.begin();
	ASSERT_EQUALS(77 , *it);
	ASSERT_EQUALS(true ,set.end() != set.begin());
	ASSERT_EQUALS(true , set.insert(7));
	ASSERT_EQUALS(true , set.insert(777));
	it = set.begin();
	ASSERT_EQUALS(7 , *(it++));
	ASSERT_EQUALS(77 , *(it++));
	ASSERT_EQUALS(777 , *(it++));
	ASSERT_EQUALS(set.end() , it);
	return true;
}

bool sortedSetSizeTest(){
	SortedSet<Int , IntCompare> set;
	Int array[] = { 765 , 52 , 99 , 0 , -32 , 1112 };
	for(int i = 0 ; i < 6 ; i++){
		ASSERT_EQUALS(i , set.size());
		ASSERT_EQUALS(true , set.insert(array[i]));
	}
	ASSERT_EQUALS(6 , set.size());
	ASSERT_EQUALS(false , set.remove(31));
	ASSERT_EQUALS(6 , set.size());
	set.remove(-32);
	ASSERT_EQUALS(5 , set.size());
	set.remove(1112);
	ASSERT_EQUALS(4 , set.size());
	set.remove(0);
	set.remove(99);
	set.remove(765);
	set.remove(52);
	ASSERT_EQUALS(0 , set.size());
	set.insert(99999);
	ASSERT_EQUALS(1 , set.size());
	return true;
}

bool sortedSetFindTest(){
	SortedSet<Int , IntCompare> set;

	ASSERT_EQUALS(true , set.insert(1));
	ASSERT_EQUALS(true , set.insert(2));
	ASSERT_EQUALS(true , set.insert(3));
	ASSERT_EQUALS(true , set.insert(4));
	ASSERT_EQUALS(true , set.insert(6));
	ASSERT_EQUALS(true , set.insert(5));

	ASSERT_EQUALS(set.end() , set.find(-1));
	ASSERT_EQUALS(set.begin() , set.find(1));

	SortedSet<Int , IntCompare>::iterator it = set.find(2);

	ASSERT_EQUALS(3 , *(++it));

	return true;
}

bool sortedSetUnionTest(){
	SortedSet<Int , IntCompare> set1;
	SortedSet<Int , IntCompare> set2;

	for(int i = 0 ; i < 5 ; i++){
		set1.insert(i);
	}
	for(int i = 5 ; i < 10 ; i++){
			set2.insert(i);
	}

	SortedSet<Int , IntCompare> set3 = (set1 | set2);
	ASSERT_EQUALS(10 , set3.size());
	SortedSet<Int , IntCompare> set4 = (set2 | set1);
	ASSERT_EQUALS(10 , set4.size());

	SortedSet<Int , IntCompare>::iterator it3 = set3.begin();
	SortedSet<Int , IntCompare>::iterator it4 = set4.begin();
	for(int i = 0 ; i < 10 ; i++){
		ASSERT_EQUALS(*(it3++) , *(it4++));
	}

	return true;
}

bool sortedSetIntersectionTest(){
	SortedSet<Int , IntCompare> set1;
	SortedSet<Int , IntCompare> set2;

	for(int i = 0 ; i < 6 ; i++){
		set1.insert(i);
	}
	for(int i = 5 ; i < 10 ; i++){
			set2.insert(i);
	}

	SortedSet<Int , IntCompare> set3 = (set1 & set2);
	ASSERT_EQUALS(1 , set3.size());
	SortedSet<Int , IntCompare> set4 = (set2 & set1);
	ASSERT_EQUALS(1 , set4.size());

	SortedSet<Int , IntCompare>::iterator it3 = set3.begin();
	SortedSet<Int , IntCompare>::iterator it4 = set4.begin();
	ASSERT_EQUALS(*(it3) , *(it4));

	return true;
}

bool sortedSetComplementTest(){
	SortedSet<Int , IntCompare> set1;
	SortedSet<Int , IntCompare> set2;

	for(int i = 0 ; i < 6 ; i++){
		set1.insert(i);
	}
	for(int i = 5 ; i < 10 ; i++){
		set2.insert(i);
	}

	SortedSet<Int , IntCompare> set3 = (set1 - set2);
	ASSERT_EQUALS(5 , set3.size());
	SortedSet<Int , IntCompare> set4 = (set2 - set1);
	ASSERT_EQUALS(4 , set4.size());

	SortedSet<Int , IntCompare>::iterator it3 = set3.begin();
	SortedSet<Int , IntCompare>::iterator it4 = set4.begin();

	for(int i = 0 ; i < 4 ; i++){
			ASSERT_EQUALS(i , *(it3++));
			ASSERT_EQUALS(i+6 , *(it4++));
	}

	return true;
}

bool sortedSetSymmetricDifferenceTest(){
	SortedSet<Int , IntCompare> set1;
	SortedSet<Int , IntCompare> set2;

	for(int i = 0 ; i < 6 ; i++){
		set1.insert(i);
	}
	for(int i = 5 ; i < 10 ; i++){
		set2.insert(i);
	}

	SortedSet<Int , IntCompare> set3 = (set1 ^ set2);
	ASSERT_EQUALS(9 , set3.size());
	SortedSet<Int , IntCompare> set4 = (set2 ^ set1);
	ASSERT_EQUALS(9 , set4.size());

	SortedSet<Int , IntCompare>::iterator it3 = set3.begin();
	SortedSet<Int , IntCompare>::iterator it4 = set4.begin();

	for(int i = 0 ; i < 9 ; i++){
			ASSERT_EQUALS(*(it4++) , *(it3++));
	}

	return true;
}

bool sortedSetTest() {
	RUN_TEST(sortedSetConstructorsTest1);
	RUN_TEST(sortedSetInsertTest);
	RUN_TEST(sortedSetRemoveTest);
	RUN_TEST(sortedSetSizeTest);
	RUN_TEST(sortedSetIteratorTest);
	RUN_TEST(sortedSetConstructorsTest2);
	RUN_TEST(sortedSetAssignmentOperatorTest);
	RUN_TEST(sortedSetFindTest);
	RUN_TEST(sortedSetUnionTest);
	RUN_TEST(sortedSetIntersectionTest);
	RUN_TEST(sortedSetComplementTest);
	RUN_TEST(sortedSetSymmetricDifferenceTest);

    return true;
}

