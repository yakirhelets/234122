#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "yad3.h"
#include "mtm_ex2.h"

#define DELIMITER " \r\t\n"


/** Converts a given Yad3Result type into a MtmErrorCode type.
 * If the Yad3Result type is different than a Success message, this function
 * will print it using mtmPrintErrorMessage.
 */
static void Yad3ResultToMtmErrorCode(Yad3Result result) {
	MtmErrorCode error_code;
	switch(result) {
		case YAD3_SUCCESS: return;
		case YAD3_OUT_OF_MEM: error_code = MTM_OUT_OF_MEMORY; break;
		case YAD3_INVALID_COMMAND_LINE_PARAMETERS: error_code = MTM_INVALID_COMMAND_LINE_PARAMETERS; break;
		case YAD3_NULL_ARG: error_code = MTM_INVALID_PARAMETERS; break;
		case YAD3_CANNOT_OPEN_FILE: error_code = MTM_CANNOT_OPEN_FILE; break;
		case YAD3_BAD_ARG: error_code = MTM_INVALID_PARAMETERS; break;
		case YAD3_EMAIL_ALREADY_EXISTS: error_code = MTM_EMAIL_ALREADY_EXISTS; break;
		case YAD3_EMAIL_DOES_NOT_EXIST: error_code = MTM_EMAIL_DOES_NOT_EXIST; break;
		case YAD3_EMAIL_WRONG_ACCOUNT_TYPE: error_code = MTM_EMAIL_WRONG_ACCOUNT_TYPE; break;
		case YAD3_ALREADY_REQUESTED: error_code = MTM_ALREADY_REQUESTED; break;
		case YAD3_NOT_REQUESTED: error_code = MTM_NOT_REQUESTED; break;
		case YAD3_APARTMENT_SERVICE_ALREADY_EXISTS: error_code = MTM_APARTMENT_SERVICE_ALREADY_EXISTS; break;
		case YAD3_APARTMENT_SERVICE_DOES_NOT_EXIST: error_code = MTM_APARTMENT_SERVICE_DOES_NOT_EXIST; break;
		case YAD3_APARTMENT_SERVICE_FULL: error_code = MTM_APARTMENT_SERVICE_FULL; break;
		case YAD3_APARTMENT_ALREADY_EXISTS: error_code = MTM_APARTMENT_ALREADY_EXISTS; break;
		case YAD3_APARTMENT_DOES_NOT_EXIST: error_code = MTM_APARTMENT_DOES_NOT_EXIST; break;
		case YAD3_PURCHASE_WRONG_PROPERTIES: error_code = MTM_PURCHASE_WRONG_PROPERTIES; break;
		case YAD3_REQUEST_WRONG_PROPERTIES: error_code = MTM_REQUEST_WRONG_PROPERTIES; break;
		case YAD3_REQUEST_ILLOGICAL_PRICE: error_code = MTM_REQUEST_ILLOGICAL_PRICE; break;
	}
	mtmPrintErrorMessage (stderr, error_code);
}

// initializing the input channel
static FILE* initInput(int argc, char** argv) {
	for (int i=1; i<argc; i+=2) {
		if (strcmp(argv[i],"-i") == 0) {
			return fopen(argv[i+1], "r");
		}
	}
	return stdin;
}

// initializing the output channel
static FILE* initOutput(int argc, char** argv) {
	for (int i=1; i<argc; i+=2) {
		if (strcmp(argv[i],"-o") == 0) {
			return fopen(argv[i+1], "w");
		}
	}
	return stdout;
}

static void checkCommandLineParameters(int argc, char** argv, Yad3Result* error_code)  {
	if (argc != 1 && argc !=3 && argc !=5) {
		*error_code = YAD3_INVALID_COMMAND_LINE_PARAMETERS;
		mtmPrintErrorMessage (stderr, *error_code);
		return;
	} else {
		if (argc == 5)	{
			if ((strcmp(argv[1],"-i")==0 && strcmp(argv[3],"-o")!=0) || (strcmp(argv[1],"-o")==0 && strcmp(argv[3],"-i")!=0))	{
				*error_code = YAD3_INVALID_COMMAND_LINE_PARAMETERS;
				Yad3ResultToMtmErrorCode(*error_code);
				return;
			}
		}
		for (int i=1; i<argc; i+=2) {
			if (strcmp(argv[i],"-i")!=0 && strcmp(argv[i],"-o")!=0) {
				*error_code = YAD3_INVALID_COMMAND_LINE_PARAMETERS;
				Yad3ResultToMtmErrorCode(*error_code);
				return;
			}
			if ((strcmp(argv[i],"-i")==0 || strcmp(argv[i],"-o")==0) && strcmp(argv[i+1],"")==0) {
				*error_code = YAD3_INVALID_COMMAND_LINE_PARAMETERS;
				Yad3ResultToMtmErrorCode(*error_code);
				return;
			}
		}
	}
}

