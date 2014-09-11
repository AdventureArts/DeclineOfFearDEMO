

#pragma once

#include "GameFramework/Character.h"
#include "DOFCharacter.generated.h"

/**
 * 
 */
UCLASS()
class DECLINEOFFEAR_API ADOFCharacter : public ACharacter
{
	GENERATED_UCLASS_BODY()

	bool running = false;

	float forwardSpeed = 0.f;
	float rightSpeed = 0.f;
	float cameraDistance = 50.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
	float walkSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
	float runSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	TSubobjectPtr<class USpringArmComponent> supportPivot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	TSubobjectPtr<class USpringArmComponent> cameraSupport;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	TSubobjectPtr<class UCameraComponent> playerCamera;

public:

	bool IsRunning();

	void SetForwardSpeed(float speed);
	void SetRightSpeed(float speed);

	void UnPossessMe();
	void TeleportCamera(FVector loc, FRotator rot);
	void TeleportCamera();

	FVector GetCameraLocation();
	FRotator GetCameraRotation();

protected:

	virtual void SetupPlayerInputComponent(class UInputComponent *InputComponent) override;

	virtual void BeginPlay() override;

	void UpdateMovementDirection();
	void InterpolateCameraDistance(float delta);

	virtual void Tick(float DeltaSeconds) override;

	virtual void LookUp(float val);
	virtual void Turn(float val);
	virtual void WalkForward(float val);
	virtual void WalkRight(float val);
	virtual void CameraZoomIn();
	virtual void CameraZoomOut();
	virtual void StartRunning();
	virtual void StopRunning();

	UFUNCTION(Reliable, Server, WithValidation)
	virtual void ServerStartRunning();

	UFUNCTION(Reliable, Server, WithValidation)
	virtual void ServerStopRunning();

};
