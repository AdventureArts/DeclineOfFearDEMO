

#include "DeclineOfFear.h"
#include "DOFGameMode.h"
#include "DOFSpectatorPawn.h"
#include "DOFPlayerController.h"


ADOFGameMode::ADOFGameMode(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<ADOFCharacter> adamClass(TEXT("/Game/Blueprints/Characters/Hero/Hero_Character"));

	PlayerControllerClass = ADOFPlayerController::StaticClass();
	SpectatorClass = ADOFSpectatorPawn::StaticClass();
	DefaultPawnClass = adamClass.Class;
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