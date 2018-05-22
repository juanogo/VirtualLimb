#ifndef EMOTIV_CONTROLLER_H
#define EMOTIV_CONTROLLER_H

#include "EmoStateDLL.h"
#include "edk.h"
#include "edkErrorCode.h"

#include <iostream>
#include <vector>
#include <map>
using std::cout;
using std::endl;

enum IndexNames
{
    CONTROL,
    COMPOSER,
    ENGINE,
    CUSTOM
};

static const char* ConnectionTypes[] =
{
    "Control Panel",
    "EmoComposer",
    "Emotiv Engine",
    "Custom"
};

enum
{
    MAX
};

static const char* OSCConnectionTypes[] =
{
    "Max/MSP"
};

class AffectivSuite
{
	
public:

	typedef enum AffectivTypes
	{
		ENGAGED_BORED, 
		EXCITEMENT,
		EXCITEMENTLT, 
		MEDITATION, 
		FRUSTRATION
	} AffectivTypes_t;

	
	AffectivSuite()
	{
		AffectivLabels[ENGAGED_BORED] = "Engaged/Bored";
		AffectivLabels[EXCITEMENT] = "Excitement";
		AffectivLabels[EXCITEMENTLT] = "Excitement Long Term";
		AffectivLabels[MEDITATION] = "Meditation";
		AffectivLabels[FRUSTRATION] = "Frustration";
	}

	void stateHandle(EmoStateHandle eState)
	{
		AffectivValues[ENGAGED_BORED] = ES_AffectivGetEngagementBoredomScore(eState);
		AffectivValues[EXCITEMENT]    = ES_AffectivGetExcitementShortTermScore(eState);
		AffectivValues[EXCITEMENTLT]  = ES_AffectivGetExcitementLongTermScore(eState);
		AffectivValues[MEDITATION]    = ES_AffectivGetMeditationScore(eState);
		AffectivValues[FRUSTRATION]   = ES_AffectivGetFrustrationScore(eState);

		//return AffectivValues;
	}

	std::map<int, char*> getLabels()
		{ return AffectivLabels; }

	std::map<int, float> getValues()
		{ return AffectivValues; }

	void getData(std::map<int, char*>& labels, std::map<int, float>& values)
	{
		labels = AffectivLabels;
		values = AffectivValues;
	}


private:
	std::map<int, float> AffectivValues;
	std::map<int, char*> AffectivLabels;
};

class ExpressivSuite
{

public:

	enum ExpressivTypes_Eyes
	{
		WINK_LEFT,
		WINK_RIGHT,
		BLINK,
		LEFT_LID,
		RIGHT_LID,
		HORIEYE,
		LOOK_LEFT,
		LOOK_RIGHT,
		VERTEYE,
		LOOK_UP,
		LOOK_DOWN
	};

	enum ExpressivTypes_LowerFace
	{
		SMILE = LOOK_DOWN + 1, 
		CLENCH,
		LAUGH,
		SMIRK_LEFT,
		SMIRK_RIGHT
	};

	enum ExpressivTypes_UpperFace
	{
		FURROW = SMIRK_RIGHT + 1,
		EYEBROW
	};

	ExpressivSuite()
	{
		for(int i=0; i<EYEBROW; i++)
			values[i] = 0;

		labels[WINK_LEFT] = "WINK_LEFT";
		labels[WINK_RIGHT] = "WINK_RIGHT";
		labels[BLINK] = "BLINK";
		labels[LEFT_LID] = "LEFT_LID";
		labels[RIGHT_LID] = "RIGHT_LID";
		labels[HORIEYE] = "HORIEYE";

		labels[VERTEYE] = "VERTEYE";

		labels[SMILE] = "SMILE";
		labels[CLENCH] = "CLENCH";
		labels[LAUGH] = "LAUGH";
		labels[SMIRK_LEFT] = "SMIRK_LEFT";
		labels[SMIRK_RIGHT] = "SMIRK_RIGHT";
		labels[FURROW] = "FURROW";
		labels[EYEBROW] = "EYEBROW";
	}

