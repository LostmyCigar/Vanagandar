// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/WeaponComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetStringLibrary.h"
#include "Engine/CollisionProfile.h"
#include "VanagandrPlayerController.h"
#include "ActorComponents/WeaponComponents/WeaponConstraint.h"
#include "ActorComponents/WeaponComponents/BulletSpawner.h"
#include "ActorComponents/WeaponComponents/BulletModifier.h"
#include "ActorComponents/WeaponComponents/WeaponEffect.h"
#include "ActorComponents/WeaponComponents/AimModifier.h"
#include "ActorComponents/WeaponComponents/FirePattern.h"


UWeaponComponent::UWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UWeaponComponent::TryFire(APlayerController* Controller, AVanagandrPlayerCharacter* ShootingCharacter, 
	FVector ShootWorldStartPoint, FVector AimDirection, EInputState inputState)
{
	if (!CheckConstraints() || 
		!GetActiveParams().CheckFirePattern(inputState)) 
	{
		return;
	}

	Fire(Controller, ShootingCharacter, ShootWorldStartPoint, AimDirection);
}

void UWeaponComponent::Fire(APlayerController* Controller, AVanagandrPlayerCharacter* ShootingCharacter, FVector ShootWorldStartPoint, FVector AimDirection)
{
	GetActiveParams().Fire(Controller, ShootingCharacter, ShootWorldStartPoint, AimDirection);
	OnFire.Broadcast(Controller, ShootingCharacter, ShootWorldStartPoint, AimDirection);
}


bool UWeaponComponent::CheckConstraints()
{
	if (!bCanFire)
		return false;

	for (int i = 0; i < FireConstraints.Num(); i++)
	{
		if (FireConstraints[i] != nullptr) {
			if (!FireConstraints[i]->CheckConstraint())
				return false;
		} else PRINT(FString("Constraint is nullptr"))
	}
	return true;
}


void UWeaponComponent::BeginPlay()
{
	Super::BeginPlay();
	
	BeginBehaviours();
}

//We should probly only tick if we are equipped
void UWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	TickBehaviours(DeltaTime);
}

//ToDo: Make this in a non-shitty way
//Decide when we want to update secondary vs primary behaviors
void UWeaponComponent::BeginBehaviours()
{
	AllBehaviours.Append(FireConstraints);

	AllBehaviours.Append(PrimaryFireParams.AimModifiers);
	AllBehaviours.Append(PrimaryFireParams.BulletModifiers);
	AllBehaviours.Add(PrimaryFireParams.BulletSpawn);
	AllBehaviours.Add(PrimaryFireParams.FirePattern);

	if (bHasSecondaryFireMode) {
		AllBehaviours.Append(SecondaryFireParams.AimModifiers);
		AllBehaviours.Append(SecondaryFireParams.BulletModifiers);
		AllBehaviours.Add(SecondaryFireParams.BulletSpawn);
		AllBehaviours.Add(SecondaryFireParams.FirePattern);
	}

	for (int i = 0; i < AllBehaviours.Num(); i++)
	{
		if (AllBehaviours[i] != nullptr) {
			AllBehaviours[i]->Begin(this, GetWorld());
		}
	}
}

void UWeaponComponent::TickBehaviours(float DeltaTime)
{
	for (int i = 0; i < AllBehaviours.Num(); i++)
	{
		if (AllBehaviours[i])
			AllBehaviours[i]->Tick(DeltaTime, this);
	}
}


//////////////// FireParams ////////////////

FFireParams::FFireParams()
{
}

void FFireParams::ApplyAimModifiers(APlayerController* Controller, AVanagandrPlayerCharacter* ShootingCharacter)
{
	for (int i = 0; i < AimModifiers.Num(); i++)
	{
		if (AimModifiers[i] != nullptr) {
			AimModifiers[i]->ApplyModifier(Controller, ShootingCharacter);
		}
	}
}

FVector FFireParams::ApplyBulletModifiers(APlayerController* Controller, AVanagandrPlayerCharacter* ShootingCharacter, FVector& AimDirection)
{
	auto aimDir = AimDirection;
	for (int i = 0; i < BulletModifiers.Num(); i++)
	{
		if (BulletModifiers[i] != nullptr) {
			aimDir = BulletModifiers[i]->ModifyBullet(Controller, ShootingCharacter, aimDir);
		}
	}

	return aimDir;
}

bool FFireParams::CheckFirePattern(EInputState inputState)
{
	if (FirePattern)
		return FirePattern->CheckFirePattern(inputState);
	return false;
}

void FFireParams::PlayEffects(FBulletResults& bulletResults)
{
	for (int i = 0; i < WeaponEffects.Num(); i++)
	{
		if (WeaponEffects[i] != nullptr) {
			WeaponEffects[i]->PlayEffect(bulletResults);
		}
	}
}



//float FFireParams::CalculateDamage(FHitResult hitResult)
//{
//	float currentDistanceCheck = 0;
//	float damageMultiplier = 1;
//	float distanceInMeters = hitResult.Distance / 100;
//
//	//Checks all designer set values for falloff and multipliers
//	for (const TPair<float, float >& pair : DamageParams.FallOfDamageMultiplierAtDistance)
//	{
//		if (currentDistanceCheck > pair.Value)
//			continue;
//
//		if (distanceInMeters <= pair.Value) {
//			damageMultiplier = pair.Key;
//			currentDistanceCheck = pair.Value;
//		}
//	}
// 
//
//	//Hate this kinda code but also, its a name check, how else would i do this? 
//	if (hitResult.BoneName == HeadBone)
//		return DamageParams.HeadDamage * damageMultiplier;
//
//	if (hitResult.BoneName == BodyBone)
//		return DamageParams.BodyDamage * damageMultiplier;
//
//	if (hitResult.BoneName == ArmBone)
//		return DamageParams.ArmDamage * damageMultiplier;
//
//	if (hitResult.BoneName == LegBone)
//		return DamageParams.LegDamage * damageMultiplier;
//
//	//this means if we hit static things such as boxes we wont do distance falloff
//	return DamageParams.BaseDamage;
//}
//
//void FFireParams::ApplyDamage(APlayerController* playerController, FBulletResults& bulletResults)
//{
//	auto PC = Cast<AVanagandrPlayerController>(playerController);
//	if (!PC)
//		return;
//
//	for (int i = 0; i < bulletResults.HitResults.Num(); i++)
//	{
//		auto hitActor = bulletResults.HitResults[i].GetActor();
//		auto bulletStartPoint = bulletResults.HitResults[i].TraceStart;
//
//		float Damage = CalculateDamage(bulletResults.HitResults[i]);
//
//		PC->Server_ApplyDamage_Implementation(hitActor, Damage, bulletStartPoint, bulletResults.HitResults[i]);
//	}
//	
//}


void FFireParams::Fire(APlayerController* Controller, AVanagandrPlayerCharacter* ShootingCharacter, FVector ShootWorldStartPoint, FVector AimDirection)
{
	if (!BulletSpawn) {
		return;
	}

	auto AimDir = ApplyBulletModifiers(Controller, ShootingCharacter, AimDirection);
	
	auto bulletResults = BulletSpawn->CreateBullet(Controller, ShootingCharacter, ShootWorldStartPoint, AimDir);
	ApplyAimModifiers(Controller, ShootingCharacter);

	//Send bullet results to server 
	//CallDamageOnServer(Controller, bulletResults);
}