/** functions related to realtor*/

static void executeRealtorAdd(char* string, Yad3 yad3, Yad3Result* error_code) {
	char* email = strtok(NULL, DELIMITER);
	char* company_name = strtok(NULL, DELIMITER);
	int tax_percentage = atoi(strtok(NULL, DELIMITER));
	Yad3Result result = addRealtor(yad3, email, company_name, tax_percentage);
	Yad3ResultToMtmErrorCode(result);
}

static void executeRealtorRemove(char* string, Yad3 yad3, Yad3Result* error_code) {
	char* email = strtok(NULL, DELIMITER);
	Yad3Result result = removeRealtor(yad3, email);
	Yad3ResultToMtmErrorCode(result);
}

static void executeRealtorAddApartmentService(char* string, Yad3 yad3, Yad3Result* error_code) {
	char* email = strtok(NULL, DELIMITER);
	char* service_name = strtok(NULL, DELIMITER);
	int max_apartments = atoi(strtok(NULL, DELIMITER));
	Yad3Result result = addApartmentService(yad3, email, service_name, max_apartments);
	Yad3ResultToMtmErrorCode(result);
}

static void executeRealtorRemoveApartmentService(char* string, Yad3 yad3, Yad3Result* error_code) {
	char* email = strtok(NULL, DELIMITER);
	char* service_name = strtok(NULL, DELIMITER);
	Yad3Result result = removeApartmentService(yad3, email, service_name);
	Yad3ResultToMtmErrorCode(result);
}

static void executeRealtorAddApartment(char* string, Yad3 yad3, Yad3Result* error_code) {
	char* realtor_mail = strtok(NULL, DELIMITER);
	char* service_name = strtok(NULL, DELIMITER);
	int id = atoi(strtok(NULL, DELIMITER));
	int price = atoi(strtok(NULL, DELIMITER));
	int width = atoi(strtok(NULL, DELIMITER));
	int height = atoi(strtok(NULL, DELIMITER));
	char* matrix = strtok(NULL, DELIMITER);
	Yad3Result result = realtorAddApartment(yad3, realtor_mail, service_name, id, price, width, height, matrix);
	Yad3ResultToMtmErrorCode(result);
}

static void executeRealtorRemoveApartment(char* string, Yad3 yad3, Yad3Result* error_code) {
	char* realtor_mail = strtok(NULL, DELIMITER);
	char* service_name = strtok(NULL, DELIMITER);
	int id = atoi(strtok(NULL, DELIMITER));
	Yad3Result result = realtorRemoveApartment(yad3, realtor_mail, service_name, id);
	Yad3ResultToMtmErrorCode(result);
}

static void executeRealtorRespondToOffer(char* string, Yad3 yad3, Yad3Result* error_code) {
	char* realtor_mail = strtok(NULL, DELIMITER);
	char* customer_mail = strtok(NULL, DELIMITER);
	char* choice = strtok(NULL, DELIMITER);
	Yad3Result result = realtorRespondToOffer(yad3, realtor_mail, customer_mail, choice);
	Yad3ResultToMtmErrorCode(result);
}


static void handleRealtorCommand(FILE* output, char* second_word, char* current_line, Yad3 yad3, Yad3Result* error_code) {
	if (strcmp(second_word,"add") == 0) {
		executeRealtorAdd(current_line, yad3, error_code);
		if (*error_code == YAD3_OUT_OF_MEM) {
			return;
		}
	} else if (strcmp(second_word,"remove") == 0) {
			executeRealtorRemove(current_line, yad3, error_code);
			if (error_code == MTM_OUT_OF_MEMORY) {
				return;
			}
		} else if (strcmp(second_word,"add_apartment_service") == 0) {
				executeRealtorAddApartmentService(current_line, yad3, error_code);
				if (error_code == MTM_OUT_OF_MEMORY) {
					return;
				}
			} else if (strcmp(second_word,"remove_apartment_service") == 0) {
					executeRealtorRemoveApartmentService(current_line, yad3, error_code);
					if (error_code == MTM_OUT_OF_MEMORY) {
						return;
					}
				} else if (strcmp(second_word,"add_apartment") == 0) {
						executeRealtorAddApartment(current_line, yad3, error_code);
						if (error_code == MTM_OUT_OF_MEMORY) {
							return;
						}
					} else if (strcmp(second_word,"remove_apartment") == 0) {
							executeRealtorRemoveApartment(current_line, yad3, error_code);
							if (error_code == MTM_OUT_OF_MEMORY) {
								return;
							}
						} else if (strcmp(second_word,"respond_to_offer") == 0) {
								executeRealtorRespondToOffer(current_line, yad3, error_code);
								if (error_code == MTM_OUT_OF_MEMORY) {
									return;
								}
							} else {
								*error_code = MTM_INVALID_PARAMETERS;
								mtmPrintErrorMessage(stderr, MTM_INVALID_PARAMETERS);
							}
}

