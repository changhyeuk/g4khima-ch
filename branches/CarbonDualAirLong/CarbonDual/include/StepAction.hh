#ifndef STEPACTION_HH_
#define STEPACTION_HH_

#include "G4UserSteppingAction.hh"

class StepAction : public G4UserSteppingAction
{
public:
  StepAction();
  ~StepAction();
  void UserSteppingAction(const G4Step*);
private:

};
#endif // STEPACTION_HH_
