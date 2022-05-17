#include "pid.h"
extern int I_Set;
/*====================================================================================================
PID Function
The PID (比例、积分、微分) function is used in mainly
control applications. PIDCalc performs one iteration of the PID
algorithm.
While the PID function works, main is just a dummy program showing
a typical usage.
=====================================================================================================*/
/*====================================================================================================/
PID计算部分
=====================================================================================================*/
PID   iPID={0,0,0,0,0,0,0};    //0.3,0.005,0
PID	  vPID={0,0,0,0,0,0,0};	   //0.008,0.006,0.001


double PIDCalc( PID *pp, double NextPoint  )
{
	double dError,
	Error;
	Error = pp->SetPoint - NextPoint; // 偏差
	pp->SumError += Error; // 积分
	dError = pp->LastError - pp->PrevError; // 当前微分
	pp->PrevError = pp->LastError;
	pp->LastError = Error;

    if(pp->SumError>1500)  pp->SumError=1500;
    else if(pp->SumError<-1500)	  pp->SumError=-1500;

	return (pp->Proportion * Error // 比例项
	+ pp->Integral * pp->SumError // 积分项
	+ pp->Derivative * dError // 微分项
	);
}


double Vol(double vol_set,double vol_adc)
{
	double vOut; // PID Response (Output)
	double vIn; // PID Feedback (Input)

	vPID.SetPoint = (double)vol_set; // Set PID Setpoint
	vIn = (double)vol_adc; // Read Input
	vOut = PIDCalc ( &vPID,vIn ); // Perform PID Interation	 
	
	return vOut;
}



double Cur(double iSet,double i3)
{

	double iOut; // PID Response (Output)
	double iIn; // PID Feedback (Input)

	iPID.SetPoint = iSet; // Set PID Setpoint
	iIn = (double)i3; // Read Input
	iOut = PIDCalc ( &iPID,iIn ); // Perform PID Interation	 
	return iOut;
}




void Set_vPid(double p,double i,double d)
{
	vPID.Proportion = p ;
	vPID.Integral = i ;
	vPID.Derivative  = d ;
}


void Reset_vPid(void)
{
	vPID.LastError = 0 ;
	vPID.PrevError = 0 ;
	vPID.SumError  = 0 ;
}


void Set_iPid(double p,double i,double d)
{
	iPID.Proportion = p ;
	iPID.Integral = i ;
	iPID.Derivative  = d ;  //待添加 ，修改后要存入Flash中，否则重启值会变化
}


void Reset_iPid(void)
{
	iPID.LastError = 0 ;
	iPID.PrevError = 0 ;
	iPID.SumError  = 0 ;

}

void Pid_Init(void)
{
	Set_vPid(0.01,0,0);   //待修改  ，应该从Flash中读取存储的pid值
	Set_iPid(0.04,0.0005,0.005);
}
