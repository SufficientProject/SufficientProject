#pragma once

enum class sound_shot : int { LOW, MED, HIGH, HIGHEST };

enum class sound_combo_effect : int { NONE, FAILED, DMG, HEAL };

class SoundCombo
{
	size_t comboSize;
	sound_shot *soundSequence;

public:

	SoundCombo();

	~SoundCombo();

};

class SoundComboObserver
{
	size_t comboCount, delayTime, possibleSequences;

	void reset();

public:

	SoundComboObserver();
	SoundComboObserver(size_t, size_t);

	~SoundComboObserver();

	sound_combo_effect check();
	sound_shot peek_next();

	size_t getComboCount();
	size_t getDelayTime();

	void setDelayTime(size_t);
	void setComboCount(size_t);
};

// How this should work?
// I will write soon... XD too late now.
// Player will be let to use items only after playing sound sequence, each item is going to have a corresponding, unique sequence.