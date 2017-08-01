# CarND-Controls-PID
Self-Driving Car Engineer Nanodegree Program

## Student describes the effect of the P, I, D component of the PID algorithm in their implementation. Is it what you expected?
* The `P` component is the `proportional` aspect of the steering angle.  That is, it provides a component that is directly proportional to the cross track error (CTE).  This causes the vehicle to take a direct line to the middle of the road, however since there is no compensation, it causes overshoot and thus the vehicle oscillates around the middle of the road.
  * The larger the parameters, the faster the vehicle will move toward the middle of the road - still oscillating.  Therefore, the frequency of the oscillation is directly associated to the magnitude of the P component (small P - low frequency, large P - high frequency)

* The `I` component is the `integral` aspect of the steering angle. This is used to offset any inherent `systematic bias`.  The example provided in the lecture series is a wheel alignment offset.  In this case the system is biased by this offset, and so any calculation for steering angle will be biased toward this angle and the vehicle will always be offset.  Therefore, the system will not perform as well as if no bias was observed.  By including the error integral, the steering angle takes into account the total observed error through time.  Therefore, if the `P` and `D` aspects are not having the desired effect of reducing the CTE, the integral component is used to make the final adjustments to `realign` the system and remove the bias.

* The `D` component is the `derivative` aspect, which is the temporal derivative of the crosstrack error.  It is used to reduce the overshoot caused by the proportional component.  It does this as it takes into account the changing cross track error.  So if the cross track error is already reducing, this component incorporates a `counter steer` to the proportional aspect.  Therefore the steering angle is reduced as the vehicle nears the middle of the road.
  * The larger the parameter the slower the vehicle tends to the centre of the road, as it causes a greater counter steer.


## Student discusses how they chose the final hyperparameters (P, I, D coefficients)
I used a twiddle function to chose the final parameters.  The initial values were chosen based on those used in the lectures series (0.2, 0.004, 3.0).  The twiddle function takes a long time to run, so I limited it to 1000 updates, which corresponded to the vehicle travelling along the first straight and around the first corner.  The twiddle function iterated 250 times, resetting the simulator each time.  Each time the total error was compared and ner parameters tested.  

The final parameters were found to be (0.420973, 0.000424476, 19.2487), resulting in a total error of 277.91.

---

## Dependencies

* cmake >= 3.5
 * All OSes: [click here for installation instructions](https://cmake.org/install/)
* make >= 4.1
  * Linux: make is installed by default on most Linux distros
  * Mac: [install Xcode command line tools to get make](https://developer.apple.com/xcode/features/)
  * Windows: [Click here for installation instructions](http://gnuwin32.sourceforge.net/packages/make.htm)
* gcc/g++ >= 5.4
  * Linux: gcc / g++ is installed by default on most Linux distros
  * Mac: same deal as make - [install Xcode command line tools]((https://developer.apple.com/xcode/features/)
  * Windows: recommend using [MinGW](http://www.mingw.org/)
* [uWebSockets](https://github.com/uWebSockets/uWebSockets)
  * Run either `./install-mac.sh` or `./install-ubuntu.sh`.
  * If you install from source, checkout to commit `e94b6e1`, i.e.
    ```
    git clone https://github.com/uWebSockets/uWebSockets 
    cd uWebSockets
    git checkout e94b6e1
    ```
    Some function signatures have changed in v0.14.x. See [this PR](https://github.com/udacity/CarND-MPC-Project/pull/3) for more details.
* Simulator. You can download these from the [project intro page](https://github.com/udacity/self-driving-car-sim/releases) in the classroom.

There's an experimental patch for windows in this [PR](https://github.com/udacity/CarND-PID-Control-Project/pull/3)

## Basic Build Instructions

1. Clone this repo.
2. Make a build directory: `mkdir build && cd build`
3. Compile: `cmake .. && make`
4. Run it: `./pid`. 

## Editor Settings

We've purposefully kept editor configuration files out of this repo in order to
keep it as simple and environment agnostic as possible. However, we recommend
using the following settings:

* indent using spaces
* set tab width to 2 spaces (keeps the matrices in source code aligned)

## Code Style

Please (do your best to) stick to [Google's C++ style guide](https://google.github.io/styleguide/cppguide.html).

## Project Instructions and Rubric

Note: regardless of the changes you make, your project must be buildable using
cmake and make!

More information is only accessible by people who are already enrolled in Term 2
of CarND. If you are enrolled, see [the project page](https://classroom.udacity.com/nanodegrees/nd013/parts/40f38239-66b6-46ec-ae68-03afd8a601c8/modules/f1820894-8322-4bb3-81aa-b26b3c6dcbaf/lessons/e8235395-22dd-4b87-88e0-d108c5e5bbf4/concepts/6a4d8d42-6a04-4aa6-b284-1697c0fd6562)
for instructions and the project rubric.

## Hints!

* You don't have to follow this directory structure, but if you do, your work
  will span all of the .cpp files here. Keep an eye out for TODOs.

## Call for IDE Profiles Pull Requests

Help your fellow students!

We decided to create Makefiles with cmake to keep this project as platform
agnostic as possible. Similarly, we omitted IDE profiles in order to we ensure
that students don't feel pressured to use one IDE or another.

However! I'd love to help people get up and running with their IDEs of choice.
If you've created a profile for an IDE that you think other students would
appreciate, we'd love to have you add the requisite profile files and
instructions to ide_profiles/. For example if you wanted to add a VS Code
profile, you'd add:

* /ide_profiles/vscode/.vscode
* /ide_profiles/vscode/README.md

The README should explain what the profile does, how to take advantage of it,
and how to install it.

Frankly, I've never been involved in a project with multiple IDE profiles
before. I believe the best way to handle this would be to keep them out of the
repo root to avoid clutter. My expectation is that most profiles will include
instructions to copy files to a new location to get picked up by the IDE, but
that's just a guess.

One last note here: regardless of the IDE used, every submitted project must
still be compilable with cmake and make./
