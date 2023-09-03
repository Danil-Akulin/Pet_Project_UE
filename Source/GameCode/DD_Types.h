#pragma once

#define ECC_Climbing ECC_GameTraceChannel1
#define ECC_InteractionVolume ECC_GameTraceChannel2

const FName CollisionProfilePawn = FName("Pawn");
const FName CollisionProfileRegdoll = FName("Ragdoll");

const FName SocketCharacterWeapon = FName("WeaponSocket"); 
const FName SocketWeaponMuzzle = FName("Muzzle_Socket");

const FName CollisionProfilePawnInteractionVolume = FName("PawnInteractionVolume");

const FName DebugCategoryLedgeDetection = FName("LedgeDetection");

const FName DebugCategoryCharacterAttributes = FName("CharacterAttributes");

UENUM(BlueprintType)
enum class EEquippedItemType : uint8
{
	None,
	Pistol
};