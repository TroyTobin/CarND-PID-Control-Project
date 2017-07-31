#include "PID.h"

using namespace std;

/*
* TODO: Complete the PID class.
*/

PID::~PID() {}

PID::PID(double Kp,
	 double Ki,
	 double Kd)
{
  this->Kp = Kp;
  this->Ki = Ki;
  this->Kd = Kd;

  // Initially we set the integral error to 0 so that we start from the "zero" accumulation.
  p_error = 0;
  i_error = 0;
  d_error = 0;
  
}

void PID::UpdateError(double cte)
{
  // From "PID Implementation Solution" Lecture
  this->i_error += cte;
  this->d_error = cte - this->p_error;
  this->p_error = cte;  
}

double PID::TotalError()
{
  return this->i_error;
}

double PID::SteeringAngle()
{
  return 0 - (this->Kp * this->p_error) - (this->Kd * this->d_error) - (this->Ki * this->i_error);
}

