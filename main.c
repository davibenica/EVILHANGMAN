#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "generic_vector.h"
#include "AVL.h"


GENERIC_VECTOR* create_dictionary(void);
void string_destroy(ITEM* phString);
ITEM string_init_copy(ITEM hArg);
Status get_word_key_value(MY_STRING current_word_family, MY_STRING new_key, MY_STRING word,  char guess);
void clear_buffer(void);
int prompt_get_word_lenght(GENERIC_VECTOR* dictionary);
int prompt_number_guesses(void);
Boolean prompt_running_total(void);
char make_guess(void);
void play_evil_hangman(void);
Boolean prompt_play_again(void);

int main(int agrc, char* argv[])
{

	play_evil_hangman();
	return 0;
}

void play_evil_hangman(void)
{
	
	Boolean keep_playing;
	int i;

	printf("╔═══╗───╔╗─╔╗─╔╗\n");
    printf("║╔══╝───║║─║║─║║\n");
    printf("║╚══╦╗╔╦╣║─║╚═╝╠══╦═╗╔══╦╗╔╦══╦═╗\n");
    printf("║╔══╣╚╝╠╣║─║╔═╗║╔╗║╔╗╣╔╗║╚╝║╔╗║╔╗╗\n");
    printf("║╚══╬╗╔╣║╚╗║║─║║╔╗║║║║╚╝║║║║╔╗║║║║\n");
    printf("╚═══╝╚╝╚╩═╝╚╝─╚╩╝╚╩╝╚╩═╗╠╩╩╩╝╚╩╝╚╝\n");
    printf("─────────────────────╔═╝║\n");
    printf("─────────────────────╚══╝\n");
	

	printf("\n\nWelcome to Evil Hangman!\n\n");

	
	GENERIC_VECTOR* dictionary = create_dictionary();
	TREE hTree = tree_init_default();
	MY_STRING guesses = my_string_init_c_string(" ");
	MY_STRING current_word_family = my_string_init_default();
	
	int word_length = prompt_get_word_lenght(dictionary);
	int guesses_left = prompt_number_guesses();
	Boolean running_total = prompt_running_total();

	for ( i = 0; i < word_length; i++)
	{
		my_string_push_back(current_word_family, '-');

	}

	printf("The game will now start!\n");
	printf("You have %d guesses!\n", guesses_left);
	printf("The word is %d letters long!\n", word_length);

	while(guesses_left > 0)
	{
		char guess = make_guess();

		// check if that letter has already been guessed
		for (int i = 0; i < my_string_get_size(guesses); i++)
		{
			if (guess == *my_string_at(guesses, i))
			{
				printf("That letter has already been guess pick another\n");
				guess = make_guess();
			}
		}

		guesses_left--;
		my_string_push_back(guesses, guess);
	
		for (i = 0; i < generic_vector_get_size(dictionary[word_length]); i++)
		{

			MY_STRING key = my_string_init_default();
			get_word_key_value(current_word_family, key, generic_vector_at(dictionary[word_length], i), guess);
			tree_push(hTree, key, generic_vector_at(dictionary[word_length], i));
			my_string_destroy(&key);
		}

	
		GENERIC_VECTOR largest_family = get_largest_family(hTree);
		int largest_family_size = generic_vector_get_size(largest_family);

		

		if (running_total == TRUE)
		{
			printf("There are %d words left in the word list!\n", largest_family_size);
		}

		generic_vector_destroy(&dictionary[word_length]);
		dictionary[word_length] = generic_vector_init_default(my_string_init_copy, my_string_destroy);

		for (i = 0; i < largest_family_size; i++)
		{
			generic_vector_push_back(dictionary[word_length], generic_vector_at(largest_family, i));
		}
		

		
		MY_STRING new_key = my_string_init_default();
		get_word_key_value(current_word_family, new_key, generic_vector_at(dictionary[word_length], 0), guess);


		if (my_string_compare(new_key, current_word_family) == 0)
		{
			printf("You guessed incorectly!\n");
			printf("The letter %c was not in the word\n", guess);
			printf("The current key is: %s\n", my_string_c_str(new_key));
		}
		else
		{
			printf("You guessed correctly!\n");
			printf("The key is now %s\n", my_string_c_str(new_key));
			my_string_assignment(current_word_family, new_key);
		}

		printf("Letters guessed: %s\n", my_string_c_str(guesses));

		my_string_destroy(&new_key);

		tree_destroy(&hTree);

		hTree = tree_init_default();


		// Check if the word has been guessed
		if(my_string_compare(generic_vector_at(dictionary[word_length], 0), current_word_family) == 0)
		{
			printf("Congrats!!\nYou guessed the word!\n");
			printf("The word was %s\n", my_string_c_str(generic_vector_at(dictionary[word_length], 0)));
			break;
		}
		
	}

	if (guesses_left == 0)
	{
    	printf("You lose the word was %s\n", my_string_c_str(generic_vector_at(dictionary[word_length], 0)));
	}

	my_string_destroy(&guesses);
	my_string_destroy(&current_word_family);
	tree_destroy(&hTree);
	
	for(int i =0; i < 30; i++)
	{
		generic_vector_destroy(&dictionary[i]);
	}
	free(dictionary);

	// Prompt to play again
	keep_playing = prompt_play_again();

	while (keep_playing == TRUE)
	{
		play_evil_hangman();
	}
	return;
}

