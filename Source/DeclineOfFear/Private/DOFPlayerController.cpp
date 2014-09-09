

#include "DeclineOfFear.h"
#include "DOFPlayerController.h"


ADOFPlayerController::ADOFPlayerController(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	InputComponent = ConstructObject<UInputComponent>(UInputComponent::StaticClass(), this, TEXT("ControllerInputComponent"));

	toDestroy = nullptr;
	controlledCharacter = nullptr;
}

void ADOFPlayerController::BeginPlay()
{
	SetupControllerInputComponent(InputComponent);
}

void ADOFPlayerController::SetupControllerInputComponent(UInputComponent* InputComponent)
{
	check(InputComponent);

	InputComponent->BindAction("UnPossessAvatar", IE_Released, this, &ADOFPlayerController::UnPossessAvatar);
}

void ADOFPlayerController::toDestroyOnPossess(APawn *pawn)
{
	toDestroy = pawn;
}

void ADOFPlayerController::Possess(class APawn* inPawn)
{
	ASpectatorPawn *specP = GetSpectatorPawn();

	if (inPawn && specP)
	{
		FVector destLocation = specP->GetActorLocation();
		destLocation.Z -= inPawn->GetDefaultHalfHeight();

		FRotator destRotation = specP->GetControlRotation();
		destRotation.Pitch = 0.f;

		inPawn->TeleportTo(destLocation, destRotation);
	}

	ADOFCharacter *character = Cast<ADOFCharacter>(inPawn);

	if (character)
	{
		controlledCharacter = character;

		if (specP)
		{
			controlledCharacter->TeleportCamera(specP->GetActorLocation(), specP->GetControlRotation());
		}
	}

	if (controlledCharacter) controlledCharacter->Mesh->SetVisibility(true, true);

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

	Super::Possess(inPawn);
}

ADOFCharacter* ADOFPlayerController::GetControlledCharacter() { return controlledCharacter; };

void ADOFPlayerController::UnPossessAvatar()
{
	if (controlledCharacter) controlledCharacter->UnPossessMe();
}