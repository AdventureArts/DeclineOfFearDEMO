

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
	bool Crouching = false;

	float forwardSpeed = 0.f;
	float rightSpeed = 0.f;
	float cameraDistance = 50.f;

	FName cameraBone;
	bool cameraBoneAttached = false;
	float runningCameraOffset = 30.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
	float walkSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
	float runSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
	float CrouchSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	USpringArmComponent* supportPivot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	USpringArmComponent* cameraSupport;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	UCameraComponent* playerCamera;

public:

	UFUNCTION(BlueprintCallable, Category = Movement)
	bool IsRunning();

	UFUNCTION(BlueprintCallable, Category = Movement)
	bool IsCrouching();

	void SetForwardSpeed(float speed);
	void SetRightSpeed(float speed);

	void UnPossessMe();
	void TeleportCamera(FVector loc, FRotator rot);
	void TeleportCamera();

	FVector GetCameraLocation();
	FRotator GetCameraRotation();

	UFUNCTION(BlueprintCallable, Category = Camera)
	void AttachCameraToBone(FName boneName);

	UFUNCTION(BlueprintCallable, Category = Camera)
	void DetachCameraFromBone();

	UFUNCTION(BlueprintCallable, Category = Camera)
	void SetRunningCameraOffset(float offset);

protected:

	virtual void SetupPlayerInputComponent(class UInputComponent *InputComponent) override;

	virtual void BeginPlay() override;

	void UpdateMovementDirection();
	void InterpolateCameraDistance(float delta);

	virtual void UpdateCameraAnimation();

	virtual void Tick(float DeltaSeconds) override;

	virtual void LookUp(float val);
	virtual void Turn(float val);
	virtual void WalkForward(float val);
	virtual void WalkRight(float val);
	virtual void CameraZoomIn();
	virtual void CameraZoomOut();
	virtual void StartRunning();
	virtual void StopRunning();
	virtual void StartCrouching();
	virtual void StopCrouching();

	UFUNCTION(Reliable, Server, WithValidation)
	virtual void ServerStartCrouching();

	UFUNCTION(Reliable, Server, WithValidation)
	virtual void ServerStopCrouching();

	UFUNCTION(Reliable, Server, WithValidation)
	virtual void ServerStartRunning();

	UFUNCTION(Reliable, Server, WithValidation)
	virtual void ServerStopRunning();

};
