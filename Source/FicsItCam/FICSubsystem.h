﻿#pragma once


#include "Subsystem/ModSubsystem.h"
#include "FICAnimation.h"
#include "FGSaveInterface.h"
#include "FICCameraCharacter.h"
#include "FICTimelapseCamera.h"
#include "UI/FICEditorContext.h"
#include "FICSubsystem.generated.h"

UCLASS()
class AFICSubsystem : public AModSubsystem, public IFGSaveInterface {
	GENERATED_BODY()
private:
	UPROPERTY()
	AFICCameraCharacter* Camera = nullptr;

	UPROPERTY()
	AFICAnimation* ActiveAnimation = nullptr;

	UPROPERTY()
	UFICEditorContext* EditorContext = nullptr;
	
public:
	UPROPERTY(BlueprintReadWrite, SaveGame, Category="FicsIt-Cam")
	TMap<FString, AFICAnimation*> StoredAnimations;

	UPROPERTY(BlueprintReadWrite, SaveGame, Category="FicsIt-Cam")
	TMap<FString, AFICTimelapseCamera*> TimelapseCameras;
	
	UPROPERTY()
	TSet<AFICAnimation*> VisibleAnimations;

	AFICSubsystem();
	
	// Begin AActor
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	// End AActor
	
	// Begin IFGSaveInterface
	virtual bool ShouldSave_Implementation() const override;
	// End IFGSaveInterface

	UFUNCTION(BlueprintCallable, Category="FicsIt-Cam", meta=(WorldContext = "WorldContextObject"))
	static AFICSubsystem* GetFICSubsystem(UObject* WorldContext);

	UFUNCTION(BlueprintCallable, Category="FicsIt-Cam")
	void PlayAnimation(AFICAnimation* Path, bool bDoRendering = false);
	
	UFUNCTION(BlueprintCallable, Category="FicsIt-Cam")
	void StopAnimation();
	
	void AddVisibleAnimation(AFICAnimation* Path);

	void SetActiveAnimation(AFICAnimation* ActiveAnimation);
	UFICEditorContext* GetEditor() const;

	void CreateCamera();
};
