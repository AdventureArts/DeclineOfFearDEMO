

#include "DeclineOfFear.h"
#include "DOFSpectatorPawn.h"
#include "DOFCharacter.h"
#include "DOFPlayerController.h"


ADOFSpectatorPawn::ADOFSpectatorPawn(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void ADOFSpectatorPawn::BeginPlay()
{
	Super::BeginPlay();

#ifdef UE_EDITOR

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Spectator ON"));
	}

#endif
}
