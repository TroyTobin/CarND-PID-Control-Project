#ifndef PID_H
#define PID_H

class PID {
public:
  /*
  * Errors
  */
  double p_error;
  double i_error;
  double d_error;
  double total_error;

  /*
  * Coefficients
  */ 
  double Kp;
  double Ki;
  double Kd;

  /*
  * Constructor
  */
  PID(double Kp,
      double Ki,
      double Kd);

  /*
  * Destructor.
  */
  virtual ~PID();

  /*
  * Reset the PID controller
  */
  void Reset(double Kp,
             double Ki,
             double Kd);
  /*
  * Update the PID error variables given cross track error.
  */
  void UpdateError(double cte);

  /*
  * Calculate the total PID error.
  */
  double TotalError();

  /*
   * Determine steering angle
   */
  double SteeringAngle();
};

#endif /* PID_H */
