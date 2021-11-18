//     fertility_Diagnosis_Data_Group9_11.txt

/**************************************************************/
/* Input_priorProbability.c */
/* Tay Yi Lin  2103154@sit.singaporetech.edu.sg              */
/*************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define PI 3.142

int readingFile(void);        // function Prototypes
void priorProbability(void);  // function Prototypes
void conditionalProbability(void); // function Prototypes
void posteriorProbability(void);

// number of row in data
int Training_Row = 80;
int Testing_Row = 20;


// prior probability
double priorProbability_Normal = 0;
double priorProbability_Altered = 0;

int yAlteredCounter = 0; /*Counter for Altered diagnosis*/
int yNormalCounter = 0; /*Counter for Normal diagnosis*/

/* global variable declaration */
// split into training set and testing set
double arrayTraining[80][10];
double arrayTesting[20][10];

// further split training set and testing set into feature and output set
double trainingFeature[80][9];
double testingFeature[20][9];

double trainingOutput[80][1];
double testingOutput[20][1];

double CP_SeasonAnalysis_Normal[4];    /*Training set: conditional probability of Season Analysis under normal diagnosis*/
double CP_SeasonAnalysis_Altered[4];   /*Training set: conditional probability of Season Analysis under altered diagnosis*/
double CP_Childish_Disease_Normal[2];  /*Training set: conditional probability of Childish Disease under normal diagnosis*/
double CP_Childish_Disease_Altered[2]; /*Training set: conditional probability of Childish Disease under altered diagnosis*/
double CP_Accident_Trauma_normal[2];   /*Training set: conditional probability of Accident Trauma under normal diagnosis*/
double CP_Accident_Trauma_Altered[2];  /*Training set: conditional probability of Accident Trauma  under altered diagnosis*/
double CP_Surgical_Intervention_Normal[2];  /*Training set: conditional probability of Surgical Intervention under normal diagnosis*/
double CP_Surgical_Intervention_Altered[2];  /*Training set: conditional probability of Surgical Intervention under altered diagnosis*/
double CP_High_Fever_Normal[3];  /*Training set: conditional probability of High Fever under normal diagnosis*/
double CP_High_Fever_Altered[3];  /*Training set: conditional probability of High Fever  under altered diagnosis*/
double CP_Alcohol_Consumption_Normal[5];  /*Training set: conditional probability of Alcohol Consumption under normal diagnosis*/
double CP_Alcohol_Consumption_Altered[5];  /*Training set: conditional probability of Alcohol Consumption under altered diagnosis*/
double CP_Smoking_Habit_Normal[3];  /*Training set: conditional probability of Smoking Habit under normal diagnosis*/
double CP_Smoking_Habit_Altered[3];  /*Training set: conditional probability of Smoking Habit under altered diagnosis*/

double MEAN_Age_Analysis_Normal = 0.0; /*Training set: mean of Age of Analysis for Gaussian PDF under normal diagnosis*/
double MEAN_Age_Analysis_Altered = 0.0; /*Training set: mean of Age of Analysis for Gaussian PDF under altered diagnosis*/
double VAR_Age_Analysis_Normal= 0.0;/*Training set: variance of Age of Analysis for Gaussian PDF under normal diagnosis*/
double VAR_Age_Analysis_Altered= 0.0; /*Training set: variance of Age of Analysis for Gaussian PDF under altered diagnosis*/


double MEAN_Hour_Sitting_Normal= 0.0;/*Training set: mean of number of hour spent sitting per day for Gaussian PDF under normal diagnosis*/
double MEAN_Hour_Sitting_Altered= 0.0;/*Training set:mean of number of hour spent sitting per day for Gaussian PDF under altered diagnosis*/
double VAR_Hour_Sitting_Normal= 0.0;/*Training set: variance of number of hour spent sitting per day for Gaussian PDF under normal diagnosis*/
double VAR_Hour_Sitting_Altered= 0.0;/*Training set: variance of number of hour spent sitting per day for Gaussian PDF under altered diagnosis*/

double PP_Season_Normal;
double PP_Season_Altered;
double PP_Age_Normal;
double PP_Age_Altered;
double PP_Childish_Disease_Normal;
double PP_Childish_Disease_Altered;
double PP_Accident_Trauma_Normal;
double PP_Accident_Trauma_Altered;
double PP_Surgical_Intervention_Normal;
double PP_Surgical_Intervention_Altered;
double PP_High_Fever_Normal;
double PP_High_Fever_Altered;
double PP_Alcohol_Consumption_Normal;
double PP_Alcohol_Consumption_Altered;
double PP_Somking_Habit_Normal;
double PP_Somking_Habit_Altered;
double PP_Hour_Sitting_Normal;
double PP_Hour_Sitting_Altered;
double PP_Result_Normal[90][1];
double PP_Result_Altered[90][1];
double PP_Predicted_Result[90][1];
double z;

double Training_Error [90][1];
double Testing_Error [50][1];
double Sum_Test_Error = 0;
double Sum_Train_Error = 0;

void main(void)
{
       /* print heading */
       printf("\n Input_priorProbability.c \n");

       /* Input */
       readingFile();

       /* calculate prior probability */
       priorProbability();
       printf("\nBased on the training dataset, P(Y = Normal) prior probability: %f \n", priorProbability_Normal);
       printf("Based on the training dataset, P(Y = Altered) prior probability: %f \n", priorProbability_Altered);

       /* calculate Conditional probability */
       conditionalProbability();

       /* calculate Posterior Probability */
       posteriorProbability();     

}

/**************************************************************/
/* void readingFile(void); */
/* Tay Yi Lin  2103154@sit.singaporetech.edu.sg              */
/*************************************************************/
int readingFile(void)
{
    // read from a text file
    char *filename[100];
    puts("\nEnter the filename to be opened : ");
    gets(filename);
    FILE *fp = fopen(filename, "r");

    if (fp == NULL)
    {
        printf("\nError: Could not open file %s", filename);
        exit(1);
    }
    printf("\nSuccess: Open dataset file - %s \n", filename);

    int row_Count = 0;
    int row_Count2 = 0;
    int col_Count = 0;

    // reading line by line
    const unsigned MAX_LENGTH = 100;
    char buffer[MAX_LENGTH];

    while (fgets(buffer, MAX_LENGTH, fp)) //reads a line from the specified stream and stores it into the string- buffer.
    {
        // extract the first token
        char *token = strtok(buffer, ","); //breaks string-buffer into a series of tokens using the delimiter-",".

        // loop through the string to extract all other tokens
        while (token != NULL)
        {
            col_Count = col_Count % 10;
            // training set
            if (row_Count < 80)
            {
                arrayTraining[row_Count][col_Count] = atof(token);

                // further split training set into feature and output set
                if (col_Count < 9)
                {
                    trainingFeature[row_Count][col_Count] = atof(token);
                }
                else if (col_Count == 9)
                {
                    trainingOutput[row_Count][0] = atof(token);
                }
            }
            // testing set
            if (row_Count >= 80)
            {
                arrayTesting[row_Count2][col_Count] = atof(token);

                // further split testing set into feature and output set
                if (col_Count < 9)
                {
                    testingFeature[row_Count2][col_Count] = atof(token);
                }
                else if (col_Count == 9)
                {
                    testingOutput[row_Count2][0] = atof(token);
                }
            }
            col_Count += 1;
            token = strtok(NULL, ","); //NULL as the first argument, tells the function to continue tokenizing the string you passed in first.
        }

        if (row_Count >= 80)
        {
            row_Count2 += 1;
        }
        row_Count += 1;
    }

    /* close the file */
    fclose(fp);
    return 0;
}

