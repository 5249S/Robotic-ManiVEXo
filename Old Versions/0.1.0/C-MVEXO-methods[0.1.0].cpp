/*--------------------------------------------*/
/*                    5249S                   */
/*                The ManiVEXo                */
/*                   Methods                  */
/*                Version 0.1.0               */
/*--------------------------------------------*/

//The methods used in these classes will not contain while loops, to prevent linearity and to prevent freezing of autons
class Pid {
    private:
        //variables that affect pid that will remain hidden from user to increase stability/abstraction
        float prevError = 0;
        float pidIntegral = 0;
    public:
    
        Pid() { }
        
        float setPoint = 0;
        float kP = 0.0;
        float kI = 0.0;
        float kD = 0.0;
    
        float pidCalc(float processVar){
            //This is a single calculation for one cycle of a PID calculator, run in while loop, put output into control system
            //Define variables
            float pidProportional = 0.0;
            float pidDerivative = 0.0;
            float pidError = 0.0;

            //Calculate Error
            pidError = setPoint - processVar;

            //Calculate Proportional
            pidProportional = pidError * kP;
            
            //Calculate Derivative
            pidDerivative = (pidError - prevError) * kD;

            //Calculate Integral
            if (fabs((double)pidIntegral) < 50) {
                    pidIntegral += (pidError + prevError)/2;
            } else {
                    pidIntegral = 0;
            }

            //use pid structure to return values
            
            float adjust = pidDerivative + pidProportional + kI * pidIntegral;
            //reset error for next loop
            prevError = pidError;
            

            //return adjustment, new error, and pidIntegral
            return adjust;
        }
        
};
class Lift {//Class for driving the lift
    public:
        void drive(int power){
            if (power < 0){
                mtrLiftLeft.spin(vex::directionType::rev, (double)(-power), vex::velocityUnits::pct);
                mtrLiftRight.spin(vex::directionType::rev, (double)(-power), vex::velocityUnits::pct);
            } else {
                mtrLiftLeft.spin(vex::directionType::fwd, (double)power, vex::velocityUnits::pct);
                mtrLiftRight.spin(vex::directionType::fwd, (double)power, vex::velocityUnits::pct);
            }
        }
};
class DriveMethods {
    //Class for methods for driving the robot around the field
    protected:
        //Calculators for motor power to translate remote joystick values into motor powers
        int leftMotor(int y, int x){
            int powerLeft = 0;
            if (y >= 0) {
                if (x >= 0) {
                    if (y >= x) {
                        powerLeft = y;
                    } else {
                        powerLeft = x;
                    }
                } else {
                    powerLeft = y + x;
                }
            } else {
                if (x <= 0) {
                    if (y <= x) {
                        powerLeft = y;
                    } else {
                        powerLeft = x;
                    }
                } else {
                    powerLeft = y + x;
                }
            }
            return powerLeft;
        }
        int rightMotor(int y, int x){
            return leftMotor(y, -x);
        }
    public:
    
        DriveMethods() { }
    
        Pid drive; //Creates a PID object for controlling driving
        Pid turn;  //Creates a PID object for controlling turning
        void driveH(int y, int x){ //Method for driving the chassis with an h-drive, using turning
            //*Repeat method in while loop for continuous control, this is instant implementation for the motor power calculators*
            if (y < 3 && y > -3) {
                y = 0;
            }
            if (x < 3 && x > -3) {
                x = 0;
            }

            int leftPower = leftMotor(y, x);
            int rightPower = rightMotor(y, x);
            if (leftPower < 0){
                mtrDriveLeft.spin(vex::directionType::rev, (double)(-leftPower), vex::velocityUnits::pct);
            } else {
                mtrDriveLeft.spin(vex::directionType::fwd, (double)leftPower, vex::velocityUnits::pct);
            }
            
            if (rightPower < 0){
                mtrDriveRight.spin(vex::directionType::rev, (double)(-rightPower), vex::velocityUnits::pct);
            } else {
                mtrDriveRight.spin(vex::directionType::fwd, (double)rightPower, vex::velocityUnits::pct);
            }
            
        }
    
};
