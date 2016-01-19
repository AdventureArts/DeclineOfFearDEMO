

#include "DeclineOfFear.h"
#include "DOFPlayerController.h"


ADOFPlayerController::ADOFPlayerController(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	InputComponent = NewObject<UInputComponent>(this, TEXT("ControllerInputComponent"));

	toDestroy = nullptr;
	controlledCharacter = nullptr;
}

void ADOFPlayerController::BeginPlay()
{
	controlledCharacter = Cast<ADOFCharacter>(GetPawn());

	SetupControllerInputComponent(InputComponent);
}

void ADOFPlayerController::SetupControllerInputComponent(UInputComponent* InputComponent)
{
	check(InputComponent);

	InputComponent->BindAction("PossessAvatar", IE_Released, this, &ADOFPlayerController::PossessAvatar);
	InputComponent->BindAction("UnPossessAvatar", IE_Released, this, &ADOFPlayerController::UnPossessAvatar);
}

void ADOFPlayerController::ToDestroyOnPossess(APawn *pawn)
{
	toDestroy = pawn;
}

void ADOFPlayerController::Possess(class APawn* inPawn)
{
	ADOFSpectatorPawn *specPawn = Cast<ADOFSpectatorPawn>(GetPawn());

	if (inPawn && specPawn)
	{
		FVector destLocation = specPawn->GetActorLocation();
		destLocation.Z -= inPawn->GetDefaultHalfHeight();

		FRotator destRotation = specPawn->GetControlRotation();
		destRotation.Pitch = 0.f;

		inPawn->TeleportTo(destLocation, destRotation);
	}

	ADOFCharacter *character = Cast<ADOFCharacter>(inPawn);

	if (character)
	{
		controlledCharacter = character;
		controlledCharacter->TeleportCamera();
	}

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

ADOFCharacter* ADOFPlayerController::GetControlledCharacter()
{
	controlledCharacter = Cast<ADOFCharacter>(GetPawn());

	return controlledCharacter;
}

void ADOFPlayerController::PossessAvatar()
{
	ADOFSpectatorPawn *specPawn = Cast<ADOFSpectatorPawn>(GetPawn());

	if (specPawn && controlledCharacter)
	{
		FVector newLocation = specPawn->GetActorLocation();
		newLocation.Z -= controlledCharacter->GetDefaultHalfHeight();

		FRotator newRotation = specPawn->GetControlRotation();
		newRotation.Pitch = 0.f;

		ToDestroyOnPossess(specPawn);
		ServerPossessAvatar(newLocation, newRotation);

		PlayerState->bIsSpectator = false;

#ifdef UE_EDITOR

		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Possessed"));
		}

#endif
	}
}

void ADOFPlayerController::UnPossessAvatar()
{
	UWorld *world = GetWorld();

	GetControlledCharacter();

	if (world && controlledCharacter && !PlayerState->bIsSpectator)
	{
		FActorSpawnParameters spawnParams;
		spawnParams.Owner = this;
		spawnParams.Instigator = Instigator;

		FVector spawnLocation;
		FRotator spawnRotation;

		spawnLocation = controlledCharacter->GetCameraLocation();
		spawnRotation = controlledCharacter->GetCameraRotation();

		ADOFSpectatorPawn *specPawn;

		specPawn = world->SpawnActor<ADOFSpectatorPawn>(spawnLocation, spawnRotation, spawnParams);

		Possess(specPawn);

		PlayerState->bIsSpectator = true;

#ifdef UE_EDITOR

		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("UnPossessed"));
		}

#endif
	}
}

bool ADOFPlayerController::ServerPossessAvatar_Validate(FVector newLocation, FRotator newRotation)
{
	return true;
}

void ADOFPlayerController::ServerPossessAvatar_Implementation(FVector newLocation, FRotator newRotation)
{
	Possess(controlledCharacter);
	controlledCharacter->SetActorLocationAndRotation(newLocation, newRotation);
	controlledCharacter->TeleportCamera();
}
