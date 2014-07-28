

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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	TSubobjectPtr<class USpringArmComponent> supportPivot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	TSubobjectPtr<class USpringArmComponent> cameraSupport;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	TSubobjectPtr<class UCameraComponent> playerCamera;

public:



protected:

	virtual void SetupPlayerInputComponent(class UInputComponent *InputComponent) override;

	virtual void BeginPlay() override;

	void unPossessMe();

	virtual void Tick(float DeltaSeconds) override;

	virtual void LookUp(float val);
	virtual void Turn(float val);
	virtual void WalkForward(float val);
	virtual void WalkRight(float val);
	virtual void CameraZoomIn();
	virtual void CameraZoomOut();
};
