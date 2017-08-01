#include <math.h>
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

  // Initially we set the errors to 0 so that we start from the "zero" accumulation.
  p_error = 0;
  i_error = 0;
  d_error = 0;
  total_error = 0;
  
}


/*
 * Reset the PID without having to destroy and re-create
 */
void PID::Reset(double Kp,
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
  total_error = 0;

}

/*
 * Update the internal erro tracking
 */
void PID::UpdateError(double cte)
{
  // From "PID Implementation Solution" Lecture
  this->i_error += cte;
  this->d_error = cte - this->p_error;
  this->p_error = cte; 

  // Error can be negative so use the absolute version
  this->total_error += fabs(cte); 
}

/*
 * Return the total error
 */
double PID::TotalError()
{
  return fabs(this->total_error);
}

/*
 * Determine steering angle using PID equation
 */
double PID::SteeringAngle()
{
  // PID equation
  return 0 - (this->Kp * this->p_error) - (this->Kd * this->d_error) - (this->Ki * this->i_error);
}

