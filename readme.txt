This is a program done in C++ that analyzes the size of the file by counting the number of bytes. The program first checks if the file inputted in the argument is valid. Then it creates a pthread (POSIX thread) to count the number of words in the file and calculates the percentage of the current progress. The pthread will first output the progress bar using dashes and plus symbols and then the total number of words in file.

- To compile: type "make" on command shell
- To run: "./wordcount <filename>
- Partner for assignment: Alberto Guadiana

