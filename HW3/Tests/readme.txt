O------------------------------------------------------------------------------O
|                                                                              |
|                |  _`\                   ( )   /'\_/`\                        |
|                | (_) )   __     _ _    _| |   |     |   __                   |
|                | ,  /  /'__`\ /'_` ) /'_` |   | (_) | /'__`\                 |
|                | |\ \ (  ___/( (_| |( (_| |   | | | |(  ___/                 |
|                (_) (_)`\____)`\__,_)`\__,_)   (_) (_)`\____)                 |
|                                                                              |
O------------------------------------------------------------------------------O

                This program contains the following scripts:

O------------------------------------------------------------------------------O

▩ cleanLines: [Parameters: Text given via a file]                         [AUX]
================================================================================
    ◢ An auxiliary script that removes empty lines and remarks from a given      
    raw text.                                                                  
    Remarks begin with the char '#'.     
        
O------------------------------------------------------------------------------O

▩ handleNamesDates:                                                       [AUX] 
================================================================================
   ◢ An auxiliary script for data organization and optimization.
   
   ◢ Functions:
        ◍ date_to_num:     ◢ Parameters: A date represented by a string.
                            ◢ Task: Receives a date in a format DD.MM.YYYY and
                                    turns it into a number in the 
                                    format YYYYMMDD.
                            
        ◍ convert_dates:   ◢ Parameters: A text file (.flat format).
                            ◢ Task: Converts all dates (DD.MM.YYYY format) in 
                                    a given file to an easier, comparable 
                                    format (YYYYMMDD).
                                    
        ◍ add_index:       ◢ Parameters: A text file (.flat format).
                            ◢ Task: Adds a numerical index specifier 
                                     value to every line.
                                     
        ◍ underline_names: ◢ Parameters: A text file (.flat format).
                            ◢ Task:  Replaces every space with an underscore 
                            '_' and every uppercase character with
                            a lowercase char. 
        
O------------------------------------------------------------------------------O


▩ getApartments: [Command: getApartments <file> <city> <limit>]          [MAIN]
================================================================================
    ◢ The main script.
       Can be executed with the first parameter only OR the entire 3 parameters.
    
    ◢ Parameters:
        ● file:  Name of a text file which contains a list of files
                 or directories. Every name in the list is seperated
                 by a new line.
                 In-file remarks begin with the char '#'.
                 Only .flat files will be executed in the main script.
        ● city:  A city name to search for apartments. 
                 May contain spaces. (e.g: "Tel Aviv", "Kiryat  Yam").
        ● limit: Number of maximum matches to be printed.
                 May be a positive integer only.
                          
    ◢ Functions:
        ◍ check_first_parameter: 
            ◢ Parameters: A string representing the first parameter of the 
                          command.
            ◢ Task: Checks for the existence of the source file.
                    If file doesn't exist, an error message is
                    given ("File is missing").
                    
        ◍ check_third_parameter: 
            ◢ Parameters: A string representing the third parameter of the
                           command.
            ◢ Task: Checks whether the <limit> parameter is a
                     positive integer. If not, an error message is given.
                     ("Illegal or missing parameters").
                     
        ◍ recursive_search:      
            ◢ Parameters: A path from a source file.
            ◢ Task: Traverse through every sub-folder and files in the
                    given path to find all the valid
                    .flat files to activate a search on. (Only valid paths are
                    checked).
                          
O------------------------------------------------------------------------------O
O------------------------------------------------------------------------------O