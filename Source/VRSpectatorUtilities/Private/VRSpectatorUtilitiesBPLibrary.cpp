// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "VRSpectatorUtilitiesBPLibrary.h"
#include "VRSpectatorUtilities.h"

#if WITH_EDITOR
#include "Editor/UnrealEd/Classes/Editor/EditorEngine.h"
#endif

#include "Runtime/HeadMountedDisplay/Public/HeadMountedDisplayFunctionLibrary.h"
#include "Runtime/Engine/Classes/GameFramework/PlayerController.h"
#include "Runtime/Engine/Public/Slate/SceneViewport.h"
#include "Runtime/Engine/Classes/Engine/GameEngine.h"

UVRSpectatorUtilitiesBPLibrary::UVRSpectatorUtilitiesBPLibrary(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

const FVector2D UVRSpectatorUtilitiesBPLibrary::GetAbsoluteLocationForCursorWidgetFromMousePosition(APlayerController* PlayerController, const FVector2D WidgetSize)
{
	float mouse_x;
	float mouse_y;
	PlayerController->GetMousePosition(mouse_x, mouse_y);
	const FVector2D DisplaySize = GetSpectatorDisplayResolution();
	FVector2D MousePositionPercentage = FVector2D(mouse_x / DisplaySize.X, mouse_y / DisplaySize.Y);
	return MousePositionPercentage * WidgetSize;
}

const FVector2D UVRSpectatorUtilitiesBPLibrary::OffsetCursorWidgetFromMouseLocationForMiddlePivot(const FVector2D AbsoluteCursorLocation, const FVector2D TargetResolution)
{
	FVector2D Result = TargetResolution / 2.f - AbsoluteCursorLocation;
	return Result;
}

const FVector2D UVRSpectatorUtilitiesBPLibrary::GetSpectatorDisplayResolution()
{
	FSceneViewport* sceneViewport = nullptr;

	if (!GIsEditor)
	{
		UGameEngine* GameEngine = Cast<UGameEngine>(GEngine);
		sceneViewport = GameEngine->SceneViewport.Get();
	}
#if WITH_EDITOR
	else
	{
		UEditorEngine* EditorEngine = CastChecked<UEditorEngine>(GEngine);
		FSceneViewport* PIEViewport = (FSceneViewport*)EditorEngine->GetPIEViewport();
		if (PIEViewport != nullptr && PIEViewport->IsStereoRenderingAllowed())
		{
			sceneViewport = PIEViewport;
		}
		else
		{
			FSceneViewport* EditorViewport = (FSceneViewport*)EditorEngine->GetActiveViewport();
			if (EditorViewport != nullptr && EditorViewport->IsStereoRenderingAllowed())
			{
				sceneViewport = EditorViewport;
			}
		}
	}
#endif
	if (sceneViewport != nullptr)
	{
		return sceneViewport->FindWindow()->GetSizeInScreen();
	}

	return FVector2D();
}


