#include <uWS/uWS.h>
#include <iostream>
#include "json.hpp"
#include "PID.h"
#include <math.h>

// Values from "PID Implementaiton" Lecture
#define KP      (0.2)
#define KI    (0.004)
#define KD      (3.0)

// Set to 1 if using twiddle
#define TWIDDLING           (0)
// Number of twiddle points to use for parameter tessting
// This is instead of running for a full lap as this takes too long.
// It does take into acount a "straight" and a "curve" for tuning.
#define TWIDDLE_POINTS   (1000)

// for convenience
using json = nlohmann::json;

// For converting back and forth between radians and degrees.
constexpr double pi() { return M_PI; }
double deg2rad(double x) { return x * pi() / 180; }
double rad2deg(double x) { return x * 180 / pi(); }

// Checks if the SocketIO event has JSON data.
// If there is data the JSON object in string format will be returned,
// else the empty string "" will be returned.
std::string hasData(std::string s) {
  auto found_null = s.find("null");
  auto b1 = s.find_first_of("[");
  auto b2 = s.find_last_of("]");
  if (found_null != std::string::npos) {
    return "";
  }
  else if (b1 != std::string::npos && b2 != std::string::npos) {
    return s.substr(b1, b2 - b1 + 1);
  }
  return "";
}

static int count = 0;
static int iterations = 1;
static int index_ = 0;
static int state = 0;
static double bestErr = 100000000;
static double dp[3] = {0.2, 0.004, 3.0};

// Initial settings from "PID implementation" lecture
// This is used as the starting point when using twiddle to optimise the parameters
//static double p[3]  = {0.2, 0.004, 3.0};

// Paramters found from Twiddle after 250 iterations.  Lowest total error of 277.91
static double p[3]  = {0.420973, 0.000424476, 19.2487};

int main()
{
  uWS::Hub h;
  
  // Create the PID object
  PID pid(p[0], p[1], p[2]);

  h.onMessage([&pid](uWS::WebSocket<uWS::SERVER> ws, char *data, size_t length, uWS::OpCode opCode) 
  {
    count++;
    // "42" at the start of the message means there's a websocket message event.
    // The 4 signifies a websocket message
    // The 2 signifies a websocket event
    if (length && length > 2 && data[0] == '4' && data[1] == '2')
    {
      auto s = hasData(std::string(data).substr(0, length));
      if (s != "") {
        auto j = json::parse(s);
        std::string event = j[0].get<std::string>();
        if (event == "telemetry") {
          // j[1] is the data JSON object
          double cte = std::stod(j[1]["cte"].get<std::string>());
          double speed = std::stod(j[1]["speed"].get<std::string>());
          double angle = std::stod(j[1]["steering_angle"].get<std::string>());
          double steer_value;
          /*
          * TODO: Calcuate steering value here, remember the steering value is
          * [-1, 1].
          * NOTE: Feel free to play around with the throttle and speed. Maybe use
          * another PID controller to control the speed!
          */

          // Update the error in teh PID
	        pid.UpdateError(cte);

          // Find the appropriate steering angle
	        steer_value = pid.SteeringAngle();

      	  // Sanity check the steering angle is in range
      	  if (steer_value < -1)
      	  {
            steer_value = -1;
      	  }
      	  else if (steer_value > 1)
      	  {
            steer_value = 1;
      	  }
	      
          json msgJson;
          msgJson["steering_angle"] = steer_value;

          // Allow the speed to be 50% of max
          msgJson["throttle"] = 0.5;
          auto msg = "42[\"steer\"," + msgJson.dump() + "]";
          //std::cout << msg << std::endl;
          ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
        }
      } else {
        // Manual driving
        std::string msg = "42[\"manual\",{}]";
        ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
      }
    }

    // For twiddle
    if (TWIDDLING)
    {
      if (count > TWIDDLE_POINTS)
      {
        double error = pid.TotalError();
        std::cout << "Twiddle iteration " << iterations << ", [" << p[0] << ", " << p[1] << ", " << p[2] << "], error " << error << std::endl;
        std::cout << "Twiddle iteration " << iterations << ", [" << dp[0] << ", " << dp[1] << ", " << dp[2] << "], error " << bestErr << std::endl;
        iterations++;

        // Check the error from using the current parameters
        // Adjust the parameters and the tuning adjustment parameters
        if (error < bestErr)
        {
          bestErr = error;
          dp[index_] *= 1.1;
          state = 0;
          index_ = (index_ + 1) % 3;
          p[index_] += dp[index_];
          pid.Reset(p[0], p[1], p[2]);
        }
        else if (state == 0)
        {
          p[index_] -= 2 * dp[index_];
          pid.Reset(p[0], p[1], p[2]);
          state = 1;
        }
        else if (state == 1)
        {
          p[index_] += dp[index_];
          dp[index_] *= 0.9;
          index_ = (index_ + 1) % 3;
          p[index_] += dp[index_];
          pid.Reset(p[0], p[1], p[2]);
          state = 0;
        }
        count = 0;
    
        // Test new parameters of the same stretch of road
        // Reset the steering parameters
        json msgJson;
        msgJson["steering_angle"] = 0;
        msgJson["throttle"] = 0.3;
        std::string msg = "42[\"steer\"," + msgJson.dump() + "]";
        ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);

        // Reset the drive to the start location
        msg = "42[\"reset\",{}]";
        ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
    }
  }
  });

  // We don't need this since we're not using HTTP but if it's removed the program
  // doesn't compile :-(
  h.onHttpRequest([](uWS::HttpResponse *res, uWS::HttpRequest req, char *data, size_t, size_t) {
    const std::string s = "<h1>Hello world!</h1>";
    if (req.getUrl().valueLength == 1)
    {
      res->end(s.data(), s.length());
    }
    else
    {
      // i guess this should be done more gracefully?
      res->end(nullptr, 0);
    }
  });

  h.onConnection([&h](uWS::WebSocket<uWS::SERVER> ws, uWS::HttpRequest req) {
    std::cout << "Connected!!!" << std::endl;
  });

  h.onDisconnection([&h](uWS::WebSocket<uWS::SERVER> ws, int code, char *message, size_t length) {
    ws.close();
    std::cout << "Disconnected" << std::endl;
  });

  int port = 4567;
  if (h.listen(port))
  {
    std::cout << "Listening to port " << port << std::endl;
  }
  else
  {
    std::cerr << "Failed to listen to port" << std::endl;
    return -1;
  }
  h.run();
}
