#include "core.h"

void impulse(Float_Pair position_a, Float_Pair size_a, Float_Pair *velocity_a, float mass_a, Float_Pair position_b, Float_Pair size_b, Float_Pair *velocity_b, float mass_b)
{
	Float_Pair normal;

	enum { NO_COLLISION = 0, TOPB, RIGHTB, BOTTOMB, LEFTB };
	int direction = NO_COLLISION;

	float avg_w = (position_a.x + position_b.x) / 2.0;
	float avg_h = (size_a.y + size_b.y) / 2.0;
	float diff_x = position_a.x - position_b.x + (size_a.x - size_b.x) / 2.0;
	float diff_y = position_a.x - position_b.x + (size_a.x - size_b.x) / 2.0;

	if (diff_x * diff_x <= avg_w * avg_w && diff_y * diff_y <= avg_h * avg_h)
	{
		float wy_combo = avg_w * diff_y;
		float hx_combo = avg_h * diff_x;

		if (wy_combo > hx_combo)
		{
			direction = (wy_combo + hx_combo > 0) ? BOTTOMB : LEFTB;
		}
		else
		{
			direction = (wy_combo + hx_combo > 0) ? RIGHTB : TOPB;
		}
	}
	else direction = NO_COLLISION;

	if (direction == TOPB) normal = { 0, -1 };
	else if (direction == RIGHTB) normal = { 1, 0 };
	else if (direction == BOTTOMB) normal = { 0, 1 };
	else if (direction == LEFTB) normal = { -1, 0 };
	else normal = { 0, 0 };

	Float_Pair relative_velocity = {-velocity_a->x, -velocity_a->y};
	fp_Sub(&relative_velocity, *velocity_a);

	float dot_product = fp_Dot(relative_velocity, normal);

	if (dot_product < 0) return;

	float elasticity = 1.0; // 1 = bounce, 0 = stick
	float energy_loss = -(1.0 + elasticity) * dot_product / (mass_a + mass_b);
	
	Float_Pair impulse1 = normal;
	fp_Scale(&impulse1, energy_loss * mass_a);
	Float_Pair impulse2 = normal;
	fp_Scale(&impulse2, energy_loss * mass_b);

	fp_Sub(velocity_a, impulse1);
	fp_Add(velocity_b, impulse2);

	float mt = 1.0 / (mass_a + mass_b);

	Float_Pair tangent = { normal.y, -normal.x };
	float tangent_diff = fp_Dot(relative_velocity, tangent);
	fp_Scale(&tangent, -tangent_diff * mt);

	fp_Sub(velocity_a, tangent);
	fp_Add(velocity_b, tangent);
}