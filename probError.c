/**************************************************************/
/* Input_priorProbability.c */
/* Tay Yi Lin  2103154@sit.singaporetech.edu.sg              */
/*************************************************************/
//Count the number of error for Testing dataset
errorProbability_=0;
for (i=0;i<20;i++)
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
printf("\nTraining error =%lf", errorper);