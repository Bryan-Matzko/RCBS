//Functions
//Opens or closes the claw based on main's call
void clawManip(bool opened)
{
	const int timeOpen=100;
	//If opened close the claw
	if (opened)
	{
		motor[motorC]=75;
		wait1Msec(timeOpen);
		motor[motorC]=0;
	}
	//If closed open the claw
	else
	{
		motor[motorC]=-75;
		wait1Msec(timeOpen);
		motor[motorC]=0;
	}
}

//Checks to see if the ball is in grasp
bool inGrasp()
{
	if (SensorValue[S1]==0)
		return false;
	else
		return true;
}

//Lifts the arm up or down by a given angle
void armYMovement(int degrees)
{
	if (degrees<0)
	{
		nMotorEncoder[motorB]=0;
		motor[motorB]=-100;
		//Moves the arm in specified degrees
		//stops when degrees is met or if the touch sensor is activated
		clearTimer(T1);
		while (time1[T1]<4500 &&!inGrasp())
		{}
		motor[motorB]=0;
	}
	else
	{
		motor[motorC]=10;
		nMotorEncoder[motorB]=0;
		motor[motorB]=100;
		wait1Msec(2900);
		motor[motorB]=0;
		motor[motorC]=0;
	}
}

//Senses the colour of a bin or ball
int colourSensed()
{
	wait1Msec(1000);
	return SensorValue[S3];
}

//Constraint to arm spin
bool binSense()
{
	//If colour sensor senses black, blue, green or red and if so returns true
	if (SensorValue[S2]==1||SensorValue[S2]==2||SensorValue[S2]==3||SensorValue[S2]==5)
		return true;
	else
		return false;
}

//Compares the colour of the ball to the bin colour that was just sensed
bool compare(int binColour, int ballColour)
{
	//Returns whether bin colour is equivalent to ball colour
	if (binColour==ballColour)
		return true;
	else
		return false;
}

//Spins Arm until it senses a depicted colour bin
//compares the bin colour to a ball colour, if the
//same the claw will open to drop the ball, wait
//And return to the starting position
void spinArm(int ballCOLOUR)
{
	bool stopSpin=false;
	nMotorEncoder[motorA]=0;
	motor[motorA]=60;

	while (!stopSpin)
	{
		//Determines if a bin is sensed
		if (binSense())
		{
			//if a bin is sensed stop motor
			motor[motorA]=0;
			//compare ball to the bin type if same drop ball and
			//stop the continuation of spinning
			if (compare(SensorValue[S2],ballCOLOUR))
			{
				wait1Msec(1000);
				clawManip(false);
				stopSpin=true;
			}
			//if not the same continue spinning to find correct bin
			else
				motor[motorA]=40;
		}
	}
	wait1Msec(1000);
	//returns to the initial position using the motor encoder
	motor[motorA]=-40;
	while(nMotorEncoder[motorA]!=0)
	{}
	motor[motorA]=0;
}

task main()
{
	//Sensor Declarations
	SensorType[S1]=sensorTouch;
	SensorType[S2]=sensorColorNxtFULL;
	SensorType[S3]=sensorColorNxtFULL;

	//Variable Declaration degree in y axis and ball colour value
	const int degreesY=1;
	int ballColour;



	//Starting of robot functions
	//Maximum of 10 balls to be sorted in a single program
	for (int x=0; x<10; x++)
	{
		//Waits for a user to press the orange button, starting program
		while(nNxtButtonPressed!=3)
		{}

		//Changes the degrees in Y to grab ball
		armYMovement(-degreesY);

		//Grabs the ball
		clawManip(true);

		//Returns to the original ball height
		armYMovement(degreesY);


		//Determines the colour of the ball in grasp
		ballColour=colourSensed();

		//Calls spinArm function to spin arm and find the correct bin
		//to place the ball in. Returns to the initial position after
		if (ballColour==6)
			clawManip(false);
		else
			spinArm(ballColour);
	}

}
