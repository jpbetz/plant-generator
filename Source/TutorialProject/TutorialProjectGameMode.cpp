

#include "TutorialProject.h"
#include "TutorialProjectGameMode.h"
#include "TutorialProjectPlayerController.h"

ATutorialProjectGameMode::ATutorialProjectGameMode(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	PlayerControllerClass = ATutorialProjectPlayerController::StaticClass();
}


