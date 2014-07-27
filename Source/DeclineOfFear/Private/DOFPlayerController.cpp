

#include "DeclineOfFear.h"
#include "DOFPlayerController.h"


ADOFPlayerController::ADOFPlayerController(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	toDestroy = nullptr;
}

void ADOFPlayerController::toDestroyOnPossess(APawn *pawn)
{
	toDestroy = pawn;
}

void ADOFPlayerController::Possess(class APawn* inPawn)
{
	Super::Possess(inPawn);

	if (toDestroy != nullptr)
	{
		if (toDestroy->Destroy())
		{
			toDestroy = nullptr;

#ifdef UE_EDITOR

			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Spectator OFF"));
			}

#endif
		}
	}
}