/** functions related to customer*/

static void executeCustomerAdd(char* string, Yad3 yad3, Yad3Result* error_code) {
	char* email = strtok(NULL, DELIMITER);
	int min_area = atoi(strtok(NULL, DELIMITER));
	int min_rooms = atoi(strtok(NULL, DELIMITER));
	int max_price = atoi(strtok(NULL, DELIMITER));
	Yad3Result result = addCustomer(yad3, email, min_area, min_rooms, max_price);
	Yad3ResultToMtmErrorCode(result);
}

static void executeCustomerRemove(char* string, Yad3 yad3, Yad3Result* error_code) {
	char* email = strtok(NULL, DELIMITER);
	Yad3Result result = removeCustomer(yad3, email);
	Yad3ResultToMtmErrorCode(result);
}

static void executeCustomerPurchase(char* string, Yad3 yad3, Yad3Result* error_code) {
	char* customer_mail = strtok(NULL, DELIMITER);
	char* realtor_mail = strtok(NULL, DELIMITER);
	char* service_name = strtok(NULL, DELIMITER);
	int id = atoi(strtok(NULL, DELIMITER));
	Yad3Result result = customerPurchase(yad3, customer_mail, realtor_mail, service_name, id);
	Yad3ResultToMtmErrorCode(result);
}

static void executeCustomerMakeOffer(char* string, Yad3 yad3, Yad3Result* error_code) {
	char* customer_mail = strtok(NULL, DELIMITER);
	char* realtor_mail = strtok(NULL, DELIMITER);
	char* service_name = strtok(NULL, DELIMITER);
	int apartment_id = atoi(strtok(NULL, DELIMITER));
	int new_price = atoi(strtok(NULL, DELIMITER));
	Yad3Result result = customerMakeOffer(yad3, customer_mail, realtor_mail, service_name, apartment_id, new_price);
	Yad3ResultToMtmErrorCode(result);
}


static void handleCustomerCommand(FILE* output, char* second_word, char* current_line, Yad3 yad3, Yad3Result* error_code) {
	if (strcmp(second_word,"add") == 0) {
		executeCustomerAdd(current_line, yad3, error_code);
		if (error_code == MTM_OUT_OF_MEMORY) {
			return;
		}
	} else if (strcmp(second_word,"remove") == 0) {
			executeCustomerRemove(current_line, yad3, error_code);
			if (error_code == MTM_OUT_OF_MEMORY) {
				return;
			}
		} else if (strcmp(second_word,"purchase") == 0) {
				executeCustomerPurchase(current_line, yad3, error_code);
				if (error_code == MTM_OUT_OF_MEMORY) {
					return;
				}
			} else if (strcmp(second_word,"make_offer") == 0) {
					executeCustomerMakeOffer(current_line, yad3, error_code);
					if (error_code == MTM_OUT_OF_MEMORY) {
						return;
					}
				} else {
					*error_code = MTM_INVALID_PARAMETERS;
					mtmPrintErrorMessage(stderr, MTM_INVALID_PARAMETERS);
				}
}

/** functions related to reports*/

static void executeReportRelevantRealtors(char* string, Yad3 yad3, FILE* output, Yad3Result* error_code) {
	char* customer_mail = strtok(NULL, DELIMITER);
	Yad3Result result = reportRelevantRealtors(yad3, output, customer_mail);
	Yad3ResultToMtmErrorCode(result);
}

static void executeReportSignificantRealtors(char* string, Yad3 yad3, FILE* output, Yad3Result* error_code) {
	int count = atoi(strtok(NULL, DELIMITER));
	Yad3Result result = reportSignificantRealtors(yad3, output, count);
	Yad3ResultToMtmErrorCode(result);
}

static void executeReportMostPayingCustomers(char* string, Yad3 yad3, FILE* output, Yad3Result* error_code) {
	int count = atoi(strtok(NULL, DELIMITER));
	Yad3Result result = mostPayingCustomers(yad3, output, count);
	Yad3ResultToMtmErrorCode(result);
}