void priorProbability(void)
{
    
    float yAlteredProbability = 0.0;

    for (size_t i = 0; i < 80; i++)
    {
        if (trainingOutput[i][0] == 1.000000)
        {
            yAlteredCounter += 1;
        }
    }
    yNormalCounter = 80 - yAlteredCounter;
   
    yAlteredProbability = (float)yAlteredCounter / (float)80;

    priorProbability_Altered = yAlteredProbability;
    priorProbability_Normal = 1 - priorProbability_Altered;
}


/**************************************************************/
/* void conditionalProbability(void) */
/* Ang Quan Xing 2101536@sit.singaporetech.edu.sg              */
/*************************************************************/
void conditionalProbability(void)
{

/************************************************************************************************************/
/*Find the conditional probability of Season of Analysis under Diagnosis=Normal=0*/
/************************************************************************************************************/
    CP_SeasonAnalysis_Normal[0] = 0; /*under diagnosis=Normal, Winter;*/
    CP_SeasonAnalysis_Normal[1] = 0; /*under diagnosis=Normal, Spring;*/
    CP_SeasonAnalysis_Normal[2] = 0; /*under diagnosis=Normal, Summer;*/
    CP_SeasonAnalysis_Normal[3] = 0;/*under diagnosis=Normal, Fall;*/

    for (size_t i = 0; i < 80; i++)
    {

       if((trainingOutput[i][0] == 0.000000)&&(trainingFeature[i][0] == -1.000000)) /*under diagnosis=Normal, Winter;*/
       {
              CP_SeasonAnalysis_Normal[0] = CP_SeasonAnalysis_Normal[0] + 1;  
       }


       if((trainingOutput[i][0] == 0.000000)&&(trainingFeature[i][0] == -0.330000))/*under diagnosis=Normal, Spring;*/
       {
              CP_SeasonAnalysis_Normal[1]=  CP_SeasonAnalysis_Normal[1] + 1; 
       }

       if((trainingOutput[i][0] == 0.000000)&&(trainingFeature[i][0] == 0.330000))/*under diagnosis=Normal, Summer;*/
       {
               CP_SeasonAnalysis_Normal[2] =  CP_SeasonAnalysis_Normal[2] + 1; 
       }

        if((trainingOutput[i][0] == 0.000000)&&(trainingFeature[i][0] == 1.000000))/*under diagnosis=Normal, Fall;*/
       {
               CP_SeasonAnalysis_Normal[3] =  CP_SeasonAnalysis_Normal[3] + 1; 
       }
             
    }

    printf("\n CP_SeasonAnalysis_Normal [0] = %f",CP_SeasonAnalysis_Normal[0] );
    printf("\n CP_SeasonAnalysis_Normal [1] = %f",CP_SeasonAnalysis_Normal[1] );
    printf("\n CP_SeasonAnalysis_Normal [2] = %f",CP_SeasonAnalysis_Normal[2] );
    printf("\n CP_SeasonAnalysis_Normal [3] = %f",CP_SeasonAnalysis_Normal[3] );
    CP_SeasonAnalysis_Normal[0] /= yNormalCounter;
    CP_SeasonAnalysis_Normal[1] /= yNormalCounter;
    CP_SeasonAnalysis_Normal[2] /= yNormalCounter;
    CP_SeasonAnalysis_Normal[3] /= yNormalCounter;
    printf("\n CP_SeasonAnalysis_Normal [0] = %f",CP_SeasonAnalysis_Normal[0] );
    printf("\n CP_SeasonAnalysis_Normal [1] = %f",CP_SeasonAnalysis_Normal[1] );
    printf("\n CP_SeasonAnalysis_Normal [2] = %f",CP_SeasonAnalysis_Normal[2] );
    printf("\n CP_SeasonAnalysis_Normal [3] = %f",CP_SeasonAnalysis_Normal[3] );
 
   
/************************************************************************************************************/
/*Find the conditional probability of Season of Analysis under Diagnosis=Altered=1*/
/************************************************************************************************************/
    CP_SeasonAnalysis_Altered[0] = 0; /*under diagnosis=Altered, Winter;*/
    CP_SeasonAnalysis_Altered[1] = 0; /*under diagnosis=Altered, Spring;*/
    CP_SeasonAnalysis_Altered[2] = 0; /*under diagnosis=Altered, Summer;*/
    CP_SeasonAnalysis_Altered[3] = 0; /*under diagnosis=Altered, Fall;*/ 

       for (size_t i = 0; i < 80; i++)
       {

       if((trainingOutput[i][0] == 1.000000)&&(trainingFeature[i][0] == -1.000000)) /*under diagnosis=Altered, Winter;*/
       {
              CP_SeasonAnalysis_Altered[0] = CP_SeasonAnalysis_Altered[0] + 1;  
       }


       if((trainingOutput[i][0] == 1.000000)&&(trainingFeature[i][0] == -0.330000))/*under diagnosis=Altered, Spring;*/
       {
              CP_SeasonAnalysis_Altered[1]=  CP_SeasonAnalysis_Altered[1] + 1; 
       }

       if((trainingOutput[i][0] == 1.000000)&&(trainingFeature[i][0] == 0.330000))/*under diagnosis=Altered, Summer;*/
       {
               CP_SeasonAnalysis_Altered[2] =  CP_SeasonAnalysis_Altered[2] + 1; 
       }

        if((trainingOutput[i][0] == 1.000000)&&(trainingFeature[i][0] == 1.000000))/*under diagnosis=Altered, Fall;*/ 
       {
               CP_SeasonAnalysis_Altered[3] =  CP_SeasonAnalysis_Altered[3] + 1; 
       }
             
    }
    printf("\n CP_SeasonAnalysis_Altered [0] = %f",CP_SeasonAnalysis_Altered[0] );
    printf("\n CP_SeasonAnalysis_Altered [1] = %f",CP_SeasonAnalysis_Altered[1] );
    printf("\n CP_SeasonAnalysis_Altered [2] = %f",CP_SeasonAnalysis_Altered[2] );
    printf("\n CP_SeasonAnalysis_Altered [3] = %f",CP_SeasonAnalysis_Altered[3] );
    CP_SeasonAnalysis_Altered[0] /= yAlteredCounter;
    CP_SeasonAnalysis_Altered[1] /= yAlteredCounter;
    CP_SeasonAnalysis_Altered[2] /= yAlteredCounter;
    CP_SeasonAnalysis_Altered[3] /= yAlteredCounter;
    printf("\n After Calculation");
    printf("\n CP_SeasonAnalysis_Altered [0] = %f",CP_SeasonAnalysis_Altered[0] );
    printf("\n CP_SeasonAnalysis_Altered [1] = %f",CP_SeasonAnalysis_Altered[1] );
    printf("\n CP_SeasonAnalysis_Altered [2] = %f",CP_SeasonAnalysis_Altered[2] );
    printf("\n CP_SeasonAnalysis_Altered [3] = %f",CP_SeasonAnalysis_Altered[3] );

/************************************************************************************************************/
/*find the conditional probability (normal gaussian)parameter of Age of Analysis under Diagnosis=normal=0 and Diagnosis=altered=1*/
/************************************************************************************************************/
 
     for (size_t i = 1; i < 80; ++i)
     {
        if(trainingOutput[i][0] == 0.000000)  /*under diagnosis=normal, 0;*/ 
        {
            /*Get total sum of Age of Analysis under diagnosis=normal, 0;*/
            MEAN_Age_Analysis_Normal += trainingFeature[i][1];   
            VAR_Age_Analysis_Normal += trainingFeature[i][1] * trainingFeature[i][1]; 
        }
        else    /*under diagnosis=altered, 1;*/ 
         {
            /*Get total sum of Age of Analysis under diagnosis=altered, 1;*/
            MEAN_Age_Analysis_Altered += trainingFeature[i][1];
            VAR_Age_Analysis_Altered += trainingFeature[i][1] * trainingFeature[i][1];
        }
    }
    //Calculate mean and variance of Age of Analysis under diagnosis=normal, 0;*/
    MEAN_Age_Analysis_Normal = MEAN_Age_Analysis_Normal / yNormalCounter; 
    VAR_Age_Analysis_Normal = (VAR_Age_Analysis_Normal-yNormalCounter*(MEAN_Age_Analysis_Normal*MEAN_Age_Analysis_Normal))/(yNormalCounter-1);

    //Calculate mean and variance of Age of Analysis under diagnosis=altered, 1;*/
    MEAN_Age_Analysis_Altered = MEAN_Age_Analysis_Altered / yAlteredCounter; 
    VAR_Age_Analysis_Altered = (VAR_Age_Analysis_Altered-yAlteredCounter*(MEAN_Age_Analysis_Altered*MEAN_Age_Analysis_Altered))/(yAlteredCounter-1);

/************************************************************************************************************/
/*Find the conditional probability of Child Disease under Diagnosis=normal=0*/
/************************************************************************************************************/
    CP_Childish_Disease_Normal[0] = 0; /*under diagnosis=normal, Yes;*/ 
    CP_Childish_Disease_Normal[1] = 0; /*under diagnosis=normal, No;*/ 
   
     for (size_t i = 0; i < 80; i++)
       {

       if((trainingOutput[i][0] == 0.000000)&&(trainingFeature[i][2] == 0.000000)) /*under diagnosis=normal, Yes;*/ 
       {
              CP_Childish_Disease_Normal[0] = CP_Childish_Disease_Normal[0] + 1;  
       }


       if((trainingOutput[i][0] == 0.000000)&&(trainingFeature[i][2] == 1.000000))/*under diagnosis=normal, No;*/ 
       {
              CP_Childish_Disease_Normal[1]=  CP_Childish_Disease_Normal[1] + 1; 
       }

             
    }
    printf("\n CP_Childish_Disease_Normal [0] = %f",CP_Childish_Disease_Normal[0] );
    printf("\n CP_Childish_Disease_Normal [1] = %f",CP_Childish_Disease_Normal[1] );
    CP_Childish_Disease_Normal[0] /= yNormalCounter;
    CP_Childish_Disease_Normal[1] /= yNormalCounter;
    printf("\n After Calculation");
    printf("\n CP_Childish_Disease_Normal [0] = %f",CP_Childish_Disease_Normal[0] );
    printf("\n CP_Childish_Disease_Normal [1] = %f",CP_Childish_Disease_Normal[1] );
  
   
/************************************************************************************************************/
/*Find the conditional probability of Child Disease under Diagnosis=altered=1*/
/************************************************************************************************************/
    CP_Childish_Disease_Altered[0] = 0.000000; /*under diagnosis=Altered, Yes;*/ 
    CP_Childish_Disease_Altered[1] = 0.000000; /*under diagnosis=Altered, No;*/ 
   
     for (size_t i = 0; i < 80; i++)
       {

       if((trainingOutput[i][0] == 1.000000)&&(trainingFeature[i][2] == 0.000000)) /*under diagnosis=altered, Yes;*/ 
       {
              CP_Childish_Disease_Altered[0] = CP_Childish_Disease_Altered[0] + 1;  
       }


       if((trainingOutput[i][0] == 1.000000)&&(trainingFeature[i][2] == 1.000000))/*under diagnosis=altered, No;*/ 
       {
              CP_Childish_Disease_Altered[1]=  CP_Childish_Disease_Altered[1] + 1; 
       }

             
    }
    printf("\n CP_Childish_Disease_Altered [0] = %f",CP_Childish_Disease_Altered[0] );
    printf("\n CP_Childish_Disease_Altered [1] = %f",CP_Childish_Disease_Altered[1] );
    CP_Childish_Disease_Altered[0] /= yAlteredCounter;
    CP_Childish_Disease_Altered[1] /= yAlteredCounter;
    printf("\n After Calculation");
    printf("\n CP_Childish_Disease_Altered [0] = %f",CP_Childish_Disease_Altered[0] );
    printf("\n CP_Childish_Disease_Altered [1] = %f",CP_Childish_Disease_Altered[1] );
  
/************************************************************************************************************/
/*Find the conditional probability of Accident Trauma under Diagnosis=normal=0*/
/************************************************************************************************************/
    CP_Accident_Trauma_normal[0] = 0.000000; /*under diagnosis=normal, Yes;*/ 
    CP_Accident_Trauma_normal[1] = 0.000000; /*under diagnosis=normal, No;*/ 
   

    for (size_t i = 0; i < 80; i++)
       {

       if((trainingOutput[i][0] == 0.000000)&&(trainingFeature[i][3] == 0.000000)) /*under diagnosis=normal, Yes;*/ 
       {
              CP_Accident_Trauma_normal[0] = CP_Accident_Trauma_normal[0] + 1;  
       }


       if((trainingOutput[i][0] == 0.000000)&&(trainingFeature[i][3] == 1.000000))/*under diagnosis=normal, No;*/ 
       {
              CP_Accident_Trauma_normal[1]=  CP_Accident_Trauma_normal[1] + 1; 
       }

             
    }
    printf("\n CP_Accident_Trauma_normal [0] = %f",CP_Accident_Trauma_normal[0] );
    printf("\n CP_Accident_Trauma_normal [1] = %f",CP_Accident_Trauma_normal[1] );
    CP_Accident_Trauma_normal[0] /= yNormalCounter;
    CP_Accident_Trauma_normal[1] /= yNormalCounter;
    printf("\n CP_Accident_Trauma_normal [0] = %f",CP_Accident_Trauma_normal[0] );
    printf("\n CP_Accident_Trauma_normal [1] = %f",CP_Accident_Trauma_normal[1] );

  
/************************************************************************************************************/
/*Find the conditional probability of Accident Trauma under Diagnosis=altered=1*/
/************************************************************************************************************/
    CP_Accident_Trauma_Altered[0] = 0.000000; /*under diagnosis=altered, Yes;*/ 
    CP_Accident_Trauma_Altered[1] = 0.000000; /*under diagnosis=altered, No;*/ 
   
     for (size_t i = 0; i < 80; i++)
       {

       if((trainingOutput[i][0] == 1.000000)&&(trainingFeature[i][3] == 0.000000)) /*under diagnosis=altered, Yes;*/ 
       {
              CP_Accident_Trauma_Altered[0] = CP_Accident_Trauma_Altered[0] + 1;  
       }


       if((trainingOutput[i][0] == 1.000000)&&(trainingFeature[i][3] == 1.000000))/*under diagnosis=altered, No;*/ 
       {
              CP_Accident_Trauma_Altered[1]=  CP_Accident_Trauma_Altered[1] + 1; 
       }

             
    }

    printf("\n CP_Accident_Trauma_Altered [0] = %f",CP_Accident_Trauma_Altered[0] );
    printf("\n CP_Accident_Trauma_Altered [1] = %f",CP_Accident_Trauma_Altered[1] );
    CP_Accident_Trauma_Altered[0] /= yAlteredCounter;
    CP_Accident_Trauma_Altered[1] /= yAlteredCounter;
    printf("\n CP_Accident_Trauma_Altered [0] = %f",CP_Accident_Trauma_Altered[0] );
    printf("\n CP_Accident_Trauma_Altered [1] = %f",CP_Accident_Trauma_Altered[1] );

  
/************************************************************************************************************/
/*Find the conditional probability of Surgical Intervention under Diagnosis=normal=0*/
/************************************************************************************************************/
    CP_Surgical_Intervention_Normal[0] = 0.000000; /*under diagnosis=normal, Yes;*/ 
    CP_Surgical_Intervention_Normal[1] = 0.000000; /*under diagnosis=normal, No;*/ 
   
    
     for (size_t i = 0; i < 80; i++)
       {

       if((trainingOutput[i][0] == 0.000000)&&(trainingFeature[i][4] == 0.000000)) /*under diagnosis=normal, Yes;*/ 
       {
              CP_Surgical_Intervention_Normal[0] = CP_Surgical_Intervention_Normal[0] + 1;  
       }


       if((trainingOutput[i][0] == 0.000000)&&(trainingFeature[i][4] == 1.000000))/*under diagnosis=normal, No;*/ 
       {
              CP_Surgical_Intervention_Normal[1]=  CP_Surgical_Intervention_Normal[1] + 1; 
       }

             
    }
    printf("\n CP_Surgical_Intervention_Normal [0] = %f",CP_Surgical_Intervention_Normal[0] );
    printf("\n CP_Surgical_Intervention_Normal [1] = %f",CP_Surgical_Intervention_Normal[1] );
    CP_Surgical_Intervention_Normal[0] /= yNormalCounter;
    CP_Surgical_Intervention_Normal[1] /= yNormalCounter;
    printf("\n CP_Surgical_Intervention_Normal [0] = %f",CP_Surgical_Intervention_Normal[0] );
    printf("\n CP_Surgical_Intervention_Normal [1] = %f",CP_Surgical_Intervention_Normal[1] );

/************************************************************************************************************/
/*Find the conditional probability of Surgical Intervention under Diagnosis=altered=1*/
/************************************************************************************************************/
    CP_Surgical_Intervention_Altered[0] = 0.000000; /*under diagnosis=altered, Yes;*/ 
    CP_Surgical_Intervention_Altered[1] = 0.000000; /*under diagnosis=altered, No;*/ 
   
    for (size_t i = 0; i < 80; i++)
       {

       if((trainingOutput[i][0] == 1.000000)&&(trainingFeature[i][4] == 0.000000)) /*under diagnosis=altered, Yes;*/ 
       {
              CP_Surgical_Intervention_Altered[0] = CP_Surgical_Intervention_Altered[0] + 1;  
       }


       if((trainingOutput[i][0] == 1.000000)&&(trainingFeature[i][4] == 1.000000))/*under diagnosis=altered, No;*/ 
       {
              CP_Surgical_Intervention_Altered[1]=  CP_Surgical_Intervention_Altered[1] + 1; 
       }

             
    }
    printf("\n CP_Surgical_Intervention_Altered [0] = %f",CP_Surgical_Intervention_Altered[0] );
    printf("\n CP_Surgical_Intervention_Altered [1] = %f",CP_Surgical_Intervention_Altered[1] );
    CP_Surgical_Intervention_Altered[0] /= yAlteredCounter;
    CP_Surgical_Intervention_Altered[1] /= yAlteredCounter;
    printf("\n CP_Surgical_Intervention_Altered [0] = %f",CP_Surgical_Intervention_Altered[0] );
    printf("\n CP_Surgical_Intervention_Altered [1] = %f",CP_Surgical_Intervention_Altered[1] );

/************************************************************************************************************/
/*Find the conditional probability of High Fever under Diagnosis=normal=0*/
/************************************************************************************************************/
    CP_High_Fever_Normal[0] = 0.000000; /*under diagnosis=normal, Less than 3 months ago;*/ 
    CP_High_Fever_Normal[1] = 0.000000; /*under diagnosis=normal, More than 3 months ago;*/ 
    CP_High_Fever_Normal[2] = 0.000000; /*under diagnosis=normal, No;*/ 


    for (size_t i = 0; i < 80; i++)
       {

       if((trainingOutput[i][0] == 0.000000)&&(trainingFeature[i][5] == -1.000000)) /*under diagnosis=normal, Less than 3 months ago;*/ 
       {
              CP_High_Fever_Normal[0] = CP_High_Fever_Normal[0] + 1;  
       }


       if((trainingOutput[i][0] == 0.000000)&&(trainingFeature[i][5] == 0.000000))/*under diagnosis=normal, More than 3 months ago;*/ 
       {
              CP_High_Fever_Normal[1]=  CP_High_Fever_Normal[1] + 1; 
       }

       if((trainingOutput[i][0] == 0.000000)&& (trainingFeature[i][5] == 1.000000))/*under diagnosis=normal, No;*/ 
       {
              CP_High_Fever_Normal[2]=  CP_High_Fever_Normal[2] + 1; 
       }

             
    }
    printf("\n CP_High_Fever_Normal [0] = %f",CP_High_Fever_Normal[0] );
    printf("\n CP_High_Fever_Normal [1] = %f",CP_High_Fever_Normal[1] );
    printf("\n CP_High_Fever_Normal [2] = %f",CP_High_Fever_Normal[2] );
    CP_High_Fever_Normal[0] /= yNormalCounter;
    CP_High_Fever_Normal[1] /= yNormalCounter;
    CP_High_Fever_Normal[2] /= yNormalCounter;
    printf("\n CP_High_Fever_Normal [0] = %f",CP_High_Fever_Normal[0] );
    printf("\n CP_High_Fever_Normal [1] = %f",CP_High_Fever_Normal[1] );
    printf("\n CP_High_Fever_Normal [2] = %f",CP_High_Fever_Normal[2] );


/************************************************************************************************************/
/*Find the conditional probability of High Fever under Diagnosis=altered=1*/
/************************************************************************************************************/
    CP_High_Fever_Altered[0] = 0.000000; /*under diagnosis=altered, Less than 3 months ago;*/ 
    CP_High_Fever_Altered[1] = 0.000000; /*under diagnosis=altered, More than 3 months ago;*/ 
    CP_High_Fever_Altered[2] = 0.000000; /*under diagnosis=altered, No;*/ 

   
    for (size_t i = 0; i < 80; i++)
       {

       if((trainingOutput[i][0] == 1.000000)&&(trainingFeature[i][5] == -1.000000)) /*under diagnosis=altered, Less than 3 months ago;*/ 
       {
              CP_High_Fever_Altered[0] = CP_High_Fever_Altered[0] + 1;  
       }


       if((trainingOutput[i][0] == 1.000000)&&(trainingFeature[i][5] == 0.000000))/*under diagnosis=altered, More than 3 months ago;*/ 
       {
              CP_High_Fever_Altered[1]=  CP_High_Fever_Altered[1] + 1; 
       }

       if((trainingOutput[i][0] == 1.000000)&& (trainingFeature[i][5] == 1.000000)) /*under diagnosis=altered, No;*/ 
       {
              CP_High_Fever_Altered[2]=  CP_High_Fever_Altered[2] + 1; 
       }

             
    }
    printf("\n CP_High_Fever_Altered [0] = %f",CP_High_Fever_Altered[0] );
    printf("\n CP_High_Fever_Altered [1] = %f",CP_High_Fever_Altered[1] );
    printf("\n CP_High_Fever_Altered [2] = %f",CP_High_Fever_Altered[2] );
    CP_High_Fever_Altered[0] /= yAlteredCounter;
    CP_High_Fever_Altered[1] /= yAlteredCounter;
    CP_High_Fever_Altered[2] /= yAlteredCounter;
    printf("\n CP_High_Fever_Altered [0] = %f",CP_High_Fever_Altered[0] );
    printf("\n CP_High_Fever_Altered [1] = %f",CP_High_Fever_Altered[1] );
    printf("\n CP_High_Fever_Altered [2] = %f",CP_High_Fever_Altered[2] );

/************************************************************************************************************/
/*Find the conditional probability of Alcohol Consumption Fever under Diagnosis=normal=0*/
/************************************************************************************************************/
    CP_Alcohol_Consumption_Normal[0] = 0.000000; /*under diagnosis=normal, Several time a day;*/ 
    CP_Alcohol_Consumption_Normal[1] = 0.000000; /*under diagnosis=normal, Every day;*/ 
    CP_Alcohol_Consumption_Normal[2] = 0.000000; /*under diagnosis=normal, Several time a week;*/ 
    CP_Alcohol_Consumption_Normal[3] = 0.000000; /*under diagnosis=normal, Once a week;*/ 
    CP_Alcohol_Consumption_Normal[4] = 0.000000; /*under diagnosis=normal, Hardly ever or never;*/ 


      for (size_t i = 0; i < 80; i++)
       {

       if((trainingOutput[i][0] == 0.000000)&&(trainingFeature[i][6] == 0.200000)) /*under diagnosis=normal, Several time a day;*/ 
       {
              CP_Alcohol_Consumption_Normal[0] = CP_Alcohol_Consumption_Normal[0] + 1;  
       }


       if((trainingOutput[i][0] == 0.000000)&&(trainingFeature[i][6] == 0.400000))/*under diagnosis=normal, Every day;*/ 
       {
              CP_Alcohol_Consumption_Normal[1]=  CP_Alcohol_Consumption_Normal[1] + 1; 
       }

       if((trainingOutput[i][0] == 0.000000)&& (trainingFeature[i][6] == 0.600000)) /*under diagnosis=normal, Several time a week;*/ 
       {
              CP_Alcohol_Consumption_Normal[2]=  CP_Alcohol_Consumption_Normal[2] + 1; 
       }
       if((trainingOutput[i][0] == 0.000000)&&(trainingFeature[i][6] == 0.800000)) /*under diagnosis=normal, Once a week;*/ 
       {
              CP_Alcohol_Consumption_Normal[3]=  CP_Alcohol_Consumption_Normal[3] + 1; 
       }
       if((trainingOutput[i][0] == 0.000000)&& (trainingFeature[i][6] == 1.000000))  /*under diagnosis=normal, Hardly ever or never;*/  
       {
              CP_Alcohol_Consumption_Normal[4]=  CP_Alcohol_Consumption_Normal[4] + 1; 
       }

             
    }
    printf("\n CP_Alcohol_Consumption_Normal [0] = %f",CP_Alcohol_Consumption_Normal[0] );
    printf("\n CP_Alcohol_Consumption_Normal [1] = %f",CP_Alcohol_Consumption_Normal[1] );
    printf("\n CP_Alcohol_Consumption_Normal [2] = %f",CP_Alcohol_Consumption_Normal[2] );
    printf("\n CP_Alcohol_Consumption_Normal [3] = %f",CP_Alcohol_Consumption_Normal[3] );
    printf("\n CP_Alcohol_Consumption_Normal [4] = %f",CP_Alcohol_Consumption_Normal[4] );

    CP_Alcohol_Consumption_Normal[0] /= yNormalCounter;
    CP_Alcohol_Consumption_Normal[1] /= yNormalCounter;
    CP_Alcohol_Consumption_Normal[2] /= yNormalCounter;
    CP_Alcohol_Consumption_Normal[3] /= yNormalCounter;
    CP_Alcohol_Consumption_Normal[4] /= yNormalCounter;

    printf("\n CP_Alcohol_Consumption_Normal [0] = %f",CP_Alcohol_Consumption_Normal[0] );
    printf("\n CP_Alcohol_Consumption_Normal [1] = %f",CP_Alcohol_Consumption_Normal[1] );
    printf("\n CP_Alcohol_Consumption_Normal [2] = %f",CP_Alcohol_Consumption_Normal[2] );
    printf("\n CP_Alcohol_Consumption_Normal [3] = %f",CP_Alcohol_Consumption_Normal[3] );
    printf("\n CP_Alcohol_Consumption_Normal [4] = %f",CP_Alcohol_Consumption_Normal[4] );


/************************************************************************************************************/
/*Find the conditional probability of Alcohol Consumption Fever under Diagnosis=altered=1*/
/************************************************************************************************************/
    CP_Alcohol_Consumption_Altered[0] = 0.000000; /*under diagnosis=altered, Several time a day;*/ 
    CP_Alcohol_Consumption_Altered[1] = 0.000000; /*under diagnosis=altered, Every day;*/ 
    CP_Alcohol_Consumption_Altered[2] = 0.000000; /*under diagnosis=altered, Several time a week;*/ 
    CP_Alcohol_Consumption_Altered[3] = 0.000000; /*under diagnosis=altered, Once a week;*/ 
    CP_Alcohol_Consumption_Altered[4] = 0.000000; /*under diagnosis=altered, Hardly ever or never;*/ 

   
    for (size_t i = 0; i < 80; i++)
       {

       if((trainingOutput[i][0] == 1.000000)&&(trainingFeature[i][6] == 0.200000)) /*under diagnosis=altered, Several time a day;*/ 
       {
              CP_Alcohol_Consumption_Altered[0] = CP_Alcohol_Consumption_Altered[0] + 1;  
       }


       if((trainingOutput[i][0] == 1.000000)&&(trainingFeature[i][6] == 0.400000))/*under diagnosis=altered, Every day;*/ 
       {
              CP_Alcohol_Consumption_Altered[1]=  CP_Alcohol_Consumption_Altered[1] + 1; 
       }

       if((trainingOutput[i][0] == 1.000000)&& (trainingFeature[i][6] == 0.600000)) /*under diagnosis=altered, Several time a week;*/ 
       {
              CP_Alcohol_Consumption_Altered[2]=  CP_Alcohol_Consumption_Altered[2] + 1; 
       }
       if((trainingOutput[i][0] == 1.000000)&&(trainingFeature[i][6] == 0.800000)) /*under diagnosis=altered, Once a week;*/ 
       {
              CP_Alcohol_Consumption_Altered[3]=  CP_Alcohol_Consumption_Altered[3] + 1; 
       }
       if((trainingOutput[i][0] == 1.000000)&& (trainingFeature[i][6] == 1.000000))  /*under diagnosis=altered, Hardly ever or never;*/  
       {
              CP_Alcohol_Consumption_Altered[4]=  CP_Alcohol_Consumption_Altered[4] + 1; 
       }

             
    }
    printf("\n CP_Alcohol_Consumption_Altered [0] = %f",CP_Alcohol_Consumption_Altered[0] );
    printf("\n CP_Alcohol_Consumption_Altered [1] = %f",CP_Alcohol_Consumption_Altered[1] );
    printf("\n CP_Alcohol_Consumption_Altered [2] = %f",CP_Alcohol_Consumption_Altered[2] );
    printf("\n CP_Alcohol_Consumption_Altered [3] = %f",CP_Alcohol_Consumption_Altered[3] );
    printf("\n CP_Alcohol_Consumption_Altered [4] = %f",CP_Alcohol_Consumption_Altered[4] );
    CP_Alcohol_Consumption_Altered[0] /= yAlteredCounter;
    CP_Alcohol_Consumption_Altered[1] /= yAlteredCounter;
    CP_Alcohol_Consumption_Altered[2] /= yAlteredCounter;
    CP_Alcohol_Consumption_Altered[3] /= yAlteredCounter;
    CP_Alcohol_Consumption_Altered[4] /= yAlteredCounter;
    printf("\n CP_Alcohol_Consumption_Altered [0] = %f",CP_Alcohol_Consumption_Altered[0] );
    printf("\n CP_Alcohol_Consumption_Altered [1] = %f",CP_Alcohol_Consumption_Altered[1] );
    printf("\n CP_Alcohol_Consumption_Altered [2] = %f",CP_Alcohol_Consumption_Altered[2] );
    printf("\n CP_Alcohol_Consumption_Altered [3] = %f",CP_Alcohol_Consumption_Altered[3] );
    printf("\n CP_Alcohol_Consumption_Altered [4] = %f",CP_Alcohol_Consumption_Altered[4] );

/************************************************************************************************************/
/*Find the conditional probability of Smoking Habit Fever under Diagnosis=normal=0*/
/************************************************************************************************************/
    CP_Smoking_Habit_Normal[0] = 0.000000; /*under diagnosis=normal, Never;*/ 
    CP_Smoking_Habit_Normal[1] = 0.000000; /*under diagnosis=normal, Occasional;*/ 
    CP_Smoking_Habit_Normal[2] = 0.000000; /*under diagnosis=normal, Daily;*/ 
   
   
     for (size_t i = 0; i < 80; i++)
       {

       if((trainingOutput[i][0] == 0.000000)&&(trainingFeature[i][7] == -1.000000)) /*under diagnosis=normal, Never;*/ 
       {
              CP_Smoking_Habit_Normal[0] = CP_Smoking_Habit_Normal[0] + 1;  
       }


       if((trainingOutput[i][0] == 0.000000)&&(trainingFeature[i][7] == 0.000000))/*under diagnosis=normal, Occasional;*/ 
       {
              CP_Smoking_Habit_Normal[1]=  CP_Smoking_Habit_Normal[1] + 1; 
       }

       if((trainingOutput[i][0] == 0.000000)&& (trainingFeature[i][7] == 1.000000)) /*under diagnosis=normal, Daily;*/ 
       {
              CP_Smoking_Habit_Normal[2]=  CP_Smoking_Habit_Normal[2] + 1; 
       }
      
    }

    printf("\n CP_Smoking_Habit_Normal [0] = %f",CP_Smoking_Habit_Normal[0] );
    printf("\n CP_Smoking_Habit_Normal [1] = %f",CP_Smoking_Habit_Normal[1] );
    printf("\n CP_Smoking_Habit_Normal [2] = %f",CP_Smoking_Habit_Normal[2] );
    CP_Smoking_Habit_Normal[0] /= yNormalCounter;
    CP_Smoking_Habit_Normal[1] /= yNormalCounter;
    CP_Smoking_Habit_Normal[2] /= yNormalCounter;
    printf("\n CP_Smoking_Habit_Normal [0] = %f",CP_Smoking_Habit_Normal[0] );
    printf("\n CP_Smoking_Habit_Normal [1] = %f",CP_Smoking_Habit_Normal[1] );
    printf("\n CP_Smoking_Habit_Normal [2] = %f",CP_Smoking_Habit_Normal[2] );
   
/************************************************************************************************************/
/*Find the conditional probability of Smoking Habit Fever under Diagnosis=altered=1*/
/************************************************************************************************************/
    CP_Smoking_Habit_Altered[0] = 0.000000; /*under diagnosis=altered, Never;*/ 
    CP_Smoking_Habit_Altered[1] = 0.000000; /*under diagnosis=altered, Occasional;*/ 
    CP_Smoking_Habit_Altered[2] = 0.000000; /*under diagnosis=altered, Daily;*/ 
   
   
     for (size_t i = 0; i < 80; i++)
       {

       if((trainingOutput[i][0] == 1.000000)&&(trainingFeature[i][7] == -1.000000)) /*under diagnosis=altered, Never;*/ 
       {
              CP_Smoking_Habit_Altered[0] = CP_Smoking_Habit_Altered[0] + 1;  
       }


       if((trainingOutput[i][0] == 1.000000)&&(trainingFeature[i][7] == 0.000000))/*under diagnosis=altered, Occasional;*/ 
       {
              CP_Smoking_Habit_Altered[1]=  CP_Smoking_Habit_Altered[1] + 1; 
       }

       if((trainingOutput[i][0] == 1.000000)&& (trainingFeature[i][7] == 1.000000)) /*under diagnosis=altered, Daily;*/ 
       {
              CP_Smoking_Habit_Altered[2]=  CP_Smoking_Habit_Altered[2] + 1; 
       }
      
    }
    printf("\n CP_Smoking_Habit_Altered [0] = %f",CP_Smoking_Habit_Altered[0] );
    printf("\n CP_Smoking_Habit_Altered [1] = %f",CP_Smoking_Habit_Altered[1] );
    printf("\n CP_Smoking_Habit_Altered [2] = %f",CP_Smoking_Habit_Altered[2] );
    CP_Smoking_Habit_Altered[0] /= yAlteredCounter;
    CP_Smoking_Habit_Altered[1] /= yAlteredCounter;
    CP_Smoking_Habit_Altered[2] /= yAlteredCounter;
    printf("\n CP_Smoking_Habit_Altered [0] = %f",CP_Smoking_Habit_Altered[0] );
    printf("\n CP_Smoking_Habit_Altered [1] = %f",CP_Smoking_Habit_Altered[1] );
    printf("\n CP_Smoking_Habit_Altered [2] = %f",CP_Smoking_Habit_Altered[2] );
   

/***************************************************************************************************************************************************/
/*find the conditional probability (normal gaussian)parameter of number of hour spent sitting per day under Diagnosis=normal=0 and Diagnosis=altered=1*/
/***************************************************************************************************************************************************/
 
     for (size_t i = 1; i < 80; ++i)
     {
        if(trainingOutput[i][0] == 0.000000)  /*under diagnosis=normal, 0;*/ 
        {
            /*Get total sum of number of hour spent sitting per day under diagnosis=normal, 0;*/
            MEAN_Hour_Sitting_Normal += trainingFeature[i][8];   
            VAR_Hour_Sitting_Normal += trainingFeature[i][8] * trainingFeature[i][8]; 
        }
        else    /*under diagnosis=altered, 1;*/ 
         {
            /*Get total sum of number of hour spent sitting per day under diagnosis=altered, 1;*/
            MEAN_Hour_Sitting_Altered += trainingFeature[i][8];
            VAR_Hour_Sitting_Altered += trainingFeature[i][8] * trainingFeature[i][8];
        }
    }
    //Calculate mean and variance of number of hour spent sitting per day under diagnosis=normal, 0;*/
    MEAN_Hour_Sitting_Normal = MEAN_Hour_Sitting_Normal / yNormalCounter; 
    VAR_Hour_Sitting_Normal = (VAR_Hour_Sitting_Normal-yNormalCounter*(MEAN_Hour_Sitting_Normal*MEAN_Hour_Sitting_Normal))/(yNormalCounter-1);

    //Calculate mean and variance of number of hour spent sitting per day under diagnosis=altered, 1;*/
    MEAN_Hour_Sitting_Altered = MEAN_Hour_Sitting_Altered / yAlteredCounter; 
    VAR_Hour_Sitting_Altered = (VAR_Hour_Sitting_Altered-yAlteredCounter*(MEAN_Hour_Sitting_Altered*MEAN_Hour_Sitting_Altered))/(yAlteredCounter-1);


}


