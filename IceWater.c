// The 8-bit Ice Water Emulator

#include<stdio.h>
#include <stdint.h>


uint8_t buff[255]; // Max size of file
uint8_t eqn_len = 6; // In the format OpNum1Num2, 1 byte each
char file[] = "program.txt"; // Path to file

// Equations struct
struct eqn{
    uint8_t op;
    uint8_t rs1;
    uint8_t rs2;
    uint8_t rd;
    uint8_t status;
};

uint8_t add(uint8_t x, uint8_t y){
    uint8_t result = x + y;
    return result;
}

uint8_t sub(uint8_t x, uint8_t y){
    uint8_t result = x - y;
    return result;
}

uint8_t mult(uint8_t x, uint8_t y){
    uint8_t result = x * y;
    return result;
}

uint8_t div(uint8_t x, uint8_t y){
    uint8_t result = x / y;
    return result;
}

main(){

    printf("----------------------------------------------------------\n");
    printf("This is a program to emulate the 8-bit Ice Water Processor\n");
    printf("----------------------------------------------------------\n\n\n");


    // Reading the file
    printf("---Reading file---\n");
    FILE *fptr;

    if ((fptr = fopen(&file, "r")) == NULL){
       printf("Error! opening file");
       // Program exits if the file pointer returns NULL.
       exit(1);
   }
    fscanf(fptr,"%s", buff);
    printf("Program is: %s\n", buff);
    fclose(fptr);


    uint8_t i;
    uint8_t eqn_num = strlen(buff)/eqn_len;

    // Program exits if text file has wrong size
    if(strlen(buff)%eqn_len){
        printf("Wrong size of program!");
        return 0;
    }

    printf("Equations number: %d\n\n", eqn_num);


    struct eqn eqns[eqn_num];

    // Parsing the equations
    printf("---Parsing file---\n");
    for(i=0; i<eqn_num; i++){

        sscanf(buff+eqn_len*i, "%02x", &eqns[i].op);
        sscanf(buff+eqn_len*i+2, "%02x", &eqns[i].rs1);
        sscanf(buff+eqn_len*i+4, "%02x", &eqns[i].rs2);

        printf("eqn: %d op: %02x rs1: %02x rs2: %02x\n", i+1,eqns[i].op,eqns[i].rs1,eqns[i].rs2);
    }

    printf("\n");

    printf("Operations meaning- 00: Add, 01: Sub, 02, Mult: 04: Div\n");
    printf("Status meaning- 00: OK, 01: Overflow, 02: Underflow, 03: Decimal, 04: Zero Division.\n\n");
    // Status: 0 ok, 1 overflow, 2 underflow, 3 decimal, 4 zero division

    // Processing the equations
    printf("---Processing equations---\n");
    for(i=0; i<eqn_num; i++){

        switch(eqns[i].op)
        {
        case 00: // Addition
            eqns[i].status=0;
            printf("Equation %d: %02x + %02x = ", i+1, eqns[i].rs1, eqns[i].rs2);

            eqns[i].rd=add(eqns[i].rs1, eqns[i].rs2);

            // Check overflow
            if((eqns[i].rd < eqns[i].rs1) || (eqns[i].rd < eqns[i].rs2)){
                eqns[i].status = 1;
            }
            break;
        case 01: // Subtraction
            eqns[i].status=0;
            printf("Equation %d: %02x - %02x = ", i+1, eqns[i].rs1, eqns[i].rs2);

            eqns[i].rd=sub(eqns[i].rs1, eqns[i].rs2);

            // Check Underflow
            if(eqns[i].rs1 < eqns[i].rs2){
                eqns[i].status = 2;
            }
            break;
        case 02: // Multiplication
            eqns[i].status=0;
            printf("Equation %d: %02x * %02x = ", i+1, eqns[i].rs1, eqns[i].rs2);

            eqns[i].rd=mult(eqns[i].rs1, eqns[i].rs2);

            break;
        case 03: // Division
            eqns[i].status=0;
            printf("Equation %d: %02x / %02x = ", i+1, eqns[i].rs1, eqns[i].rs2);
            // Check division by Zero
            if (eqns[i].rs2==0){
                eqns[i].status=4;
                printf("Error: Can't divide by Zero! - status: %02x\n", eqns[i].status);
                printf("Unprocessed equations: %d\n", eqn_num-i-1);
                printf("Exiting...\n\n");
                return 0;
                continue;
            }

            eqns[i].rd=div(eqns[i].rs1, eqns[i].rs2);

            // Check decimal
            if(eqns[i].rs1 % eqns[i].rs2){
                eqns[i].status = 3;
            }
            break;
        default:
            printf("Operation not known!");
        }
        printf("%02x - status: %02x\n" , eqns[i].rd, eqns[i].status);

    }

    printf("\nProgram finished.\n");

   return 0;
}
