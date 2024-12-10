#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <ctype.h>
#include <time.h>
#include "a2_nodes.h"
#include "a2_functions.h"


user_t *add_user(user_t *users, const char *username, const char *password) {
    
    user_t *added_user = (user_t *)malloc(sizeof(user_t));
    assert(added_user != NULL);
    strcpy(added_user->username, username);
    strcpy(added_user->password, password);
    added_user->friends = NULL;
    added_user->posts = NULL;
    added_user->next = NULL;

    
    if (users == NULL || strcmp(username, users->username) < 0) {
        added_user->next = users; 
        return added_user;
    }

    user_t *curr = users;
    while (curr->next != NULL && strcmp(curr->next->username, username) < 0) {
        curr = curr->next;
    }
    added_user->next = curr->next; 
    curr->next = added_user;

    return users;
}


user_t *find_user(user_t *users, const char *username) {
    while (users != NULL) {
        if (strcmp(users->username, username) == 0) {
            return users;
        }
        else{
            users = users->next;

        }
       
    }
    return NULL;
}

friend_t *create_friend(const char *username) {
    friend_t *created_friend = (friend_t *)malloc(sizeof(friend_t));
    assert(created_friend != NULL);


    strcpy(created_friend->username, username);
    created_friend->next = NULL;



    return created_friend;
}


void add_friend(user_t *user, const char *friend_name) {


    friend_t *added_friend = create_friend(friend_name);
    if (user->friends == NULL || strcmp(added_friend->username, user->friends->username) < 0) {
        added_friend->next = user->friends;
        user->friends = added_friend;
        return;
    }

    friend_t *curr = user->friends;
    while (curr->next != NULL && strcmp(curr->next->username, friend_name) < 0) {
        curr = curr->next;
    }
    added_friend->next = curr->next;
    curr->next = added_friend;
}

_Bool delete_friend(user_t *user, char *friend_name) {
    if (user == NULL || user->friends == NULL) return false;

    friend_t *curr = user->friends;
    friend_t *prev = NULL;

    while (curr != NULL) {
        if (strcmp(curr->username, friend_name) == 0) {
            if (prev == NULL) {
                user->friends = curr->next; 
            } 
            else {
                prev->next = curr->next; 
            }
            free(curr);
            return true;
        }
        prev = curr;
        curr = curr->next;
    }
    return false;
}

post_t *create_post(const char *text) {
    post_t *new_post = (post_t *)malloc(sizeof(post_t));
    assert(new_post != NULL);

    strcpy(new_post->content, text);
    new_post->next = NULL;

    return new_post;
}

void add_post(user_t *user, const char *text) {

    post_t *new_post = create_post(text);
    new_post->next = user->posts;
    user->posts = new_post;
    
}

_Bool delete_post(user_t *user) {
    if (user->posts == NULL){
        printf("No posts to delete");
        return false;
    } 

    post_t *temp = user->posts;
    user->posts = user->posts->next;

    printf("%s's posts:\n",user->username);
    display_all_user_posts(user);

    free(temp);
    return true;
}

void display_all_user_posts(user_t *user) {
    if (user == NULL || user->posts == NULL) {
        printf("No posts to be displayed.\n");
        return;
    }

    post_t *curr = user->posts;
    while (curr != NULL) {
        printf("%s\n", curr->content);
        curr = curr->next;
    }
}

void display_user_friends(user_t *user) {
    if (user == NULL || user->friends == NULL) {
        printf("No friends to be displayed.\n");
        return;
    }

    friend_t *curr = user->friends;
    while (curr != NULL) {
        printf("%s\n", curr->username);
        curr = curr->next;
    }
}



void display_posts_by_n(user_t *user, int number) {
    if (user == NULL || user->posts == NULL || number <= 0) {
        printf("No posts available.\n");
        return;
    }

    post_t *curr = user->posts;
    int count = 0;

    while (curr != NULL) {
        printf("Post %d: %s\n", count + 1, curr->content);
        count++;

        
        if (count % number == 0 && curr->next != NULL) {
            printf("Do you want to display more posts? (Y/N): ", count);
            char response;
            scanf(" %c", &response);

            
            if (response == 'n' || response == 'N') {
                break;
            }
        }

        curr = curr->next;
    }

    
    if (curr == NULL && count > 0) {
        printf("There are no more posts. %d posts was displayed in total.\n", count);
    } 
    else if (count == 0) {
        printf("No posts to display.\n");
    }
}

void teardown(user_t *users) {
    while (users != NULL) {
        user_t *free_user = users;

        
        while (free_user->friends != NULL) {
            friend_t *free_friend = free_user->friends;
            free_user->friends = free_user->friends->next;
            free(free_friend);
        }

        
        while (free_user->posts != NULL) {
            post_t *free_post = free_user->posts;
            free_user->posts = free_user->posts->next;
            free(free_post);
        }

        users = users->next;
        free(free_user);
    }
}

void print_menu() {
    printf("Main Menu:\n");
    printf("1. Manage profile\n");
    printf("2. Manage posts\n");
    printf("3. Manage friends\n");
    printf("4. Display a friends's posts\n");
    printf("5. Exit\n");
    
}

friend_t *find_friend(friend_t *friends, const char *friend) {
    while (friends != NULL) {
        if (strcmp(friends->username, friend) == 0) {
            return friends;
        }
        else{
            friends = friends->next;

        }
       
    }
    return NULL;
}

void lower(char *str){
    while(*str){
        *str = tolower(*str);
        str++;
    }

}

void friendlink_posts(friend_t *friend, user_t* users){
    user_t* user = find_user(users, friend->username); 
    friend->posts = &(user->posts);

}




/*
   ******** DONT MODIFY THIS FUNCTION ********
   Function that reads users from the text file.
   IMPORTANT: This function shouldn't be modified and used as is
   ******** DONT MODIFY THIS FUNCTION ********
*/
user_t *read_CSV_and_create_users(FILE *file, int num_users)
{
    srand(time(NULL));
    user_t *users = NULL;
    char buffer[500];
    fgets(buffer, sizeof(buffer), file); // Read and discard the header line
    int count = 0;
    for (int i = 0; i < num_users; i++)
    {
        fgets(buffer, sizeof(buffer), file);
        buffer[strcspn(buffer, "\r\n")] = 0; // Remove newline characters

        char *token = strtok(buffer, ",");
        char *token2 = strtok(NULL, ",");
        users = add_user(users, token, token2);
        char *username = token;

        token = strtok(NULL, ",");

        user_t *current_user = users;
        for (; current_user != NULL && strcmp(current_user->username, username) != 0; current_user = current_user->next)
            ;

        while (token != NULL && strcmp(token, ",") != 0 && count < 3)
        {
            if (strcmp(token, " ") != 0)
            {
                add_friend(current_user, token);
            }
            token = strtok(NULL, ",");
            count++;
        }
        count = 0;

        // token = strtok(NULL, ",");
        while (token != NULL && strcmp(token, ",") != 0)
        {
            add_post(current_user, token);
            token = strtok(NULL, ",");
        }
    }
    return users;
}
