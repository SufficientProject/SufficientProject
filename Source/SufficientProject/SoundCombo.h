#pragma once

enum class sound_shot : int { LOW, MED, HIGH, HIGHEST };

enum class sound_combo_effect : int { FAILED, GOOD, NONE, DMG, HEAL };

const size_t comboSize = 5, genericComboCount = 5;

sound_shot genericCombos[genericComboCount][comboSize] =
{
	{sound_shot::LOW, sound_shot::LOW, sound_shot::LOW, sound_shot::HIGHEST, sound_shot::HIGH},

	{sound_shot::LOW, sound_shot::HIGH, sound_shot::LOW, sound_shot::HIGH, sound_shot::HIGHEST},

	{sound_shot::MED, sound_shot::MED, sound_shot::HIGH, sound_shot::HIGHEST, sound_shot::LOW},

	{sound_shot::LOW, sound_shot::MED, sound_shot::MED, sound_shot::HIGH, sound_shot::HIGHEST},

	{sound_shot::HIGHEST, sound_shot::LOW, sound_shot::LOW, sound_shot::MED, sound_shot::HIGH}

};

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
			delete [] soundSequence;
		}

		sound_combo_effect checkCombo(sound_shot st, size_t pos)
		{
				if (soundSequence[pos] == st)
				{
					return sound_combo_effect::GOOD;

					if (pos == comboSize)
					{
						return comboEffect
					}

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

class SoundComboObserver
{
	size_t comboCount, delayTime, possibleSequences;

	SoundCombo *comboList;

	void reset();

public:

	SoundComboObserver();
	SoundComboObserver(size_t, size_t);

	~SoundComboObserver();

	sound_combo_effect check();

	size_t getComboCount();
	size_t getDelayTime();

	void setDelayTime(size_t);


};

// How this should work?
// I will write soon... XD too late now.
// Player will be let to use items only after playing sound sequence, each item is going to have a corresponding, unique sequence.