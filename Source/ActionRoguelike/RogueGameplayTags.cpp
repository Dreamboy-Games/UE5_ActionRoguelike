#include "RogueGameplayTags.h"

namespace SharedGameplayTags {
	UE_DEFINE_GAMEPLAY_TAG(StatusEffect_Attacking, "StatusEffect.Attacking");
	UE_DEFINE_GAMEPLAY_TAG(StatusEffect_Sprinting, "StatusEffect.Sprinting");
	UE_DEFINE_GAMEPLAY_TAG(StatusEffect_Stunned, "StatusEffect.Stunned");
	
	UE_DEFINE_GAMEPLAY_TAG(Action_PrimaryAttack, "Action.Attack.Primary");
	UE_DEFINE_GAMEPLAY_TAG(Action_SecondaryAttack, "Action.Attack.Secondary");
	UE_DEFINE_GAMEPLAY_TAG(Action_SpecialAttack, "Action.Attack.Special");
	UE_DEFINE_GAMEPLAY_TAG(Action_Sprint, "Action.Sprint");
	
	UE_DEFINE_GAMEPLAY_TAG(Attribute_Health, "Attribute.Health");
	UE_DEFINE_GAMEPLAY_TAG(Attribute_HealthMax, "Attribute.HealthMax");
	UE_DEFINE_GAMEPLAY_TAG(Attribute_MoveSpeed, "Attribute.MoveSpeed");
	UE_DEFINE_GAMEPLAY_TAG(Attribute_MoveSpeedMultiplier, "Attribute.MoveSpeedMultiplier");
	UE_DEFINE_GAMEPLAY_TAG(Attribute_Rage, "Attribute.Rage");
}