#include<stdio.h>

typedef struct address {
char addr[6];
} address;

struct member {
struct address addr;
int a;
} member;

void main(){

struct member *node;
node=malloc(sizeof(member));
node->a=20;

char *msg;
msg="sama";
memcpy((char *)(msg), &node->addr, sizeof("sama"));

printf("%d",node->a);
for (i = 0; i < 6; i++))
printf("%c",node->addr[i]);

printf("%s", node->addr[0]);
free(node);
}
