#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

// freaky GNUPlot stuff
void plotGraph(const char *filename) {
    FILE *gnuplot = popen("gnuplot -persistent", "w");
    if (gnuplot != NULL) {
        fprintf(gnuplot, "set title 'Sequence Graph'\n");
        fprintf(gnuplot, "set xlabel 'n'\n");
        fprintf(gnuplot, "set ylabel 'f(n)'\n");
        fprintf(gnuplot, "set grid\n");
        fprintf(gnuplot, "plot '%s' with lines\n", filename);
        pclose(gnuplot);
    } else {
        printf("Error: Could not open gnuplot.\n");
    }
}

int main(int argc, char *argv[]) {
    int plotornah = 0; // 1 is do plot 0 is no

    // Variable to print the table in the console, or nahh
    int printornah = 1;
    int deleteafter = 1; // 1 means don't delete the file after plotting
    if (argc > 1) {
        for (int x = 0; x < argc; x++) {
            if (strcmp(argv[x], "--p") == 0 || strcmp(argv[x], "--plot") == 0) {
                plotornah = 1;
            }
            else if (strcmp(argv[x], "--noprint") == 0 || strcmp(argv[x], "--n") == 0) {
                printornah = 0;
            } else if (strcmp(argv[x], "--remove") == 0) {
                deleteafter = 0;
            } 
        }
    }
    char arithmeticOrGeometric[10];

    double rate, intercept;
    int progressionType, end; // progressionType: 1 is geometric 0 is arithmetic 2 is exponential
    int x = 0;
    FILE *fp = fopen("data.txt", "w");  // filetime hehehehe

    if (fp == NULL) {
        printf("Error opening file!\n");
        return 1;
    }

    // Arithmetic or geometric or exponentioal
    printf("Arithmetic, Geometric, or Exponential (a, g, or e): ");
    scanf("%s", arithmeticOrGeometric);

    // Check for what type to use
    if (strcmp(arithmeticOrGeometric, "arithmetic") == 0 || strcmp(arithmeticOrGeometric, "ari") == 0 || strcmp(arithmeticOrGeometric, "a") == 0) {
        progressionType = 0;
    } else if (strcmp(arithmeticOrGeometric, "geometric") == 0 || strcmp(arithmeticOrGeometric, "geo") == 0 || strcmp(arithmeticOrGeometric, "g") == 0) {
        progressionType = 1;
    } else if (strcmp(arithmeticOrGeometric, "exponential") == 0 || strcmp(arithmeticOrGeometric, "e") == 0) {
        progressionType = 2;
    }else {
        // You didn't type in any of thos
        printf("Please enter 'arithmetic', 'geometric', or 'exponential'\n");
        return 1;
    }

    // Get the rate and intercept
    printf("Enter your rate (Common difference/common ratio): ");
    if (scanf("%lf", &rate) != 1) {
        printf("Error: Invalid input for rate. Please enter a number.\n");
        return 1;
    }

    // Get the start
    printf("Enter your intercept (or start, f(0)/f(1)): ");
    if (scanf("%lf", &intercept) != 1) {
        printf("Error: Invalid input for intercept. Please enter a number.\n");
        return 1;
    }

    // Get the min X
    printf("min: ");
    if (scanf("%lf", &x) != 1) {
        printf("Error: Invalid input for minimum. Please enter a number.\n");
        return 1;
    }

    // Get the max X
    printf("max: ");
    if (scanf("%lf", &end) != 1) {
        printf("Error: Invalid input for Maximum. Please enter a number.\n");
        return 1;
    }

    // Get the start again?
    char start0[4];
    printf("Is the first value you put in f(1) (no for f(0)): ");
    scanf("%s", start0);

    // Error handling

    if (rate == 0 && progressionType == 1) {
        printf("Error: Geometric sequences cannot have a ratio of 0.\n");
        return 1;
    }

    // More error handling
    if (intercept == 0 && progressionType == 1) {
        printf("Error: An intercept for a geometric sequence will result in all values become 0. Replacing 0 with 1\n");
        intercept = 1;
    }

    // MORE ERROR HANDLING
    if (end < x) {
        printf("Error: Max value should be greater than min value.\n");
        return 1;
    }

    
    if (strcmp(start0, "no") == 0 || strcmp(start0, "n") == 0 || strcmp(start0, "0") == 0) {
        // Nothing
    } else if (strcmp(start0, "yes") == 0 || strcmp(start0, "y") == 0 || strcmp(start0, "1")) {
        // Nah, I'd adapt
        if (progressionType == 0) {
            intercept -= rate;
        } else if (progressionType == 1) {
            intercept /= rate;
        }
    } else {
        // No valid input
        printf("Please enter a valid input.\n");
        return 1;
    }

    // Print the table header
    if(printornah == 1){
        printf("  n | f(n\n");
        printf("----------\n");
    }
    // Show the graph and write the data
    while (x <= end) {
        double fn = 0;
        if (progressionType == 0) {
            fn = intercept + x * rate; // Arithmetic
        } else if (progressionType == 1) {
            fn = intercept * pow(rate, x); // Geometric
        } else if (progressionType == 2) {
            fn = intercept * exp(rate * x); // Exponential
        }
        if (printornah == 1) {
            printf("%3d | %lf\n", x, fn);
        }
        fprintf(fp, "%d %lf\n", x, fn);
        ++x;
    }
    
    // Print the equation
    if (progressionType == 0) {
        printf("Equation: f(n) = %lf + %lf * n", intercept, rate);
    } else if (progressionType == 1) {
        printf("Equation: f(n) = %lf * %lf ^ n", intercept, rate);
    } else if (progressionType == 2) {
        printf("Equation: f(n) = %lf * e ^ (%lf * n)", intercept, rate)
    }    

    fclose(fp);

    // Open Graph
    if (plotornah == 1) {
        plotGraph("data.txt");
    }
    
    // Delete the data.txt file
    if (deleteafter == 0) {
        if (remove("data.txt") == 0) {
            printf("Sucessfully deleted file");
        } else {
            printf("Error: could not delete file");
        }
    }

    // Yippie
    return 0;
}
