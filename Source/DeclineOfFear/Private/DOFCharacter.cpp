

#include "DeclineOfFear.h"
#include "DOFCharacter.h"


ADOFCharacter::ADOFCharacter(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	walkSpeed = 150.f;
	runSpeed = 400.f;
	CrouchSpeed = 100.f;

	cameraBone = FName(TEXT("None"));

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
	supportPivot->bUsePawnControlRotation = true;

	cameraSupport = PCIP.CreateDefaultSubobject<USpringArmComponent>(this, TEXT("CameraSupport"));
	cameraSupport->AttachTo(supportPivot);
	cameraSupport->SetRelativeLocation(FVector(0.f, 50.f, 0.f));
	cameraSupport->TargetArmLength = cameraDistance;
	cameraSupport->bUsePawnControlRotation = false;

	playerCamera = PCIP.CreateDefaultSubobject<UCameraComponent>(this, TEXT("PlayerCamera"));
	playerCamera->AttachTo(cameraSupport, USpringArmComponent::SocketName);
	playerCamera->bUsePawnControlRotation = false;

	bUseControllerRotationYaw = false;
}

void ADOFCharacter::BeginPlay()
{
	
}

FVector ADOFCharacter::GetCameraLocation()
{
	return playerCamera->GetComponentLocation();
}

FRotator ADOFCharacter::GetCameraRotation()
{
	return playerCamera->GetComponentRotation();
}

void ADOFCharacter::TeleportCamera()
{
	supportPivot->PreviousDesiredLoc = FVector(0.f, 0.f, 0.f);
	supportPivot->PreviousDesiredRot = FRotator(0.f, 0.f, 0.f);
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

bool ADOFCharacter::IsCrouching()
{
	return Crouching;
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

	InputComponent->BindAction("Crouch", EInputEvent::IE_Pressed, this, &ADOFCharacter::StartCrouching);
	InputComponent->BindAction("Crouch", EInputEvent::IE_Released, this, &ADOFCharacter::StopCrouching);
	InputComponent->BindAxis("Turn", this, &ADOFCharacter::Turn);
	InputComponent->BindAxis("LookUp", this, &ADOFCharacter::LookUp);
	InputComponent->BindAxis("WalkForward", this, &ADOFCharacter::WalkForward);
	InputComponent->BindAxis("WalkRight", this, &ADOFCharacter::WalkRight);
	InputComponent->BindAction("CameraZoomIn", EInputEvent::IE_Released, this, &ADOFCharacter::CameraZoomIn);
	InputComponent->BindAction("CameraZoomOut", EInputEvent::IE_Released, this, &ADOFCharacter::CameraZoomOut);
	InputComponent->BindAction("Run", EInputEvent::IE_Pressed, this, &ADOFCharacter::StartRunning);
	InputComponent->BindAction("Run", EInputEvent::IE_Released, this, &ADOFCharacter::StopRunning);
}

void ADOFCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	UpdateMovementDirection();
	InterpolateCameraDistance(DeltaSeconds);
	UpdateCameraAnimation();
}

void ADOFCharacter::AttachCameraToBone(FName boneName)
{
	if (Mesh)
	{
		if (Mesh->GetBoneIndex(boneName) != INDEX_NONE)
		{
			cameraBone = boneName;
			cameraBoneAttached = true;
			supportPivot->CameraLagSpeed = 12.f;
		}
	}
}

void ADOFCharacter::DetachCameraFromBone()
{
	cameraBone = FName(TEXT("None"));
	cameraBoneAttached = false;
	supportPivot->CameraLagSpeed = 8.f;
}

void ADOFCharacter::SetRunningCameraOffset(float offset)
{
	if (offset >= 0.f) runningCameraOffset = offset;
}

void ADOFCharacter::UpdateCameraAnimation()
{
	if (cameraBoneAttached)
	{
		FVector boneLocation = Mesh->GetBoneLocation(cameraBone);

		supportPivot->SetWorldLocation(boneLocation);
	}
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
	float armLength, speed, runningFactor, runningOffset;

	armLength = cameraSupport->TargetArmLength;
	speed = CharacterMovement->Velocity.Size2D();
	runningFactor = (speed - walkSpeed) / (runSpeed - walkSpeed);
	runningOffset = runningCameraOffset * runningFactor;

	if (runningOffset < 0.f) runningOffset = 0.f;

	armLength = FMath::FInterpTo(armLength, cameraDistance + runningOffset, delta, 4.f);

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

void ADOFCharacter::StartCrouching()
{
	if (!Crouching)
	{
		Crouching = true;
		running = false;
		supportPivot->CameraLagSpeed = 12.f;
		CharacterMovement->MaxWalkSpeed = CrouchSpeed;
	}

	if (Role < ROLE_Authority) ServerStartCrouching();
}

void ADOFCharacter::StopCrouching()
{
	if (Crouching)
	{
		Crouching = false;
		supportPivot->CameraLagSpeed = 8.f;
		CharacterMovement->MaxWalkSpeed = walkSpeed;
	}

	if (Role < ROLE_Authority) ServerStopCrouching();
}

void ADOFCharacter::ServerStartCrouching_Implementation()
{
	StartCrouching();
}

void ADOFCharacter::ServerStopCrouching_Implementation()
{
	StopCrouching();
}

bool ADOFCharacter::ServerStartCrouching_Validate()
{
	return true;
}

bool ADOFCharacter::ServerStopCrouching_Validate()
{
	return true;
}

void ADOFCharacter::StartRunning()
{
	if (!running && !Crouching)
	{
		running = true;
		Crouching = false;
		supportPivot->CameraLagSpeed = 12.f;
		CharacterMovement->MaxWalkSpeed = runSpeed;
	}

	if (Role < ROLE_Authority) ServerStartRunning();
}

void ADOFCharacter::StopRunning()
{
	if (running)
	{
		running = false;
		supportPivot->CameraLagSpeed = 8.f;
		CharacterMovement->MaxWalkSpeed = walkSpeed;
	}

	if (Role < ROLE_Authority) ServerStopRunning();
}

void ADOFCharacter::ServerStartRunning_Implementation()
{
	StartRunning();
}

void ADOFCharacter::ServerStopRunning_Implementation()
{
	StopRunning();
}

bool ADOFCharacter::ServerStartRunning_Validate()
{
	return true;
}

bool ADOFCharacter::ServerStopRunning_Validate()
{
	return true;
}
