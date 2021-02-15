#pragma once

#define LINELENGTH 1024        // Number of characters on one line of the csv file.  Could be 512.
#define INITIALLOADSIZE 16384

typedef double csvloadtype; // Determines the type of the numbers to be read in.  Sticking a float here leads
                            // to too many warngins.

