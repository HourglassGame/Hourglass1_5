#ifndef HOURGLASS_LEVEL_H
#define HOURGLASS_LEVEL_H
#include "Hourglass_Allegro.h"
#include "Editor_Guy.h"
#include "Editor_Box.h"
#include "Editor_Object.h"
#include <vector>
#include <math.h>
#include <map>
#include "Editor_HourglassInputsEnum.h"
#include "Editor_SwitchingInput.h"
#include "Editor_AbsoluteTimeDirectionEnum.h"
class Level {
public:
	Level();
	~Level();
	Level& operator=(const Level& l);
	void SetWall(bool value,unsigned int x, unsigned int y);
	bool GetWall(unsigned int x, unsigned int y);
	void Draw();
	void DrawWall();
	void DrawObjects();
	void UpdateGUI();
	void UpdateSelection();
	void UpdateSelected();
	void SetCanSelect(bool canSelect);
	void AddGuy(const unsigned int xPos, const unsigned int yPos, const double xSpeed, const double ySpeed,const unsigned int startTime,const AbsoluteTimeDirectionEnum atd);
	void AddBox(const unsigned int xPos, const unsigned int yPos, const double xSpeed, const double ySpeed);
	void SetGridSize(const unsigned int size);
	Object* GetNextObject();
	void Reset();
private:
	static const int LEVEL_WIDTH = 32;
	static const int LEVEL_HEIGHT = 19;
	bool wall[LEVEL_WIDTH][LEVEL_HEIGHT];
	std::vector<Object*> objects;
	void TestLevel(const double squareSize);
	static const int BLOCK_SIZE = 32;
	int selectedObject;
	bool objectSelected;
	void DoMoveSelected();
	void DeleteSelectedObject();
	bool snapToGrid;
	int gridSize;
	bool canSelect;
	unsigned int nextObject;
	std::map<HourglassInput,SwitchingInput> inputs;
};
#endif //HOURGLASS_LEVEL_H
