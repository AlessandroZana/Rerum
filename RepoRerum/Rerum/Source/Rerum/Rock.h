// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PaperFlipbookComponent.h"
#include "Rock.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class RERUM_API URock : public UPaperFlipbookComponent
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	class UPaperFlipbook* rockDestruction;
	
	void SetRockDestructionAnimation();
	
};
