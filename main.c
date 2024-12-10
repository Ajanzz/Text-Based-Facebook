#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "a2_nodes.h"
#include "a2_functions.h"
#define USERNAME_LENGTH 30
#define PASSWORD_LENGTH 15
#define POST_LENGTH 250
#define POSTS_DISPLAYED 3

int main()
{
    FILE *csv_file = fopen("user_details.csv", "r");
    if (csv_file == NULL)
    {
        perror("Error opening the CSV file");
        return 1;
    }
   
    user_t *users = read_CSV_and_create_users(csv_file, 50);

    fclose(csv_file);
    
    int choice;
    int login_choice;
    _Bool logged_in = false;
    char username[USERNAME_LENGTH], password[PASSWORD_LENGTH],passcode[PASSWORD_LENGTH];

    printf("************************************\n");
    printf("Welcome to text based Facebook! \n");
    printf("************************************\n");

    do {
        if (logged_in == false){

        
            do{
                printf("Initial Menu:\n");
                printf("1. Register a user\n");
                printf("2. Login\n");
                printf("3. Exit\n");
                printf("Enter your choice?");
                scanf("%d", &login_choice);
                switch (login_choice) {
                    case 1:{
                    
                        printf("Enter username: ");
                        scanf("%s", username);
                        lower(username);
                        user_t* user = find_user(users, username);
                        if (user){
                            printf("Username is taken.\n");
                            break;
                        }
                        else{
                            printf("Enter password: ");
                            scanf("%s", password);
                            while(strlen(password)<8){
                                printf("Password too short, enter another one:\n");
                                scanf("%s", password);
                            }
                            users = add_user(users, username, password);
                            printf("'%s' was registered successfully.\n", username);
                            break;

                        }
                    }
                    case 2:{
                        printf("Enter username: ");
                        scanf("%s", username);
                        lower(username);
                        user_t* user = find_user(users, username);
                        if (user) {
                            printf("Enter the password for '%s'\n", username);
                            scanf("%s", passcode);
                            if (strcmp(passcode, user->password) == 0){
                                printf("You are signed in\n");
                                logged_in = true;
                            }
                            else{
                                printf("You entered the wrong passcode, retuning you back to menu! \n");
                            }
                    
                        } 
                        else {
                            printf("Username '%s'  was not found.\n", username);
                        }
                        break;


                    }
                    case 3:{
                        printf("Exiting the application.\n");
                        return 0;
                        break;
                    }
                    default:
                        printf("Invalid choice. Please select a valid option.\n");

                }


            }while(login_choice != 3 && logged_in == false);
        }
        




        
        print_menu(); 
        printf("Enter your choice to do: ");
        scanf("%d", &choice);
         

        switch (choice) {
         
            //Managing user profile
            case 1: {
                user_t *user = find_user(users, username);
                if (user) {
                    printf("Enter the password for '%s'\n", username);
                    scanf("%s", passcode);
                    if (strcmp(passcode, user->password) == 0){
                        printf("What would you like to switch the passcode to: ");
                        scanf("%s", user->password);
                        while(strlen(user->password)<8){
                            printf("Password too short, enter another one:\n");
                            scanf("%s", user->password);
                        }
                    }
                    else{
                        printf("You entered the wrong passcode, retuning you to main menu! \n");
                    }
                    
                } else {
                    printf("Username '%s'  was not found.\n", username);
                }
                break;
            }
            //Managing posts
            case 2: {
                user_t *user = find_user(users, username);
                if (user) {
                    int subchoice;
                    do{
                        printf("Post Menu\n");
                        if (user->posts == NULL){
                            printf("Note: No posts available for %s \n",user->username);
                        }
                        printf("1. Add a new post\n");
                        printf("2. Remove a post\n");
                        printf("3. Return to main menu\n");
                        printf("Enter your choice: ");
                        scanf("%d", &subchoice);

                        switch (subchoice) {
                            case 1:
                                char post[POST_LENGTH];
                                printf("Enter the post's content: ");
                                scanf(" %[^\n]s", post);
                                add_post(user, post);
                                printf("%s's posts:\n", user->username);
                                display_all_user_posts(user);
                                break;
                            case 2:
                                delete_post(user);
                                break;
                            case 3:
                                printf("Returning to the main menu!\n");
                                break;
                                    
                            
                            default:
                                printf("Invalid choice. Try again.\n");
                            }
                    } while(subchoice != 3);  
                } 
                else {
                    printf("User '%s' not found.\n", username);
                }
                break;
            }
            //Managing Friends
            case 3: {
                
                
                user_t *user = find_user(users, username);
                if (user) {
                    printf("Managing friends for  %s:\n", username);
                    int subchoice;
                    char friendname[USERNAME_LENGTH];
                    do{
                        printf("Friends Menu\n");
                        printf("1. Add a new friend\n");
                        printf("2. Remove friend\n");
                        printf("3. Return to main menu\n");
                        printf("Enter your choice: ");
                        scanf("%d", &subchoice);

                        switch (subchoice) {
                            case 1:
                                printf("Who would you add as a friend?");
                                scanf("%s", friendname);
                                lower(friendname);
                                if (find_user(users, friendname)){
                                    add_friend(user, friendname);
                                    friendlink_posts( find_friend(user->friends, friendname), users);
                                    printf("Friend was added.\n");

                                }
                                else{
                                    printf("The friend you are trying to add is noy a user, returning to menu!\n");
                                }
                               
                                break;
                            case 2:
                                printf("Who would you remove as a friend?");
                                scanf("%s", friendname);
                                friend_t *friend = find_friend(user->friends, friendname);
                                if(friend){
                                    delete_friend(user, friendname);
                                    printf("Updated list of %s's friends:\n", user->username);
                                    display_user_friends(user);
                                }
                                else{
                                    printf("Friend not found. \n");
                                }
                                break;
                            case 3:
                                printf("Returning to main menu!\n");
                                break;
                                    
                            
                            default:
                                printf("Invalid choice. Please try again.\n");
                            }
                    } while(subchoice != 3);  

                } else {
                    printf("User '%s' not found.\n", username);
                }
                break;
            }
            //Displaying  friend posts
            case 4: {
                user_t *user = find_user(users, username);
                char friendpost[USERNAME_LENGTH];
                printf("Enter the friend to display posts from: ");
                scanf("%s", friendpost);
                lower(friendpost);
                friend_t *friend = find_friend(user->friends, friendpost);
                if (friend) {
                    user_t *friend_display = find_user(users, friendpost);
                    display_posts_by_n(friend_display, POSTS_DISPLAYED);
                } else {
                    printf("Friend '%s' not found.\n", friendpost);
                }
                break;
            }
            case 5: {
                printf("Exiting the application.\n");
                break;
            }
            default:
                printf("Invalid choice. Please select a valid option.\n");
        }
    } while (choice != 5);

    
    teardown(users);

    return 0;

}
