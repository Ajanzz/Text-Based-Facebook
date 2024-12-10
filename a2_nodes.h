#ifndef A2_NODES_H
#define A2_NODES_H



typedef struct user
{
    char username[30];
    char password[15];
    struct friend *friends;
    struct post *posts;
    struct user *next;
} user_t;


typedef struct friend
{
    char username[30];
    struct friend *next;
    struct post **posts;
}
friend_t;


typedef struct post
{
    char content[250];
    struct post *next;
} post_t;

#endif
