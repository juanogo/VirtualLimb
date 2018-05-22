#include "myoController.h"

//DataCollector of myoController constructor
DataCollector::DataCollector()
{
	onArm=false;
	isUnlocked=false;
	roll_w=0;
	pitch_w=0; 
	yaw_w=0;
	currentPose;
	emgSamples;
}

// onUnpair() is called whenever the Myo is disconnected from Myo Connect by the user.
void DataCollector::onUnpair(myo::Myo* myo, uint64_t timestamp)
{
    // We've lost a Myo.
    // Let's clean up some leftover state.
    emgSamples.fill(0);


    // Let's clean up some leftover state.
    roll_w = 0;
    pitch_w = 0;
    yaw_w = 0;
    onArm = false;
    isUnlocked = false;
}

// onOrientationData() is called whenever the Myo device provides its current orientation, which is represented
// as a unit quaternion.
void DataCollector::onOrientationData(myo::Myo* myo, uint64_t timestamp, const myo::Quaternion<float>& quat)
{
    using std::atan2;
    using std::asin;
    using std::sqrt;
    using std::max;
    using std::min;

    // Calculate Euler angles (roll, pitch, and yaw) from the unit quaternion.
    float roll = atan2(2.0f * (quat.w() * quat.x() + quat.y() * quat.z()),
                        1.0f - 2.0f * (quat.x() * quat.x() + quat.y() * quat.y()));
    float pitch = asin(max(-1.0f, min(1.0f, 2.0f * (quat.w() * quat.y() - quat.z() * quat.x()))));
    float yaw = atan2(2.0f * (quat.w() * quat.z() + quat.x() * quat.y()),
                    1.0f - 2.0f * (quat.y() * quat.y() + quat.z() * quat.z()));

    // Convert the floating point angles in radians to a scale from 0 to 18.
    //roll_w = static_cast<int>((roll + (float)M_PI)/(M_PI * 2.0f) * 18);
    //pitch_w = static_cast<int>((pitch + (float)M_PI/2.0f)/M_PI * 18);
    //yaw_w = static_cast<int>((yaw + (float)M_PI)/(M_PI * 2.0f) * 18);
	
	roll_w = roll;
	pitch_w = pitch;
	yaw_w = yaw;

}


// onEmgData() is called whenever a paired Myo has provided new EMG data, and EMG streaming is enabled.
void DataCollector::onEmgData(myo::Myo* myo, uint64_t timestamp, const int8_t* emg)
{
    for (int i = 0; i < 8; i++) {
        emgSamples[i] = emg[i];
    }
}


// onPose() is called whenever the Myo detects that the person wearing it has changed their pose, for example,
// making a fist, or not making a fist anymore.
void DataCollector::onPose(myo::Myo* myo, uint64_t timestamp, myo::Pose pose)
{
    currentPose = pose;

    if (pose != myo::Pose::unknown && pose != myo::Pose::rest) {
        // Tell the Myo to stay unlocked until told otherwise. We do that here so you can hold the poses without the
        // Myo becoming locked.
        myo->unlock(myo::Myo::unlockHold);

        // Notify the Myo that the pose has resulted in an action, in this case changing
        // the text on the screen. The Myo will vibrate.
        myo->notifyUserAction();
    } else {
        // Tell the Myo to stay unlocked only for a short period. This allows the Myo to stay unlocked while poses
        // are being performed, but lock after inactivity.
        myo->unlock(myo::Myo::unlockTimed);
    }
}

// onArmSync() is called whenever Myo has recognized a Sync Gesture after someone has put it on their
// arm. This lets Myo know which arm it's on and which way it's facing.
void DataCollector::onArmSync(myo::Myo* myo, uint64_t timestamp, myo::Arm arm, myo::XDirection xDirection, float rotation,
                myo::WarmupState warmupState)
{
    onArm = true;
    whichArm = arm;
}

// onArmUnsync() is called whenever Myo has detected that it was moved from a stable position on a person's arm after
// it recognized the arm. Typically this happens when someone takes Myo off of their arm, but it can also happen
// when Myo is moved around on the arm.
void DataCollector::onArmUnsync(myo::Myo* myo, uint64_t timestamp)
{
    onArm = false;
}

// onUnlock() is called whenever Myo has become unlocked, and will start delivering pose events.
void DataCollector::onUnlock(myo::Myo* myo, uint64_t timestamp)
{
    isUnlocked = true;
}

// onLock() is called whenever Myo has become locked. No pose events will be sent until the Myo is unlocked again.
void DataCollector::onLock(myo::Myo* myo, uint64_t timestamp)
{
    isUnlocked = false;
}

