/*
* Copyright (c) 2019-2022, NVIDIA CORPORATION.  All rights reserved.
*
* NVIDIA CORPORATION and its licensors retain all intellectual property
* and proprietary rights in and to this software, related documentation
* and any modifications thereto.  Any use, reproduction, disclosure or
* distribution of this software and related documentation without an express
* license agreement from NVIDIA CORPORATION is strictly prohibited.
*/

#include "CoreMinimal.h"
#include "HAL/IConsoleManager.h"
#include "UnrealEdGlobals.h"
#include "Editor.h"
#include "Selection.h"
#include "GameFramework/Actor.h"
#include "DDGIVolumeComponent.h"

static void RTXGICaptureBakedIrradiance(const TArray<FString>& Args)
{
	if (!GEditor)
	{
		return;
	}
	if (Args.Num() < 1)
	{
		UE_LOG(LogTemp, Warning, TEXT("Usage: rtxgi.CaptureBakedIrradiance <SlotIndex> — select actor(s) with a DDGI Volume component, then run this in the editor console (~)."));
		return;
	}

	const int32 SlotIndex = FCString::Atoi(*Args[0]);
	int32 NumCaptured = 0;

	for (auto It = GEditor->GetSelectedActorIterator(); It; ++It)
	{
		AActor* const Actor = Cast<AActor>(*It);
		if (!Actor)
		{
			continue;
		}
		if (UDDGIVolumeComponent* const DDGI = Actor->FindComponentByClass<UDDGIVolumeComponent>())
		{
			DDGI->CaptureIrradianceToBakedTimeOfDaySlot(SlotIndex);
			++NumCaptured;
			UE_LOG(LogTemp, Display, TEXT("rtxgi.CaptureBakedIrradiance: slot %d — %s"), SlotIndex, *Actor->GetActorLabel());
		}
	}

	if (NumCaptured == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("rtxgi.CaptureBakedIrradiance: no selected actor has a DDGI Volume component."));
	}
}

static FAutoConsoleCommand GRTXGICaptureBakedIrradianceCmd(
	TEXT("rtxgi.CaptureBakedIrradiance"),
	TEXT("Editor: copy current DDGI ProbesIrradiance into baked time-of-day slot for each selected actor that has a DDGI Volume component. Args: <SlotIndex>"),
	FConsoleCommandWithArgsDelegate::CreateStatic(&RTXGICaptureBakedIrradiance),
	ECVF_Default);
