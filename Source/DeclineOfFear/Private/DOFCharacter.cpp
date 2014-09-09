

#include "DeclineOfFear.h"
#include "DOFCharacter.h"


ADOFCharacter::ADOFCharacter(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	walkSpeed = 150.f;
	runSpeed = 400.f;

	CharacterMovement->MaxAcceleration = 512.f;
	CharacterMovement->GroundFriction = 2.f;
	CharacterMovement->BrakingDecelerationWalking = 256.f;
	CharacterMovement->bOrientRotationToMovement = true;
	CharacterMovement->RotationRate.Yaw = 270.f;
	CharacterMovement->MaxWalkSpeed = walkSpeed;

	supportPivot = PCIP.CreateDefaultSubobject<USpringArmComponent>(this, TEXT("PivotSupport"));
	supportPivot->AttachTo(RootComponent);
	supportPivot->SetRelativeLocation(FVector(0.f, 0.f, 60.f));
	supportPivot->bEnableCameraLag = true;
	supportPivot->bEnableCameraRotationLag = true;
	supportPivot->CameraLagSpeed = 8.f;
	supportPivot->CameraRotationLagSpeed = 8.f;
	supportPivot->TargetArmLength = 50.f;
	supportPivot->bUseControllerViewRotation = true;

	cameraSupport = PCIP.CreateDefaultSubobject<USpringArmComponent>(this, TEXT("CameraSupport"));
	cameraSupport->AttachTo(supportPivot);
	cameraSupport->SetRelativeLocation(FVector(0.f, 50.f, 0.f));
	cameraSupport->TargetArmLength = cameraDistance;
	cameraSupport->bUseControllerViewRotation = false;

	playerCamera = PCIP.CreateDefaultSubobject<UCameraComponent>(this, TEXT("PlayerCamera"));
	playerCamera->AttachTo(cameraSupport, USpringArmComponent::SocketName);
	playerCamera->bUseControllerViewRotation = false;

	bUseControllerRotationYaw = false;
}

void ADOFCharacter::BeginPlay()
{
	
}

void ADOFCharacter::TeleportCamera(FVector loc, FRotator rot)
{
	supportPivot->PreviousDesiredLoc = loc;
	supportPivot->PreviousDesiredRot = rot;
}

bool ADOFCharacter::IsRunning()
{
	return running;
}

void ADOFCharacter::SetForwardSpeed(float speed)
{
	forwardSpeed = speed;
}

void ADOFCharacter::SetRightSpeed(float speed)
{
	rightSpeed = speed;
}

void ADOFCharacter::SetupPlayerInputComponent(class UInputComponent *InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

	check(InputComponent);

	//InputComponent->BindAction("UnPossessAvatar", IE_Released, this, &ADOFCharacter::UnPossessMe);

	InputComponent->BindAxis("Turn", this, &ADOFCharacter::Turn);
	InputComponent->BindAxis("LookUp", this, &ADOFCharacter::LookUp);
	InputComponent->BindAxis("WalkForward", this, &ADOFCharacter::WalkForward);
	InputComponent->BindAxis("WalkRight", this, &ADOFCharacter::WalkRight);
	InputComponent->BindAction("CameraZoomIn", EInputEvent::IE_Released, this, &ADOFCharacter::CameraZoomIn);
	InputComponent->BindAction("CameraZoomOut", EInputEvent::IE_Released, this, &ADOFCharacter::CameraZoomOut);
	InputComponent->BindAction("Run", EInputEvent::IE_Pressed, this, &ADOFCharacter::StartRunning);
	InputComponent->BindAction("Run", EInputEvent::IE_Released, this, &ADOFCharacter::StopRunning);
}

void ADOFCharacter::UnPossessMe()
{
	if (Controller != nullptr)
	{
		Controller->PlayerState->bIsSpectator = true;
		Controller->ChangeState(NAME_Spectating);

		Mesh->SetVisibility(false, true);

#ifdef UE_EDITOR

		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("UnPossessed"));
		}

#endif
	}
}

void ADOFCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	UpdateMovementDirection();
	InterpolateCameraDistance(DeltaSeconds);
}

void ADOFCharacter::UpdateMovementDirection()
{
	FVector movementDirection;

	movementDirection = FVector(forwardSpeed, rightSpeed, 0.f);
	movementDirection = GetControlRotation().RotateVector(movementDirection);
	movementDirection = movementDirection.SafeNormal2D();

	AddMovementInput(movementDirection, 1.f);
}

void ADOFCharacter::InterpolateCameraDistance(float delta)
{
	float armLength = cameraSupport->TargetArmLength;

	armLength = FMath::FInterpTo(armLength, cameraDistance, delta, 4.f);

	cameraSupport->TargetArmLength = armLength;
}

void ADOFCharacter::LookUp(float val)
{
	AddControllerPitchInput(val * - 60.0f * GetWorld()->GetDeltaSeconds());
}

void ADOFCharacter::Turn(float val)
{
	AddControllerYawInput(val * 40.0f * GetWorld()->GetDeltaSeconds());
}

void ADOFCharacter::WalkForward(float val)
{
	forwardSpeed = val;
}

void ADOFCharacter::WalkRight(float val)
{
	rightSpeed = val;
}

void ADOFCharacter::CameraZoomIn()
{
	cameraDistance -= 10;
}

void ADOFCharacter::CameraZoomOut()
{
	cameraDistance += 10;
}

void ADOFCharacter::StartRunning()
{
	if (!running)
	{
		running = true;

		CharacterMovement->MaxWalkSpeed = runSpeed;
	}
}

void ADOFCharacter::StopRunning()
{
	running = false;

	CharacterMovement->MaxWalkSpeed = walkSpeed;
}