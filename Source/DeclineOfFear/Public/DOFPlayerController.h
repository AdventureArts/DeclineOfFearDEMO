

#pragma once

#include "GameFramework/PlayerController.h"
#include "DOFCharacter.h"
#include "DOFPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class DECLINEOFFEAR_API ADOFPlayerController : public APlayerController
{
	GENERATED_UCLASS_BODY()

	APawn *toDestroy;

	ADOFCharacter *controlledCharacter;

public:

	void toDestroyOnPossess(APawn *pawn);

protected:

	virtual void BeginPlay();

	virtual void SetupControllerInputComponent(UInputComponent* InputComponent);

	virtual void Possess(class APawn* inPawn) override;
	virtual void UnPossessAvatar();
};
