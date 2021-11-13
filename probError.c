/**************************************************************/
/* Input_priorProbability.c */
/* Tay Yi Lin  2103154@sit.singaporetech.edu.sg              */
/*************************************************************/
//Count the number of error for Testing dataset
errorProbability_=0;
for (i=0;i< 20;i++)
{
    errorProbability+=errortest[i][0];
}
errorProbability=errorProbability/20;
printf("\nTesting error =%lf",errorProbability);

//Count the number of error for Training dataset
errorProbability_ = 0;
for (i = 0; i < 80; i++)
{
    errorProbability += errortrain[i][0];
}
errorProbability = errorProbability/80;
//Confusion matrix
printf("\nTraining error =%lf", errorper);

printf("\n________________________________________\n") ;
printf(" Confusion Matrix - Testing dataset");
printf("\n_________________________________________\n") ;
printf("                     Actual Positive            Actual Negative ");
printf(" Predicted Positive      %d(True Positive)                           %d(False Negative)      ");
printf(" Predicted Negative      %d(False Positive)                           %d(True Negative)        ");

int true_Positive //No of times predict correctly that patient is not normal
int  //No of times predict wrongly that patient is not normal butpatient is actually normal
int false_Negative //No of times predict wrongly that patient is normal but patient is actually not normal
int true_Negative //No of times predict correctly that patient is normal