	/*
	typedef enum EE_ExpressivAlgo_enum {

		EXP_NEUTRAL			= 0x0001,
		EXP_BLINK			= 0x0002,
		EXP_WINK_LEFT		= 0x0004,
		EXP_WINK_RIGHT		= 0x0008,
        EXP_HORIEYE			= 0x0010,
		EXP_EYEBROW			= 0x0020,
		EXP_FURROW			= 0x0040,
        EXP_SMILE			= 0x0080,
		EXP_CLENCH			= 0x0100,
		EXP_LAUGH			= 0x0200,
		EXP_SMIRK_LEFT		= 0x0400,
		EXP_SMIRK_RIGHT		= 0x0800

	} EE_ExpressivAlgo_t;
	*/

	void stateHandle(EmoStateHandle eState)
	{
		ES_ExpressivGetEyelidState(eState, &values[LEFT_LID], &values[RIGHT_LID]);
		ES_ExpressivGetEyeLocation (eState, &values[HORIEYE], &values[VERTEYE]);

		actionLow = ES_ExpressivGetLowerFaceAction      ( eState );
		powerLow  = ES_ExpressivGetLowerFaceActionPower ( eState );

		actionHigh = ES_ExpressivGetUpperFaceAction      ( eState );		
		powerHigh  = ES_ExpressivGetUpperFaceActionPower ( eState );

		values[WINK_LEFT] = (values[LEFT_LID] == 1);
		values[WINK_RIGHT] = (values[RIGHT_LID] == 1);
		values[BLINK] = (values[LEFT_LID] == values[RIGHT_LID] && values[RIGHT_LID] == 1);
		
		if((actionLow & EXP_SMILE) == EXP_SMILE){ values[SMILE] = powerLow; }
		if((actionLow & EXP_CLENCH) == EXP_CLENCH){ values[CLENCH] = powerLow; }
		if((actionLow & EXP_LAUGH) == EXP_LAUGH){ values[LAUGH] = powerLow; }
		if((actionLow & EXP_SMIRK_LEFT) == EXP_SMIRK_LEFT){ values[SMIRK_LEFT] = powerLow; }
		if((actionLow & EXP_SMIRK_RIGHT) == EXP_SMIRK_RIGHT){ values[SMIRK_RIGHT] = powerLow; }

		if((actionHigh & EXP_EYEBROW) == EXP_EYEBROW){ values[EYEBROW] = powerHigh; }
		if((actionHigh & EXP_FURROW) == EXP_FURROW){ values[FURROW] = powerHigh; }
	}

	std::map<int, float> getValues()
	{ return values; }

	void getData(std::map<int, char*>& labels, std::map<int, float>& values)
	{
		labels = this->labels;
		values = this->values;
	}

private:
	std::map<int, char*> labels;
	std::map<int, float> values;

	float x, y, leftLid, rightLid;

	EE_ExpressivAlgo_t actionLow;
	EE_ExpressivAlgo_t actionHigh;
	float powerLow;
	float powerHigh;

};

class CognitivSuite
{
	std::map<EE_CognitivAction_t,char*> CognitivValues;
	EE_CognitivAction_t currentCogType;
	float power;

public:
	CognitivSuite()
	{
		currentCogType = COG_NEUTRAL;

		CognitivValues[COG_NEUTRAL] = "NEUTRAL";
		CognitivValues[COG_PUSH] = "PUSH";
		CognitivValues[COG_PULL] = "PULL";
		CognitivValues[COG_LIFT] = "LIFT";
		CognitivValues[COG_DROP] = "DROP";
		CognitivValues[COG_LEFT] = "LEFT";
		CognitivValues[COG_RIGHT] = "RIGHT";
		CognitivValues[COG_ROTATE_LEFT] = "ROTATE_LEFT";
		CognitivValues[COG_ROTATE_RIGHT] = "ROTATE_RIGHT";
		CognitivValues[COG_ROTATE_CLOCKWISE] = "ROTATE_CLOCKWISE";
		CognitivValues[COG_ROTATE_COUNTER_CLOCKWISE] = "ROTATE_COUNTER_CLOCKWISE";
		CognitivValues[COG_ROTATE_FORWARDS] = "ROTATE_FORWARDS";
		CognitivValues[COG_ROTATE_REVERSE] = "ROTATE_REVERSE";
		CognitivValues[COG_DISAPPEAR] = "DISAPPEAR";		
	}