/**************************************************************/
/* void posteriorProbability(void) */
/* Kenny Lim Ye Wei 2102764@sit.singaporetech.edu.sg              */
/*************************************************************/
void posteriorProbability(void){

       for(size_t i = 0; i < Training_Row; i++){

              // Using training data
              // Getting CP of Season of analysis
              if (trainingFeature[i][0] == -1)   // Winter
              {
                     PP_Season_Normal = CP_SeasonAnalysis_Normal[0];
                     PP_Season_Altered = CP_SeasonAnalysis_Altered[0];
              }else if(trainingFeature[i][0] == -0.33)  // Spring 
              {
                     PP_Season_Normal = CP_SeasonAnalysis_Normal[1];
                     PP_Season_Altered = CP_SeasonAnalysis_Altered[1];
              }else if(trainingFeature[i][0] == 0.33)   // Summer
              {
                     PP_Season_Normal = CP_SeasonAnalysis_Normal[2];
                     PP_Season_Altered = CP_SeasonAnalysis_Altered[2];
              }else  // Fall
              {
                     PP_Season_Normal = CP_SeasonAnalysis_Normal[3];
                     PP_Season_Altered = CP_SeasonAnalysis_Altered[3];
              }

              // Getting CP of Age of analysis
              z = (trainingFeature[i][1]-MEAN_Age_Analysis_Normal)/(VAR_Age_Analysis_Normal);
              PP_Age_Normal = (1/sqrt(2*PI))*exp(-0.5*pow(z,2));
              z = (trainingFeature[i][1]-MEAN_Age_Analysis_Altered)/(VAR_Age_Analysis_Altered);
              PP_Age_Altered = (1/sqrt(2*PI))*exp(-0.5*pow(z,2));
             
              // Getting CP of Childish Disease
              if (trainingFeature[i][2] == 0)   // Yes
              {
                     PP_Childish_Disease_Normal = CP_Childish_Disease_Normal[0];
                     PP_Childish_Disease_Altered = CP_Childish_Disease_Normal[0];
              }else  // No
              {
                     PP_Childish_Disease_Normal = CP_Childish_Disease_Altered[1];
                     PP_Childish_Disease_Altered = CP_Childish_Disease_Altered[1];
              }

              // Getting CP of Accident or serious trauma
              if (trainingFeature[i][3] == 0)   // Yes
              {
                     PP_Accident_Trauma_Normal = CP_Accident_Trauma_normal[0];
                     PP_Accident_Trauma_Altered = CP_Accident_Trauma_Altered[0];
              }else  // No
              {
                     PP_Accident_Trauma_Normal = CP_Accident_Trauma_normal[1];
                     PP_Accident_Trauma_Altered = CP_Accident_Trauma_Altered[1];
              }
              
              // Getting CP of Surgical Intervention
              if (trainingFeature[i][4] == 0)   // Yes
              {
                     PP_Surgical_Intervention_Normal = CP_Surgical_Intervention_Normal[0];
                     PP_Surgical_Intervention_Altered = CP_Surgical_Intervention_Altered[0];
              }else  // No
              {
                     PP_Surgical_Intervention_Normal = CP_Surgical_Intervention_Normal[1];
                     PP_Surgical_Intervention_Altered = CP_Surgical_Intervention_Altered[1];
              }

              // Getting CP of High fevers in last years
              if (trainingFeature[i][5] == -1)   // Less than three moths ago
              {
                     PP_High_Fever_Normal = CP_High_Fever_Normal[0];
                     PP_High_Fever_Altered = CP_High_Fever_Altered[0];
              }else if(trainingFeature[i][5] == 0)  // More than three moths ago
              {
                     PP_High_Fever_Normal = CP_High_Fever_Normal[1];
                     PP_High_Fever_Altered = CP_High_Fever_Altered[1];
              }else  // No
              {
                     PP_High_Fever_Normal = CP_High_Fever_Normal[2];
                     PP_High_Fever_Altered = CP_High_Fever_Altered[2];
              }

              // Getting CP of Frequency of alcohol consumption
              if (trainingFeature[i][6] == 0.2)   // Several times a day
              {
                     PP_Alcohol_Consumption_Normal = CP_Alcohol_Consumption_Normal[0];
                     PP_Alcohol_Consumption_Altered = CP_Alcohol_Consumption_Altered[0];
              }else if(trainingFeature[i][6] == 0.4)  // Every day
              {
              }else if(trainingFeature[i][6] == 0.6)  // Several times a week
              {

              }else if(trainingFeature[i][6] == 0.8)  // Once a week
              {
              }else  // Hardly ever or never
              {
                     PP_Alcohol_Consumption_Normal = CP_Alcohol_Consumption_Normal[1];
                     PP_Alcohol_Consumption_Altered = CP_Alcohol_Consumption_Altered[1];
              }

              // Getting CP of Somking habit 
              if (trainingFeature[i][7] == -1)   // Never
              {
                     PP_Somking_Habit_Normal = CP_Smoking_Habit_Normal[0];
                     PP_Somking_Habit_Altered = CP_Smoking_Habit_Altered[0];
              }else if(trainingFeature[i][7] == 0) // Occasional
              {
                     PP_Somking_Habit_Normal = CP_Smoking_Habit_Normal[1];
                     PP_Somking_Habit_Altered = CP_Smoking_Habit_Altered[1];
              }else  // Daily
              {
                     PP_Somking_Habit_Normal = CP_Smoking_Habit_Normal[2];
                     PP_Somking_Habit_Altered = CP_Smoking_Habit_Altered[2];
              }

              // Getting CP of Number of hours spent sitting per day
              z = (trainingFeature[i][8]-MEAN_Hour_Sitting_Normal)/(VAR_Hour_Sitting_Normal);
              PP_Hour_Sitting_Normal = 1/sqrt(2*PI)*exp(-0.5*pow(z,2));

              z = (trainingFeature[i][8]-MEAN_Hour_Sitting_Altered)/(VAR_Hour_Sitting_Altered);
              PP_Hour_Sitting_Altered= 1/sqrt(2*PI)*exp(-0.5*pow(z,2));

              
              PP_Result_Normal[i][0] = PP_Season_Normal * PP_Age_Normal * PP_Childish_Disease_Normal 
                                   * PP_Accident_Trauma_Normal * PP_Surgical_Intervention_Normal * PP_High_Fever_Normal 
                                   * PP_Alcohol_Consumption_Normal * PP_Somking_Habit_Normal * PP_Hour_Sitting_Normal * priorProbability_Normal;
                                    
              PP_Result_Altered[i][0] = PP_Season_Altered * PP_Age_Altered * PP_Childish_Disease_Altered 
                                   * PP_Accident_Trauma_Altered * PP_Surgical_Intervention_Altered * PP_High_Fever_Altered 
                                   * PP_Alcohol_Consumption_Altered * PP_Somking_Habit_Altered * PP_Hour_Sitting_Altered * priorProbability_Altered;
              
              // printf("\nPP_Training_Result_Normal[%d] :%f",(i+1) ,PP_Result_Normal[i][0]);
              // printf("\nPP_Training_Result_Altered[%d] :%f",(i+1) ,PP_Result_Altered[i][0]);

              if(PP_Result_Normal[i][0] >= PP_Result_Altered[i][0]){
                     PP_Predicted_Result[i][0] = 0;
              }else{
                     PP_Predicted_Result[i][0] = 1;
              }
              if(PP_Predicted_Result[i][0] != trainingOutput[i][0]){
                     Training_Error[i][0] = 1;
              } 
                                
       }
       
       // Using testing data
       for(size_t i = 0; i < Testing_Row; i++){
              
              // Getting CP of Season of analysis
              if (testingFeature[i][0] == -1)   // Winter
              {
                     PP_Season_Normal = CP_SeasonAnalysis_Normal[0];
                     PP_Season_Altered = CP_SeasonAnalysis_Altered[0];
              }else if(testingFeature[i][0] == -0.33)  // Spring 
              {
                     PP_Season_Normal = CP_SeasonAnalysis_Normal[1];
                     PP_Season_Altered = CP_SeasonAnalysis_Altered[1];
              }else if(testingFeature[i][0] == 0.33)   // Summer
              {
                     PP_Season_Normal = CP_SeasonAnalysis_Normal[2];
                     PP_Season_Altered = CP_SeasonAnalysis_Altered[2];
              }else  // Fall
              {
                     PP_Season_Normal = CP_SeasonAnalysis_Normal[3];
                     PP_Season_Altered = CP_SeasonAnalysis_Altered[3];
              }

              // Getting CP of Age of analysis
              z = (testingFeature[i][1]-MEAN_Age_Analysis_Normal)/(VAR_Age_Analysis_Normal);
              PP_Age_Normal = 1/sqrt(2*PI)*exp(-0.5*pow(z,2));

              z = (testingFeature[i][1]-MEAN_Age_Analysis_Altered)/(VAR_Age_Analysis_Altered);
              PP_Age_Altered = 1/sqrt(2*PI)*exp(-0.5*pow(z,2));

              // Getting CP of Childish Disease
              if (testingFeature[i][2] == 0)   // Yes
              {
                     PP_Childish_Disease_Normal = CP_Childish_Disease_Normal[0];
                     PP_Childish_Disease_Altered = CP_Childish_Disease_Normal[0];
              }else  // No
              {
                     PP_Childish_Disease_Normal = CP_Childish_Disease_Altered[1];
                     PP_Childish_Disease_Altered = CP_Childish_Disease_Altered[1];
              }

              // Getting CP of Accident or serious trauma
              if (testingFeature[i][3] == 0)   // Yes
              {
                     PP_Accident_Trauma_Normal = CP_Accident_Trauma_normal[0];
                     PP_Accident_Trauma_Altered = CP_Accident_Trauma_Altered[0];
              }else  // No
              {
                     PP_Accident_Trauma_Normal = CP_Accident_Trauma_normal[1];
                     PP_Accident_Trauma_Altered = CP_Accident_Trauma_Altered[1];
              }
              
              // Getting CP of Surgical Intervention
              if (testingFeature[i][4] == 0)   // Yes
              {
                     PP_Surgical_Intervention_Normal = CP_Surgical_Intervention_Normal[0];
                     PP_Surgical_Intervention_Altered = CP_Surgical_Intervention_Altered[0];
              }else  // No
              {
                     PP_Surgical_Intervention_Normal = CP_Surgical_Intervention_Normal[1];
                     PP_Surgical_Intervention_Altered = CP_Surgical_Intervention_Altered[1];
              }

              // Getting CP of High fevers in last years
              if (testingFeature[i][5] == -1)   // Less than three moths ago
              {
                     PP_High_Fever_Normal = CP_High_Fever_Normal[0];
                     PP_High_Fever_Altered = CP_High_Fever_Altered[0];
              }else if(testingFeature[i][5] == 0)  // More than three moths ago
              {
                     PP_High_Fever_Normal = CP_High_Fever_Normal[1];
                     PP_High_Fever_Altered = CP_High_Fever_Altered[1];
              }else  // No
              {
                     PP_High_Fever_Normal = CP_High_Fever_Normal[2];
                     PP_High_Fever_Altered = CP_High_Fever_Altered[2];
              }

              // Getting CP of Frequency of alcohol consumption
              if (testingFeature[i][6] == 0.2)   // Several times a day
              {
                     PP_Alcohol_Consumption_Normal = CP_Alcohol_Consumption_Normal[0];
                     PP_Alcohol_Consumption_Altered = CP_Alcohol_Consumption_Altered[0];
              }else if(testingFeature[i][6] == 0.4)  // Every day
              {
              }else if(testingFeature[i][6] == 0.6)  // Several times a week
              {

              }else if(testingFeature[i][6] == 0.8)  // Once a week
              {
              }else  // Hardly ever or never
              {
                     PP_Alcohol_Consumption_Normal = CP_Alcohol_Consumption_Normal[1];
                     PP_Alcohol_Consumption_Altered = CP_Alcohol_Consumption_Altered[1];
              }

              // Getting CP of Somking habit 
              if (testingFeature[i][7] == -1)   // Never
              {
                     PP_Somking_Habit_Normal = CP_Smoking_Habit_Normal[0];
                     PP_Somking_Habit_Altered = CP_Smoking_Habit_Altered[0];
              }else if(testingFeature[i][7] == 0) // Occasional
              {
                     PP_Somking_Habit_Normal = CP_Smoking_Habit_Normal[1];
                     PP_Somking_Habit_Altered = CP_Smoking_Habit_Altered[1];
              }else  // Daily
              {
                     PP_Somking_Habit_Normal = CP_Smoking_Habit_Normal[2];
                     PP_Somking_Habit_Altered = CP_Smoking_Habit_Altered[2];
              }

              // Getting CP of Number of hours spent sitting per day
              z = (testingFeature[i][8]-MEAN_Hour_Sitting_Normal)/(VAR_Hour_Sitting_Normal);
              PP_Hour_Sitting_Normal = 1/sqrt(2*PI)*exp(-0.5*pow(z,2));

              z = (testingFeature[i][8]-MEAN_Hour_Sitting_Altered)/(VAR_Hour_Sitting_Altered);
              PP_Hour_Sitting_Altered= 1/sqrt(2*PI)*exp(-0.5*pow(z,2));

              PP_Result_Normal[i][0] = PP_Season_Normal * PP_Age_Normal * PP_Childish_Disease_Normal 
                                   * PP_Accident_Trauma_Normal * PP_Surgical_Intervention_Normal * PP_High_Fever_Normal 
                                   * PP_Alcohol_Consumption_Normal * PP_Somking_Habit_Normal * PP_Hour_Sitting_Normal * priorProbability_Normal;
                                    
              PP_Result_Altered[i][0] = PP_Season_Altered * PP_Age_Altered * PP_Childish_Disease_Altered 
                                   * PP_Accident_Trauma_Altered * PP_Surgical_Intervention_Altered * PP_High_Fever_Altered 
                                   * PP_Alcohol_Consumption_Altered * PP_Somking_Habit_Altered * PP_Hour_Sitting_Altered * priorProbability_Altered;
              
              // printf("\nPP_Testing_Result_Normal[%d] :%f",(i+1) ,PP_Result_Normal[i][0]);
              // printf("\nPP_Testing_Result_Altered[%d] :%f",(i+1) ,PP_Result_Altered[i][0]);

              if(PP_Result_Normal[i][0] >= PP_Result_Altered[i][0]){
                     PP_Predicted_Result[i][0] = 0;
              }else{
                     PP_Predicted_Result[i][0] = 1;
              }
              if(PP_Predicted_Result[i][0] != testingOutput[i][0]){
                     Testing_Error[i][0] = 1;
              }   
       }

       Sum_Test_Error = 0.0;
       Sum_Train_Error = 0.0;
       for(size_t i = 0; i < Training_Row; i++){
              
              Sum_Train_Error += Training_Error[i][0];
              //printf("\nSum_Train_Error[%d]: %f",i, Sum_Train_Error);
       }
       for(size_t i = 0; i < Testing_Row; i++){
              Sum_Test_Error += Testing_Error[i][0];
              //printf("\nSum_Test_Error[%d]: %f",i, Sum_Test_Error);
       }

       Sum_Test_Error/=Testing_Row;
       Sum_Train_Error/=Training_Row;
       printf("\nTraining Error for %d rows :%f", Training_Row,Sum_Train_Error);
       printf("\nTesting Error for %d rows :%f", Testing_Row, Sum_Test_Error);
}
 
