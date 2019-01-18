#pragma once

enum sound_shot : int { LOW = 101, MED = 102, HIGH = 103, HIGHEST = 104 };

enum sound_combo_effect : int { FAILED = 0, GOOD = 1, NONE = 2, STAM = 3, FULLSTAM = 4, HEAL = 5, FULLHEAL = 6, DMG = 7 };

class SoundCombo
{
private:

	sound_shot *soundSequence;
	size_t comboSize;

	sound_combo_effect comboEffect;

public:
	SoundCombo() : comboSize(0), soundSequence(nullptr), comboEffect(sound_combo_effect::STAM)
	{

	}

	SoundCombo(size_t st, sound_shot *s, sound_combo_effect se) : comboSize(st), soundSequence(s), comboEffect(se)
	{

	}

	~SoundCombo()
	{
		delete[] soundSequence;
	}

	sound_combo_effect checkCombo(sound_shot st, size_t pos)
	{
		if (soundSequence[pos] == st)
		{
			if (pos == comboSize - 1)
			{
				return comboEffect;
			}

			return sound_combo_effect::GOOD;

		}

		return sound_combo_effect::FAILED;
	}

	sound_combo_effect getEffect()
	{
		return comboEffect;
	}

	void setEffect(sound_combo_effect ef)
	{
		comboEffect = ef;
	}
};