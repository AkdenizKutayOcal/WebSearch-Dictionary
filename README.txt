Author: Akdeniz Kutay Ocal

This project is developed for CMPE382 course. We were asked to build a dictionary 
for web search query logs and write into it. In this dictionary, each line will 
constitute a unique query followed by the frequency of the query in the Data files. 


Design overview:

    The project consists of different tasks and the codes that are seperated according
    to that. Detailed explanations of what is done in each tasks can be found in report.
    Data files that are used during execution can be found in Data file. The output 
    dictionaries of each task are stored in Resulting Dictionaries file.

    Task 1: 

        task1.c file contains Task1's implementation which is the Trie Data Structure.
        Can be executed by console commands;

        $ gcc -o task1 task1.c
        $ ./task1

    Task 2: 

        task2.c file contains Task2's implementation which is Sequential Execution - 
        One Query at a Time. Can be executed by console commands;

        $ gcc -o task2 task2.c
        $ ./task2
    
    Task 3: 

        task3.c file contains Task3's implementation which is Sequential Execution - 
        Multiple Queries. Can be executed by console commands;

        $ gcc -o task3 task3.c
        $ ./task3
    
    Task 4: 

        task4.c file contains Task4's implementation which is the Threaded Execution. 
        Can be executed by console commands;

        $ gcc -pthread -o task4 task4.c
        $ ./task4

    Task 5: 

        task5.c file contains Task5's implementation which is the Threaded Execution - 
        Multiple Tries. Can be executed by console commands;

        $ gcc -pthread -o task5 task5.c
        $ ./task5

    Task 6: 

        task6.c file contains Task6's implementation which is the Completely Memory-Based 
        Dictionary Creation. Can be executed by console commands;

        $ gcc -o task6 task6.c
        $ ./task6

    Task 7: 

        Improvements for previous tasks are done in Task 7. task7a.c is improved version of
        task2.c which is Improved Trie (Explained in Report). Can be executed by console 
        commands;

        $ gcc -o task7a task7a.c
        $ ./task7a
        
        task7b.c is improved version of task5.c (Explained in Report). Can be executed by
        console commands;

        $ gcc -pthread -o task7b task7b.c
        $ ./task7b

Complete specification:

    Every task and step that is done while writing the codes explained detaily in report that 
    can be found in zip in a pdf format. 

Known bugs or problems:

    We had given 10 data files with a very large number of queries but it was impossible 
    for me to work with them as they are given since my computer does not satisfy the 
    performance requirements. I had used a Tablet Laptop which has 4 GB of RAM and 4 cores 
    and I encountered with memory overflows and killed errors. That’s why I split the files 
    so that every 10 files have 50000 lines of queries to work on. Therefore I could not test
    my code in large files.

    Task5 caused memory overflow while working with this data. Therefore I had to asked one of
    my friends to run my code in his computer in order to compare the results in same dataset.
    The effect of running that in different computer and comparisons of results can be found in
    the report.
	
    my friends to run my code in his computer in order to compare the results in same dataset.
    The effect of running that in different computer and comparisons of results can be found in
    the report.
