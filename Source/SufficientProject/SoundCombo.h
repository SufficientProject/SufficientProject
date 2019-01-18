#pragma once

enum class sound_shot : int { LOW, MED, HIGH, HIGHEST };

enum class sound_combo_effect : int { FAILED, GOOD, NONE, DMG, HEAL, FULLHEAL };

class SoundCombo
{
private:

	sound_shot *soundSequence;
	size_t comboSize;

	sound_combo_effect comboEffect;

public:

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
			if (pos == comboSize)
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