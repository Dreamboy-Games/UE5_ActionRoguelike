#include "RogueGameplayTags.h"

namespace SharedGameplayTags {
	UE_DEFINE_GAMEPLAY_TAG(Action_PrimaryAttack, "Action.Attack.Primary");
	UE_DEFINE_GAMEPLAY_TAG(Action_SecondaryAttack, "Action.Attack.Secondary");
	UE_DEFINE_GAMEPLAY_TAG(Action_SpecialAttack, "Action.Attack.Special");
	UE_DEFINE_GAMEPLAY_TAG(Action_Sprint, "Action.Sprint");
	
	UE_DEFINE_GAMEPLAY_TAG(Attribute_Health, "Attribute.Health");
	UE_DEFINE_GAMEPLAY_TAG(Attribute_HealthMax, "Attribute.HealthMax");
	UE_DEFINE_GAMEPLAY_TAG(Attribute_MoveSpeed, "Attribute.MoveSpeed");
}