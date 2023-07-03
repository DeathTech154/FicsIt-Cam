#include "InitGameWorld.h"

#include "FGGameInstance.h"
#include "FGGameState.h"
#include "FGPlayerController.h"
#include "FICChatCommand.h"
#include "FICSubsystem.h"

UInitGameWorldFicsItCam::UInitGameWorldFicsItCam() {
	bRootModule = true;
	mChatCommands.Add(AFICChatCommand::StaticClass());
	ModSubsystems.Add(AFICSubsystem::StaticClass());
	ModSubsystems.Add(AFICEditorSubsystem::StaticClass());
}

#pragma optimize("", off)
void UInitGameWorldFicsItCam::DispatchLifecycleEvent(ELifecyclePhase Phase) {
	Super::DispatchLifecycleEvent(Phase);

	if (Phase == ELifecyclePhase::POST_INITIALIZATION) {
		UGameInstance* GameInstance = GetWorld()->GetGameInstance();
		UFGGameInstance* FGGameInstance = Cast<UFGGameInstance>(GameInstance);
		APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
		AFGPlayerController* FGPlayerController = Cast<AFGPlayerController>(PlayerController);
		Char_Player* Char_Player = FGPlayerController->GetCharacter()
		// Char_Player has the following:
		// Function StartFreeRotate3P() to start 3rd person rotate mode?
		// Function GetMesh3P() to pull 3rd person mesh -> Probably usable with SetOwnerNoSee
		// Function GetMesh1p() to pull 1st person mesh -> Probably usable with SetOwnerNoSee
		// Function GetCameraComponent() to access player camera component. Probably usefull to change Distance values.
		// Property SetOwnerNoSee -> Hide 3p to player, this is owned by some mesh. Have to obtain the correct one to toggle this.
		// SetFirstPersonMode() -> Using this with False may trigger 3rd person automatically doing StartFreeRotate3P or simply put out of body.
		AGameStateBase* GameState = GetWorld()->GetGameState();
		AFGGameState* FGGameState = Cast<AFGGameState>(GameState);
		UE_LOG(LogTemp, Warning, TEXT("%p %p %p %p %p %p"), GameInstance, FGGameInstance, PlayerController, FGPlayerController, GameState, FGGameState)
	}
}
#pragma optimize("", on)