char make_guess(void)
{
	char c;
	printf("\nMake a guess: \n");
	scanf("%c", &c);
	clear_buffer();
	if (isalpha(c))
	{
		c = tolower(c);
	}
	else
	{
		printf("Invalid guess please try again!\n");
		make_guess();
	}

	return c;
}

int prompt_get_word_lenght(GENERIC_VECTOR* dictionary)
{
	int letter;
	printf("How many letters do you want to play with?\n");
	scanf("%d", &letter);
	clear_buffer();
	
	
	
	while(letter < 3 || letter > 29 || generic_vector_get_size(dictionary[letter]) == 0)
	{
		printf("The number you picked is invalid please pick another number\n");
		scanf("%d", &letter);
		clear_buffer();

	}
	return letter;
}

int prompt_number_guesses(void)
{
	int guesses;
	printf("How many gueses do you want to have? (1-25)\n");
	scanf("%d", &guesses);
	clear_buffer();

	while(guesses < 1 || guesses > 26)
	{
		printf("Invalid number of guesses try again!\n");
		scanf("%d", &guesses);
		clear_buffer();
	}

	return guesses;
}

Boolean prompt_running_total(void)
{
	printf("Would you like to have a running total of words remainnig in the word list? (y-n)\n");
	char c;
	scanf("%c" , &c);
	clear_buffer();

	while(c != 'y' && c != 'n')
	{
		printf("Invalid input please try again!\n");
		scanf("%c", &c);
		clear_buffer();
	}
	
	if (c == 'y')
	{
		return TRUE;
	}

	return FALSE;
}

Boolean prompt_play_again(void)
{
	printf("\nWould you like to play again? (y-n)\n");
	char c;
	scanf("%c" , &c);
	clear_buffer();

	while(c != 'y' && c != 'n')
	{
		printf("Invalid input please try again!\n");
		scanf("%c", &c);
		clear_buffer();
	}
	
	if (c == 'y')
	{
		return TRUE;
	}

	return FALSE;
}

GENERIC_VECTOR* create_dictionary(void)
{
	GENERIC_VECTOR* pVector = (GENERIC_VECTOR*) malloc(sizeof(GENERIC_VECTOR) * 30);
	int i;
	MY_STRING hString = my_string_init_default();
	
	if (pVector == NULL)
	{
		return NULL;
	}

	for (i = 0; i < 30; i ++)
	{
		pVector[i] = generic_vector_init_default(my_string_init_copy, my_string_destroy);
		
		if (pVector[i] == NULL)
		{
			return NULL;		
		}
	}
	
	FILE* fp;
	fp = fopen("dictionary.txt", "r");
	if (fp == NULL)
	{
		return NULL;
	}

	while(my_string_extraction(hString, fp))
	{
		generic_vector_push_back(pVector[my_string_get_size(hString)], hString);
			
	}
	my_string_destroy(&hString);
	fclose(fp);	
	return pVector;
}

Status get_word_key_value(MY_STRING current_word_family, MY_STRING new_key, MY_STRING word,  char guess)
{

	int i;

	for (i = 0; i < my_string_get_size(word); i ++)
	{
		if (*my_string_at(word, i) == guess )
		{
		 my_string_push_back(new_key, *my_string_at(word, i));
		}
		else 
		{
		my_string_push_back(new_key, *my_string_at(current_word_family, i));
		}
	}
	return SUCCESS;
	
}

void clear_buffer(void)
{
	char c;
	scanf("%c", &c);

	while(c != '\n')
	{
		scanf("%c", &c);
	}
	return;
}

