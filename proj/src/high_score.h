#ifndef HIGH_SCORE_H
#define HIGH_SCORE_H

/**
 * @brief struct High_Score
 *
 * unsigned long score, punctuation of the game
 * char name, twenty characters long
 * char date, twenty characters long
 */
typedef struct
{
	unsigned long score;
	char name[20];
	char date[20];
} High_Score;


/**
 * @brief Reads high scores from txt file
 *
 * @param path High Scores txt file
 *
 * @param high_scores[] High Scores array where high scores will be stored
 *
 * @param size Size of high_scores[] array
 *
 * @return Returns 0 upon succes and 1 if it can't open the file.
 */
int read_high_scores(const char* path, High_Score high_scores[], int size);


/**
 * @brief Writes high scores to txt file
 *
 * @param path High Scores txt file
 *
 * @param high_scores[] High Scores array where high scores will be stored
 *
 * @param size Size of high_scores[] array
 *
 * @return Returns 0 upon succes and 1 if it can't open the file.
 */
int save_high_scores(const char* path, High_Score high_scores[], int size);


/**
 * @brief Inserts a new score in the sorted high scores array
 *
 * @param high_scores[] High Scores array where high scores will be stored
 *
 * @param new_highscore New high score to be inserted
 */
void insert_sort_high_score(High_Score high_scores[], High_Score new_highscore);

#endif
