

#pragma once

#include "GameFramework/PlayerController.h"
#include "DOFPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class DECLINEOFFEAR_API ADOFPlayerController : public APlayerController
{
	GENERATED_UCLASS_BODY()

	APawn *toDestroy;

public:

	void toDestroyOnPossess(APawn *pawn);

protected:

	virtual void Possess(class APawn* inPawn) override;
};
