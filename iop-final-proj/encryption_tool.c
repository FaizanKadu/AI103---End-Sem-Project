#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

void caeserencrypt(char ce_text[], int ce_shift);
void caeserdecrypt(char cd_text[]);
int check_dictionary(const char *text);
void clean_input(char *text);

int check_dictionary(const char *text) 
{
    FILE *dictionary_file = fopen("dictionary.txt", "r");
    if (dictionary_file == NULL) 
    {
        fprintf(stderr, "\nERROR , Could not open dictionary.txt \n");
        fprintf(stderr, "Program must exit. Please ensure the file is present.\n");

        exit(1); 
    }

    char lower_word[1000]; 
    size_t len = strlen(text);
    if (len >= sizeof(lower_word)) len = sizeof(lower_word) - 1; 
    
    for (size_t k = 0; k < len; k++) 
    {
        lower_word[k] = tolower(text[k]);
    }
    lower_word[len] = '\0';

    char dict_word[100];
    while (fscanf(dictionary_file, "%99s", dict_word) == 1) 
    {
        if (strcmp(lower_word, dict_word) == 0) 
        {
            fclose(dictionary_file);
            return 1;
        }
    }

  
    fclose(dictionary_file);
    return 0; 
}


void caeserencrypt(char ce_text[], int ce_shift)
{
    int i = 0;
    ce_shift = (ce_shift % 26 + 26) % 26;

    while (ce_text[i] != '\0')
    {
        char current_character = ce_text[i];
        
        if (current_character >= 'A' && current_character <= 'Z')
        {
            int position = current_character - 'A';
            int encrypt = (position + ce_shift) % 26;
            current_character = 'A' + encrypt;
        }
        else if (current_character >= 'a' && current_character <= 'z')
        {
            int position = current_character - 'a';
            int encrypt = (position + ce_shift) % 26;
            current_character = 'a' + encrypt;
        }

        ce_text[i] = current_character;
        i++;
    }
    printf("\nYOUR ENCRYPTED MESSAGE IS - %s\n", ce_text);
}



void caeserdecrypt(char cd_text[])
{
    int best_key = -1;
    char best_decrypted[1000] = ""; 
    
    printf("\n--- Brute Force Decryption Attempts ---\n");
    for (int j = 0; j < 26; j++)
    {
        char decrypted[1000];
        int i = 0;
        
        while (cd_text[i] != '\0')
        {
            char current_character = cd_text[i];
            
            if (current_character >= 'A' && current_character <= 'Z')
            {
                int position = current_character - 'A';
                int decrypt = (position - j + 26) % 26; 
                current_character = 'A' + decrypt;
            }
            else if (current_character >= 'a' && current_character <= 'z')
            {
                int position = current_character - 'a';
                int decrypt = (position - j + 26) % 26;
                current_character = 'a' + decrypt;
            }
            
            decrypted[i] = current_character;
            i++;
        }
        decrypted[i] = '\0';

        int is_found = check_dictionary(decrypted);

        printf("Attempt %2d (Key: %d) - %s ", j, decrypted); 
        
        if (is_found) 
        {
            best_key = j;
            strcpy(best_decrypted, decrypted);
            printf(" EXACT MATCH FOUND!\n");

            break; 
        } else {
            printf("\n");
        }
    }

    printf("======================================================\n");
    printf("   AUTOMATED DICTIONARY ANALYSIS (Single Word)\n");
    printf("======================================================\n");
    if (best_key != -1) 
    {
        printf(" THE PLAINTEXT IS FOUND:\n");
        printf("  * Key Used: %d\n", best_key);
        printf("  * Plaintext: %s\n", best_decrypted);
    } 
    else 
    {
        printf(" No exact word match found in the dictionary for any attempt.\n");
    }
}

void clean_input(char *text)
{
    size_t len = strlen(text);
    if (len > 0 && text[len - 1] == '\n')
    {
        text[len - 1] = '\0';
    }
}

int main()
{
    int choice1, shift;
    char text[1000]; 

    printf("--CEASER CIPHER FUNCTION-- \n");
    printf("Choose 1 to ENCRYPT \n");
    printf("Choose 2 to DECRYPT (Single Word Brute-Force)\n");
    
    char choice_input[10];
    printf("Enter choice: ");
    if (fgets(choice_input, sizeof(choice_input), stdin) == NULL || sscanf(choice_input, "%d", &choice1) != 1) {
        printf("Invalid input. Exiting.\n");
        return 1;
    }

    while(choice1 < 1 || choice1 > 2) {
        printf("INVALID CHOICE, TRY AGAIN \n");
        printf("Choose 1 to ENCRYPT \n");
        printf("Choose 2 to DECRYPT \n");
        printf("Enter choice: ");
        if (fgets(choice_input, sizeof(choice_input), stdin) == NULL || sscanf(choice_input, "%d", &choice1) != 1) {
            printf("Invalid input. Exiting.\n");
            return 1;
        }
    }

    if (choice1 == 1){
        printf("Enter the string of text to encrypt (max 999 chars):\n> ");
        fgets(text, sizeof(text), stdin);
        clean_input(text); 

        printf("Enter shift key:\n> ");
        if (scanf("%d", &shift) != 1) {
            printf("Invalid input for key. Exiting.\n");
            return 1;
        }

        caeserencrypt(text, shift);
    }
    else{
        printf("Enter the SINGLE word to decrypt (max 999 chars):\n> ");
        fgets(text, sizeof(text), stdin);
        clean_input(text); 
        
        caeserdecrypt(text);
    }
    
    return 0;
}