// There are other virtual functions in DeviceListener that we could override here, like onAccelerometerData().
// For this example, the functions overridden above are sufficient.

// We define this function to print the current values that were updated by the on...() functions above.
void DataCollector::print()
{
    //// Print out the orientation. Orientation data is always available, even if no arm is currently recognized.
    //std::cout << '[' << std::string(roll_w, '*') << std::string(18 - roll_w, ' ') << ']'
    //            << '[' << std::string(pitch_w, '*') << std::string(18 - pitch_w, ' ') << ']'
    //            << '[' << std::string(yaw_w, '*') << std::string(18 - yaw_w, ' ') << ']';

    //if (onArm) {
    //    // Print out the lock state, the currently recognized pose, and which arm Myo is being worn on.

    //    // Pose::toString() provides the human-readable name of a pose. We can also output a Pose directly to an
    //    // output stream (e.g. std::cout << currentPose;). In this case we want to get the pose name's length so
    //    // that we can fill the rest of the field with spaces below, so we obtain it as a string using toString().
    //    std::string poseString = currentPose.toString();

    //    std::cout << '[' << (isUnlocked ? "unlocked" : "locked  ") << ']'
    //                << '[' << (whichArm == myo::armLeft ? "L" : "R") << ']'
    //                << '[' << poseString << std::string(14 - poseString.size(), ' ') << ']';
    //} else {
    //    // Print out a placeholder for the arm and pose when Myo doesn't currently know which arm it's on.
    //    std::cout << '[' << std::string(8, ' ') << ']' << "[?]" << '[' << std::string(14, ' ') << ']';
    //}

	// Print out the EMG data.
    for (size_t i = 0; i < emgSamples.size(); i++) {
        std::ostringstream oss;
        oss << static_cast<int>(emgSamples[i]);
        std::string emgString = oss.str();

        std::cout << '[' << emgString << std::string(4 - emgString.size(), ' ') << ']';
    }

	std::cout << '[' << currentPose << ']';


	// Print out Gyro data.
	std::cout << '[' << std::setprecision(1) << roll_w << ']' 
			<< '[' << std::setprecision(1) << pitch_w << ']'
			<< '[' << std::setprecision(1) << yaw_w << ']' /*<< std::endl*/;

	// Clear the current line
    std::cout << "   " << '\r';

    std::cout << std::flush;
}

////////////// MODIFIED FUNCTIONS


std::vector<float> DataCollector::getGyro()
{
	std::vector<float> gyroData;
	gyroData.push_back(roll_w);
	gyroData.push_back(pitch_w);
	gyroData.push_back(yaw_w);

	return gyroData;
}

std::array<int8_t, 8> DataCollector::getEmgSamples()
{
	return emgSamples;
}

std::string DataCollector::getPose()
{
	return currentPose.toString();
}

int DataCollector::calculatePower()
{
	int potencia=0;
	std::vector <int> senales;
 
	for (size_t i = 0; i < emgSamples.size(); i++) {
		std::ostringstream oss;
		oss << static_cast<int>(emgSamples[i]);
		int senal = static_cast<int>(emgSamples[i]);
		senales.push_back(senal);
		potencia = potencia + (senal*senal);
	}

	return potencia;
}

void DataCollector::saveToFile(char* filename)
{
	// Variable to write file titles only once.
	static bool firstTime=true;

	int potencia=0;
	std::vector <int> senales;

    // Print out the EMG data.
	std::ofstream outputFile;
	outputFile.open(filename,std::ios::app);

	if (outputFile.is_open())
	{
		// Title of rows
		if(firstTime)
		{
			outputFile << "CH0," << "CH1," << "CH2," << "CH3," 
						<< "CH4," << "CH5," << "CH6," << "CH7," 
						<< "Pose,"<< "Roll," << "Pitch," << "Yaw" << std::endl;
			firstTime = false;
		}

		// Write EEG values
		for (size_t i = 0; i < emgSamples.size(); i++) {
			std::ostringstream oss;
			oss << static_cast<int>(emgSamples[i]);
			int senal = static_cast<int>(emgSamples[i]);
			senales.push_back(senal);
			potencia = potencia + (senal*senal);
				
			std::string emgString = oss.str();
			outputFile << emgString << ',';

		}

		outputFile << currentPose << ",";

		// Write Gyro values
		outputFile << std::setprecision(3) << roll_w << "," << pitch_w << "," << yaw_w;
		outputFile << "\n";

		outputFile.close();
	}
}