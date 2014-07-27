

#pragma once

#include "GameFramework/GameMode.h"
#include "DOFGameMode.generated.h"

/**
 * 
 */
UCLASS()
class DECLINEOFFEAR_API ADOFGameMode : public AGameMode
{
	GENERATED_UCLASS_BODY()

	virtual void BeginPlay() override;
};
