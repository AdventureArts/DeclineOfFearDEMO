

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
		ADOFCharacter *dofCharacter = nullptr;
		float distance, nearest = TNumericLimits<float>::Max();

		for (TObjectIterator<ADOFCharacter> it; it; ++it)
		{
			distance = FVector::Dist(it->GetActorLocation(), this->GetActorLocation());

			if (distance < nearest)
			{
				dofCharacter = *it;
				nearest = distance;
			}
		}

		if (dofCharacter != nullptr)
		{
			ADOFPlayerController *dofPC = Cast<ADOFPlayerController>(Controller);

			if (dofPC)
			{
				dofPC->toDestroyOnPossess(this);
			}

			Controller->Possess(dofCharacter);
		}

#ifdef UE_EDITOR

		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Possessed"));
		}

#endif
	}
}