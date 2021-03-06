// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "PaperFlipbookComponent.h"
#include "PaperFlipbookActor.generated.h"

UCLASS(MinimalAPI)
class APaperFlipbookActor : public AActor
{
	GENERATED_UCLASS_BODY()

private:
	UPROPERTY(Category = Sprite, VisibleAnywhere, BlueprintReadOnly, meta = (ExposeFunctionCategories = "Sprite,Rendering,Physics,Components|Flipbook", AllowPrivateAccess = "true"))
	class UPaperFlipbookComponent* RenderComponent;
public:

	// AActor interface
#if WITH_EDITOR
	virtual bool GetReferencedContentObjects(TArray<UObject*>& Objects) const override;
#endif
	// End of AActor interface

	/** Returns RenderComponent subobject **/
	FORCEINLINE class UPaperFlipbookComponent* GetRenderComponent() const { return RenderComponent; }
};

