

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
	Super::Possess(inPawn);

	ADOFCharacter *character = Cast<ADOFCharacter>(inPawn);

	if (character) controlledCharacter = character;

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

void ADOFPlayerController::UnPossessAvatar()
{
	if (controlledCharacter)
	{
		controlledCharacter->UnPossessMe();
		controlledCharacter = nullptr;
	}
}