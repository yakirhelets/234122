DEBUG = -g #debug mode. delete for release version

LIBS = -L. -lmtm -lex1

FLAGS = -std=c99 -Wall -pedantic-errors -Werror $(LIBS) $(DEBUG)

FLAGS_WITHOUT_LIB = -std=c99 -Wall -pedantic-errors -Werror $(DEBUG)

VALGRIND_FLAGS = -std=c99 -Wall -pedantic-errors -Werror -g $(LIBS)

TEST_DIR = tests/

ADTS = mtm_ex2.o yad3.o offer.o customer.o realtor.o auxFunctions.o

GCC = gcc -o $@ $(FLAGS) -c $*.c 

GCC_TEST = gcc -o $@ $(FLAGS_WITHOUT_LIB) -c $*.c 

TESTS = yad3_test offer_test customer_test realtor_test list_mtm1_test

FILES_TO_CLEAN = yad3_test offer_test customer_test realtor_test list_mtm1_test

#mtm_yad3:
mtm_yad3 : $(ADTS)
	gcc -o $@ $(ADTS) $(FLAGS)

#mtm_yad3 for valgrind:	
val : $(ADTS)
	gcc -o $@ $(ADTS) $(VALGRIND_FLAGS)
	


#.o files of mtm_yad3:	
	
auxFunctions.o : auxFunctions.c auxFunctions.h map.h apartment_service.h \
 apartment.h
	$(GCC)	
	
customer.o : customer.c customer.h auxFunctions.h map.h \
 apartment_service.h apartment.h
	$(GCC)	

realtor.o : realtor.c realtor.h map.h apartment_service.h apartment.h \
 auxFunctions.h
	$(GCC)	
	
offer.o : offer.c offer.h auxFunctions.h map.h apartment_service.h \
 apartment.h
	$(GCC)

yad3.o : yad3.c yad3.h realtor.h map.h apartment_service.h apartment.h \
 auxFunctions.h customer.h offer.h mtm_ex2.h set.h list.h
	$(GCC)	
	
mtm_ex2.o : mtm_ex2.c yad3.h realtor.h map.h apartment_service.h \
 apartment.h auxFunctions.h customer.h offer.h mtm_ex2.h set.h list.h
	$(GCC)	

#tests:
tests : $(TESTS)	
    
list_mtm1_test : list_mtm/$(TEST_DIR)list_mtm1_test.c $(TEST_DIR)test_utilities.h list_mtm/list_mtm1.c list_mtm/list_mtm1.h
	gcc list_mtm/$(TEST_DIR)list_mtm1_test.c $(TEST_DIR)test_utilities.h list_mtm/list_mtm1.c list_mtm/list_mtm1.h -o list_mtm1_test $(FLAGS_WITHOUT_LIB)

customer_test : $(TEST_DIR)customer_test.c $(TEST_DIR)test_utilities.h customer.o
	$(GCC_TEST)

realtor_test: $(TEST_DIR)realtor_test.c $(TEST_DIR)test_utilities.h realtor.o
	$(GCC_TEST)
	
offer_test : $(TEST_DIR)offer_test.c $(TEST_DIR)test_utilities.h offer.o
	$(GCC_TEST)
	
yad3_test : $(TEST_DIR)yad3_test.c $(TEST_DIR)test_utilities.h yad3.o
	$(GCC_TEST)

clean :
	rm -f $(ADTS) $(TESTS) mtm_yad3 $(FILES_TO_CLEAN)