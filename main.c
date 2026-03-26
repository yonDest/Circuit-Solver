//
//  main.c
//  Circuit_Impedance_Solver_Calculator
//
//  Created by Yoni Desta on April 2018.
//  Copyright © 2018 Yoni Desta. All rights reserved.
//

#include<stdio.h>
#include<stdlib.h>

float seriesResistor(){
    int count, i = 0;
    float result = 0;
    printf("Enter Resistor Count = ");
    scanf("%d", &count);
    if(count <= 1){
        printf("Error! Need more than one resistor to calculate impedance");
        return;
    }
    float arr[count];

    while(i < count){         // user enter values
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

float seriesCapacitor(){
    int count, i = 0;
    float result = 0;
    printf("Enter Capacitor Count = ");
    scanf("%d", &count);
    if(count <= 1){
        printf("Error! Need more than one capacitor to calculate impedance");
        return;
    }
    float arr[count];
    
    while(i < count){         // user enter values
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

float seriesInductor(){
    int count, i = 0;
    float result = 0;
    printf("Enter Capacitor Count = ");
    scanf("%d", &count);
    if(count <= 1){
        printf("Error! Need more than one inductor to calculate impedance");
        return;
    }
    float arr[count];
    
    while(i < count){         // user enter values
        if(i == 0){
            printf("Enter 1st L value in Henrys = ");
            scanf("%f", &arr[i]);
        }
        else if(i == 1){
            printf("Enter 2nd L value in Henrys = ");
            scanf("%f", &arr[i]);
        }
        else if(i == 2){
            printf("Enter 3rd L value in Henrys = ");
            scanf("%f", &arr[i]);
        }
        else{
            printf("Enter %dth L value in Henrys = ", i+1);
            scanf("%f", &arr[i]);
        }
        i++;
    }
    // do calculations of values
    for(i = 0; i < count; i++){
        result += arr[i];
    } return (float)result;
}

float resistorCapacitor(){
    
}

float resistorInductor(){

}

float inductorCapacitor(){

}

float resistorCapacitorInductor(){

}

float parallelResistor(){

}

flaot parallelCapacitor(){

}

float parallelInductor(){

}

void mainMenu(void) {
    int choice = 11;
    do {
        printf("\n\n... Welcome to the Main Menu ...\n\n");
        // ask for circuit type to calculate
        printf("\tSelect Circuit to Solve:\n\t(1.) Series Resistor Circuit\n\t
                (2.) Series Capacitor Circuit\n\t(3.) Series Inductor Circuit\n\t(4.) RC Circuit\n\t
                (5.) RL Circuit\n\t(6.) LC Circuit\n\t(7.) RLC Circuit\n\t(8.) Parallel Resistor Circuit\n\t
                (9.) Parallel Capacitor Circuit\n\t(10.) Parallel Inductor Circuit\n\t");
        printf("\n\t(11.) Quit\n\n\t    Enter Your Selection [1 - 11]: ");
        scanf("%d", &choice);
        
        switch(choice) {
            case 1: printf("* Impedance is %f𝛺 *\n\n", seriesResistor()); break;
            case 2: printf("* Impedance is %f𝛺 *\n\n", seriesCapacitor()); break;
            case 3: printf("* Impedance is %f𝛺 *\n\n", seriesInductor()); break;
            case 4: printf("* Impedance is %f𝛺 *\n\n", resistorCapacitor()); break;
            case 5: printf("* Impedance is %f𝛺 *\n\n", resistorInductor()); break;
            case 6: printf("* Impedance is %f𝛺 *\n\n", inductorCapacitor()); break;
            case 7: printf("* Impedance is %f𝛺 *\n\n", resistorCapacitorInductor()); break;
            case 8: printf("* Impedance is %f𝛺 *\n\n", parallelResistor()); break;
            case 9: printf("* Impedance is %f𝛺 *\n\n", parallelCapacitor()); break;
            case 10: printf("* Impedance is %f𝛺 *\n\n", parallelInductor()); break;
        }
    } while(choice != 4);
}

int main()
{
    mainMenu();
    
    return 0;
}
// Series Resistance Circuit \n\t(2.) Series Capacitor Circuit\n\tSeries Resistor-Capacitor (RC) Circuit\n\t(3.) Series Inductor-Capacitor (LC) Circuit\n\t(4.) Parallel Resistance Circuit\n\t(5.) Parallel Resistor-Capacitor (RC) Circuit"
