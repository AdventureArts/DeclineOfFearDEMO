

#include "DeclineOfFear.h"
#include "DOFSpectatorPawn.h"
#include "DOFCharacter.h"
#include "DOFPlayerController.h"


ADOFSpectatorPawn::ADOFSpectatorPawn(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
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

void ADOFSpectatorPawn::SetupPlayerInputComponent(class UInputComponent *InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

	check(InputComponent);

	InputComponent->BindAction("PossessAvatar", IE_Released, this, &ADOFSpectatorPawn::possessAvatar);
}

void ADOFSpectatorPawn::possessAvatar()
{
	if (Controller != nullptr)
	{
		ADOFPlayerController *dofPC = Cast<ADOFPlayerController>(Controller);

		if (dofPC)
		{
			dofPC->toDestroyOnPossess(this);
			dofPC->Possess(dofPC->GetControlledCharacter());
		}

#ifdef UE_EDITOR

		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Possessed"));
		}

#endif
	}
}