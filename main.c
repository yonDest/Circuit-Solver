//
//  main.c
//  Circuit_Impedance_Solver
//
//  Created by Yoni Desta on April 2018.
//  Copyright © 2018 Yoni Desta. All rights reserved.
//

#include<stdio.h>
#include<stdlib.h>

float seriesResistance(){
    int count, i = 0;
    float result = 0;
    float arr[100];
    printf("Enter Resistor Count = ");
    scanf("%d", &count);

    while(i < count){ //enter values
        if(i == 0){
            printf("Enter 1st R value = ");
            scanf("%f", &arr[i]);
        }
        else if(i == 1){
            printf("Enter 2nd R value = ");
            scanf("%f", &arr[i]);
        }
        else if(i == 2){
            printf("Enter 3rd R value = ");
            scanf("%f", &arr[i]);
        }
        else{
            printf("Enter %dth R value = ", i+1);
            scanf("%f", &arr[i]);
        }
        i++;
    }
    // do calculations of values
    for(i = 0; i < count; i++){
        result += arr[i];
    } return (float)result;
}

float seriesCapacitance(){
    int count, i = 0;
    float result = 0;
    float arr[100];
    printf("Enter Capacitor Count = ");
    scanf("%d", &count);
    
    while(i < count){ //enter values
        if(i == 0){
            printf("Enter 1st C value in Farads = ");
            scanf("%f", &arr[i]);
        }
        else if(i == 1){
            printf("Enter 2nd C value in Farads = ");
            scanf("%f", &arr[i]);
        }
        else if(i == 2){
            printf("Enter 3rd C value in Farads = ");
            scanf("%f", &arr[i]);
        }
        else{
            printf("Enter %dth C value in Farads = ", i+1);
            scanf("%f", &arr[i]);
        }
        i++;
    }
    // do calculations of values
    for(i = 0; i < count; i++){
        if(arr[i] != 0){
            result += (1/arr[i]);
        }
    } return (1/result);
}

void mainMenu(void) {
    int choice = 4;
    do {
        printf("\n\n... Welcome to the Main Menu ...\n\n");
        printf("\tSelect Circuit to Solve:\n\t(1.) Series Resistance Circuit\n\t(2.) Series Capacitor Circuit\n\t(3.) Series Inductor Circuit");
        printf("\n\t(4.) Quit\n\n\t    Enter Your Selection [1-4]: ");
        scanf("%d", &choice);
        
        switch(choice) {
            case 1: printf("* Impedance is %f𝛺 *\n\n", seriesResistance()); break;
            case 2: printf("* Impedance is %fF *\n\n", seriesCapacitance()); break;
            case 3: break;
        }
    } while(choice != 4);
}

int main()
{
    mainMenu();
    
    return 0;
}
// Series Resistance Circuit \n\t(2.) Series Capacitor Circuit\n\tSeries Resistor-Capacitor (RC) Circuit\n\t(3.) Series Inductor-Capacitor (LC) Circuit\n\t(4.) Parallel Resistance Circuit\n\t(5.) Parallel Resistor-Capacitor (RC) Circuit"