static void handleReportCommand(FILE* output, char* second_word, char* current_line, Yad3 yad3, Yad3Result* error_code) {
	if (strcmp(second_word,"relevant_realtors") == 0) {
		executeReportRelevantRealtors(current_line, yad3, output, error_code);
		if (error_code == MTM_OUT_OF_MEMORY) {
			return;
		}
	} else if (strcmp(second_word,"significant_realtors") == 0) {
			executeReportSignificantRealtors(current_line, yad3, output, error_code);
			if (error_code == MTM_OUT_OF_MEMORY) {
				return;
			}
		} else if (strcmp(second_word,"most_paying_customers") == 0) {
				executeReportMostPayingCustomers(current_line, yad3, output, error_code);
				if (error_code == MTM_OUT_OF_MEMORY) {
					return;
				}
			} else {
				*error_code = MTM_INVALID_PARAMETERS;
				mtmPrintErrorMessage(stderr, MTM_INVALID_PARAMETERS);
			}
}

static void handleCommand(FILE* output, char* first_word, char* second_word, char* current_line, Yad3 yad3, Yad3Result* error_code)	{
	if (strcmp(first_word,"realtor") == 0) {
		handleRealtorCommand(output, second_word, current_line, yad3, error_code);
		if (error_code == MTM_OUT_OF_MEMORY) {
			return;
		}
	} else if (strcmp(first_word,"customer") == 0) {
			handleCustomerCommand(output, second_word, current_line, yad3, error_code);
			if (error_code == MTM_OUT_OF_MEMORY) {
				return;
			}
		} else if (strcmp(first_word,"report") == 0) {
				handleReportCommand(output, second_word, current_line, yad3, error_code);
				if (error_code == MTM_OUT_OF_MEMORY) {
					return;
				}
			} else {
				*error_code = MTM_INVALID_PARAMETERS;
				mtmPrintErrorMessage(stderr, MTM_INVALID_PARAMETERS);
			}
}

/* checks if the current input line is empty or a comment line*/
static void handleLines(FILE* input, FILE* output, Yad3 yad3) {
	Yad3Result error_code = YAD3_SUCCESS;
	char* first_word;
	char* second_word;
	char current_line[MAX_LEN];
	while (fgets (current_line, MAX_LEN, input) != NULL) {
		/* line is a comment*/
		if(strchr (current_line, '#') != NULL) {
			continue;
		}
		/* line is empty*/
		first_word = strtok(current_line, DELIMITER);
		if (first_word == NULL) {
			continue;
		}
		second_word = strtok(NULL, DELIMITER);
		if (second_word == NULL) {
			continue;
		}
		char current_line[MAX_LEN] = {""};
		handleCommand(output, first_word, second_word, current_line, yad3, &error_code);
		if (error_code == YAD3_OUT_OF_MEM) {
			break;
		}
	}
}

int main(int argc, char** argv) {
	setvbuf(stdout,NULL, _IONBF,0);
	setvbuf(stderr,NULL, _IONBF,0);
	Yad3Result yad3_result = YAD3_SUCCESS;
	Yad3 yad3 = yad3Create(&yad3_result);
	/* checks for memory allocation failure*/
	if (!yad3)	{
		mtmPrintErrorMessage(stderr, YAD3_OUT_OF_MEM);
		return 1;
	}
	/* checks for invalid command line parameters*/
	checkCommandLineParameters(argc, argv, &yad3_result);
	if (yad3_result == YAD3_INVALID_COMMAND_LINE_PARAMETERS)	{
		yad3Destroy(yad3);
		return 1;
	}

	FILE* input = initInput(argc, argv);
	if (!input) {
		yad3_result = YAD3_CANNOT_OPEN_FILE;
		Yad3ResultToMtmErrorCode(yad3_result);
		return 1;
	}
	/* checks for opening output file failure*/
	FILE* output = initOutput(argc, argv);
	if (!output) {
		if (input != stdin) {
			fclose(input);
		}
		yad3_result = YAD3_CANNOT_OPEN_FILE;
		Yad3ResultToMtmErrorCode(yad3_result);
		return 1;
	}
//	handleInputOutputFiles(argc, argv, input, output, &yad3_result);
	if (yad3_result == YAD3_CANNOT_OPEN_FILE)	{
		yad3Destroy(yad3);
		return 1;
	}
	handleLines(input, output, yad3);
	/*closing the input/output if they are files*/
	if (input != stdin) {
		fclose(input);
	}
	if (output != stdout) {
		fclose(output);
	}
	yad3Destroy(yad3);
	return 0;
}