	void stateHandle(EmoStateHandle eState)
	{
		currentCogType = ES_CognitivGetCurrentAction(eState);
		power = ES_CognitivGetCurrentActionPower(eState);			
	}

	void getCogData(char* &cog, float &pow)
	{
		cog = CognitivValues[currentCogType]; 
		pow = power;
	}


};

class EmotivToOSCPacket
{
	AffectivSuite  aff;
	ExpressivSuite exp;
	CognitivSuite  cog;

public:
	
	EmotivToOSCPacket(){}

	EmotivToOSCPacket(EmoStateHandle state)
	{
		aff.stateHandle(state);
		exp.stateHandle(state);
		cog.stateHandle(state);
	}
	
	AffectivSuite  getAffectiv()  { return aff; }
	ExpressivSuite getExpressiv() { return exp; }
	CognitivSuite  getCognitiv()  { return cog; }

};




static EE_DataChannel_t targetChannelList[] = {
		ED_COUNTER,
		ED_AF3, ED_F7, ED_F3, ED_FC5, ED_T7, 
		ED_P7, ED_O1, ED_O2, ED_P8, ED_T8, 
		ED_FC6, ED_F4, ED_F8, ED_AF4, ED_GYROX, ED_GYROY, ED_TIMESTAMP, 
		ED_FUNC_ID, ED_FUNC_VALUE, ED_MARKER, ED_SYNC_SIGNAL
	};

class EmotivConnectionThread
{

public:
    EmotivConnectionThread(){
		connectedToEngine = false;
	}

	void start(const char * connectType, const char * ipText, int portNum){
		eEvent = EE_EmoEngineEventCreate();
		eState = EE_EmoStateCreate();

		EE_EngineDisconnect();

		if(connectType == ConnectionTypes[ENGINE])
			engineStatusCode = EE_EngineConnect();
		else
			engineStatusCode = EE_EngineRemoteConnect(ipText, portNum);

		if(engineStatusCode != EDK_OK)
		{
			cout << "Can not connect to: " << connectType << endl;
		}
		else
			connectedToEngine = true;

		hData = EE_DataCreate();
	}

    void stop(){
		connectedToEngine = false;
		EE_EngineDisconnect();
	}

	bool isConnected(){
		return connectedToEngine;
	}

    void run(char* &type, float &power)
	{
		unsigned int userId;

		if(connectedToEngine)
		{
			engineStatusCode = EE_EngineGetNextEvent(eEvent);

			if(engineStatusCode == EDK_OK)
			{
				EE_Event_t eventType = EE_EmoEngineEventGetType(eEvent);
				EE_EmoEngineEventGetUserId(eEvent, &userId);

				if (eventType == EE_UserAdded) {
					EE_DataAcquisitionEnable(userId,true);
					//readytocollect = true;
				}

				if(eventType == EE_EmoStateUpdated)
				{
					EE_EmoEngineEventGetEmoState(eEvent, eState);
					
					EmotivToOSCPacket packet(eState);
					packet.getCognitiv().getCogData(type,power);

					//handleEEG(userId);
					EE_DataUpdateHandle(userId, hData);

					unsigned int nSamplesTaken=0;
					EE_DataGetNumberOfSample(hData,&nSamplesTaken);

					//std::cout << "Updated " << nSamplesTaken << std::endl;

					if (nSamplesTaken != 0) {

						double* data = new double[nSamplesTaken];
						for (int sampleIdx=0 ; sampleIdx<(int)nSamplesTaken ; ++ sampleIdx) {
							for (int i = 0 ; i<sizeof(targetChannelList)/sizeof(EE_DataChannel_t) ; i++) {

								EE_DataGet(hData, targetChannelList[i], data, nSamplesTaken);
								std::cout << data[sampleIdx] << ",";
							}	
							std::cout << std::endl;
						}
						delete[] data;
					}
				}
			}
		}
	}


private:
    EmoEngineEventHandle eEvent;
	EmoStateHandle eState;
	DataHandle hData;

    bool connectedToEngine;
	int engineStatusCode;

	int * gyroXdelta, gyroYdelta;

	void handleEEG(unsigned int userId);
};


#endif //EMOTIV_CONTROLLER_H