

#include "DeclineOfFear.h"
#include "DOFGameMode.h"
#include "DOFSpectatorPawn.h"
#include "DOFPlayerController.h"


ADOFGameMode::ADOFGameMode(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	PlayerControllerClass = ADOFPlayerController::StaticClass();
	SpectatorClass = ADOFSpectatorPawn::StaticClass();
	DefaultPawnClass = ADOFSpectatorPawn::StaticClass();
}


void ADOFGameMode::BeginPlay()
{
	Super::BeginPlay();

#ifdef UE_EDITOR

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Decline of Fear"));
	}

#endif
}