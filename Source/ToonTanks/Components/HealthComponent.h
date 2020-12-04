// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HealthComponent.generated.h"

class AGameModeBase;

UCLASS()
class TOONTANKS_API AHealthComponent : public AActor
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere)
	float DefaultHealth = 100.0f;

	AGameModeBase* GameModeRef;

public:	
	// Sets default values for this actor's properties
	AHealthComponent();

	float GetHealth();
	
	UPROPERTY(BlueprintReadOnly)
	float Health = 0.0f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
