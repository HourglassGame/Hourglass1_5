#include "Editor_SwitchingInput.h" // class's header file
extern BITMAP* buffer;

// class constructor
SwitchingInput::SwitchingInput() :
    currentValue(false),
    isToggledOn(false),
	assignedCombinations(std::list<KeyCombination*>())
{ }

// class destructor
SwitchingInput::~SwitchingInput()
{ }

void SwitchingInput::Add(const int keyCode, const RepeatType repeatType, int shiftCode)
{
     assignedCombinations.push_back(new KeyCombination(keyCode,repeatType,shiftCode));
}

void SwitchingInput::Add(const int keyCode, const RepeatType repeatType)
{
     assignedCombinations.push_back(new KeyCombination(keyCode,repeatType));
}

void SwitchingInput::Update()
{
//     const bool pastValue = currentValue;
     currentValue = false;
     for(std::list<KeyCombination*>::iterator it = assignedCombinations.begin(); it != assignedCombinations.end(); ++it)
     {
         (*it)->UpdatePressed();
         switch((*it)->GetType())
         {
             case PRESS:
                 if((*it)->GetPressed())
                 {
                     if(!(*it)->GetPastPressed())
                     {
                         currentValue = true;
                     }
                 }
                 break;
             case RELEASE:    
                 if(!((*it)->GetPressed()))
                 {
                     if((*it)->GetPastPressed())
                     {
                         currentValue = true;
                     }
                 }
                 break;
             case TOGGLE_ON_PRESS:
                 if((*it)->GetPressed())
                 {
                     if(!(*it)->GetPastPressed())
                     {
                         if(isToggledOn)
                         {
                             isToggledOn = false;
                         }
                         else
                         {
                             isToggledOn = true;
                         }
                     }
                   //  currentPress = true;
                 }
                 if (isToggledOn)
                 {
                     currentValue = true;
                 }
                 break;
             case TOGGLE_ON_RELEASE:
                 if(!((*it)->GetPressed()))
                 {
                     if((*it)->GetPastPressed())
                     {
                         if(isToggledOn)
                         {
                             isToggledOn = false;
                         }
                         else
                         {
                             isToggledOn = true;
                         }
                     }
                 }
                 if (isToggledOn)
                 {
                     currentValue = true;            
                 }
                 break;
            case HOLD:
                 if((*it)->GetPressed())
                 {
                     currentValue = true;
                 }
                 break;
            default:
            allegro_message("switch((*it)->GetType()) in SwitchingInput::Update() fell through to default\n"
                             "this should never happen!");
         }
     }
}

void SwitchingInput::ToggleOn()
{
	isToggledOn = true;
}
void SwitchingInput::ToggleOff()
{
	isToggledOn = false;
}
void SwitchingInput::Toggle()
{
	isToggledOn = isToggledOn ? false : true;
}

bool SwitchingInput::operator()()
{
	return(GetCurrentValue());
}

bool SwitchingInput::GetCurrentValue()
{
    return(currentValue);
}

void SwitchingInput::Reset()
{
    while (assignedCombinations.size() != 0)
    {
       delete assignedCombinations.back();
       assignedCombinations.pop_back();
    }
    currentValue = false;
    isToggledOn = false;
}
