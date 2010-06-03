/*
 *  TriggerTester.h
 *  HourglassIIMac
 *
 *  Created by Evan Wallace on 25/05/10.
 *  Copyright 2010 Team Causality. All rights reserved.
 *
 */
#include "Platform.h"
#include "Switch.h"
#include "TimeDirection.h"
#include "BoolValueExpression.h"
#include "boost/bind.hpp"
#include <iostream>
#include "HourglassAssert.h"
class TriggerTester {
public:
	void run() {
		int i;
		hg_assert(true);
		hg_assert(!false && !true);
	//	std::cin >> i;
	//	int j[i];
	//	j[i-2]+=30;
	//	std::cout << j[3];
		std::cout << "Test 0: " << (test0(i) ? "success" : "failure") ;

	}
	bool test0(int i) {
		/*
		 // platform which travels along simple L shaped path when a switch is activated
							  ||||||
							    ^
								|
		 ||||||   -> -> -> -> ||||||
		 
		 
		 <INSTRUCTIONS>
			<INSTRUCTION>
				TYPE = PLATFORM_AIMPOINT
				LABEL = "BLPAims"
			</INSTRUCTION>
		 </INSTRUCTIONS>
		 <SWITCHES>
			<SWITCH>
				TIME_DIRECTION = FORWARDS
				X_POS = 10
				Y_POS = 10
				LABEL = "BLPswitch"
				ROTATION = UP
			</SWITCH>
		 </SWITCHES>
		 <PLATFORMS>
			<PLATFORM>
				TIME_DIRECTION = FORWARDS
				START_X = 100
				START_Y = 120
				DEFAULT_MAX_SPEED = 14
				DEFAULT_ACCELERATION = 4
				LABEL = "bottomLeftPlatform"
				AIM_POINT_SOURCE = 0, "BLPAims"
			</PLATFORM>
		 </PLATFORMS>
		 
		 <TRIGGERS>
			0::BLPswitch.state ? {bottomLeftPlatform.xPos < 350 ? BLPAims = PlatformAimPoint(600,120) : BLPAims = PlatformAimPoint(350,220)} : {bottomLeftPlatform.yPos > 120 ? BLPAims = PlatformAimPoint(350,20) : };
		 </TRIGGERS>
		 
		 or in compiled terms (obviously all strings are removed, the names are used just to make it easier to understand):
		 
		 Instructions
			0 //= BLPAims.xAim
			1 //= BLPAims.yAim
		 Platforms
			bottomLeftPlatform:
				XAimPointSource = 0, 0
				YAimPointSource = 0, 1
		 Triggers
			0::BLPswitch.state ? {bottomLeftPlatform.xPos < 350 ? 0 = 600, 1 = 120 : 0 = 350, 1 = 220} : {bottomLeftPlatform.yPos > 120 ? 0 = 350, 1 = 20 : };
		 */
		/*
		
		TriggerEngineFactory factory = TriggerEngineFactory();
		
		Platform p1 = *(new Platform(100,120,14,4,FORWARD));
		//factory.RegisterObject("bottomLeftPlatform", &p1);
		
		Switch s1 = *(new Switch(10,10,UP,FORWARD));
		//register with myself not teh factory ((;
		//factory.RegisterObject("BLPswitch", &s1);
		int * moo = new int[i];
		int cor[i];
		boost::bind(&Switch::GetState, &s1, _1);
		factory.AddTrigger(
						   *(new Trigger(*(new BoolValueExpression(boost::bind(&Switch::GetState, &s1, _1))),
							 *(new Trigger(*(new LessExpression(*(new IntValueExpression(boost::bind(&Platform::GetXPos, &p1, _1))),*(new ArithmetricLiteralExpression(350))),
											 *(new EventSet()).Add(*(new InstructionSetter(factory.GetEngineUnderConstruction(),0,*(new IntValueExpression(600))))).Add(*(new InstructionSetter(factory.GetEngineUnderConstruction(),1,*(new IntValueExpression(120))))))
							 
							 ));
		try {
			 //use Instruction factory which takes char* and returns Instruction subclass
	//		factory.AddInstruction("BLPAims", AimPointInstruction());
		}
		catch (CompileException& e) {
			allegro_message("Level triggers failed to compile");
			throw new CompileException();
		}

		Platform& p1 = *(new Platform(100, 120, 14, 4, FORWARDS));
		try {
			factory.getInstruction("BLPAims")
			void (Platform::*setSourceFunction)() = p1.getSetSourceMember("AIM_POINT_SOURCE");
			p1.(*setSourceFunction(factory.getInstruction("BLPAims")));
		}
		catch () {
			
		}
		

		try {
			p1.addSource(AimPointInstruction());
		}
		catch (CopmpileException&) {
			allegro_message("Level triggers failed to compile");
			throw new CompileException();
		}
		factory.RegisterObject("bottomLeftPlatform", p1);
		
		
		Switch& s1 = *(new Switch(10, 10, UP, FORWARDS);
		try {
			factory.RegisterObject("BLPSwitch", s1);	
		}
		catch (CompileException& e) {
			allegro_message("Level triggers failed to compile");
			throw new CompileException();
		}
		
		
		
					   
		delete &s1;			   
		delete &p1;
	*/
		return false;
	}
	bool test1() {
		/*
		 platform which moves in a rectangular loop while switch on, and remains stationary while switch off
		 */
		return false;
	}
	bool test2() {
		/*
		 platform which moves forward in a rectangular loop while switch on and backward while switch off
		 */
		return false;
	}
	bool test3() {
	/*
		2 switch flip-flop ala HG1
	 */
		return false;
	}
	bool test4() {
		/*
			platform which moves up 3 seconds before a trigger is true
		 */
		return false;
	}
	bool test5() {
		/*
			platform which moves in a circle
		 */
		return false;
	}
	bool test6() {
		/*
			platform which moves in an elipse
		 */
		return false;
	}
	bool test7() {
		/*
			set of switches which must be pressed in a particular order in order to open a door ???
		 */
		return false;
	}
	bool test8() {
		/*
			alcatraz-like behaviour
		 */
		return false;
	}
	bool test9() {
		/*
			prints prime numbers
		 */
		return false;
	}
};
