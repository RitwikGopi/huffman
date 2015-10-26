#include<stdio.h>
#include<string.h>

struct tuple {
	int val;
	int freq;
	int *t_size;
	struct tuple * before;
	struct tuple * after; 
};

struct list{
	char c, e[9];
};

struct tuple *create(int * size, int val, struct tuple * before, struct tuple * after){
	struct tuple *new = malloc(sizeof(struct tuple));
	(*size)++;
	new->freq = 1;
	new->t_size = size;
	new->val = val;
	new->before = before;
	new->after = after;
	return new;
}

struct tuple * check_t(int val, struct tuple * first){
	struct tuple * next = first;
	while(next != 0){
		if(next->val == val)
			break;
		next = next->after;}
	return next;
}

void print_t(struct tuple * first){
	struct tuple * next = first;
	printf("(");
	while( next != 0){
		//printf("%c:%d\n",next->val,next->val);
		//printf("%d:",next->freq);
		if(next->val < 256)
			printf("%c,",next->val);
		else{
			//printf("(%d,",next->freq);
			print_t(next->val);
			printf(",");
		}
		next = next->after;
	}
	printf("\b)");
}

void sort(struct tuple * first){
	int i,j;
	struct tuple *present = first;
	struct tuple * smallest = first; 
	while(present->after != 0){
		struct tuple * next = present->after;
		while(next != 0){
			if( smallest->freq > next->freq )
				smallest = next;
			else if ( smallest->freq == next->freq )
				if ( smallest->val > next->val)
					smallest = next;
			next = next->after;
		}
		int temp_val = present->val;
		int temp_freq = present->freq;
		present->val = smallest->val;
		present->freq = smallest->freq;
		smallest->val = temp_val;
		smallest->freq = temp_freq;
		present = present->after;
		smallest = present;
		//printf("SORTING.....\n");
		//print_t(first);
		//putchar('\n');
	}
}

struct tuple * read_msg(char * msg){
	int *size = malloc(sizeof(int));
	*size = 0;
	int i;
	struct tuple *first = create(size, *msg, 0, 0);
	struct tuple *last = first;
	for(i = 1; *(msg+i) != NULL; i++){
		struct tuple * check = check_t(*(msg+i), first);
		if(check != 0)
			check->freq ++;
		else{
			struct tuple *next = create(size, *(msg+i), last, 0);
			last->after = next;
			last = next;
		}
	}
	return first;
}

struct tuple * build_tree(struct tuple * msg){
	struct tuple * tree = msg;
	struct tuple * present = msg;
	struct tuple * p_tree;
	while( *tree->t_size > 2 ){
		struct tuple * next = present->after;
		//printf("%d:%c\n",next->val,next->val);
		*tree->t_size -= 2;
		p_tree = create(tree->t_size,present,present->before,next->after);
		//struct tuple * temp = p_tree->val;
		//printf("tree %d\n",temp->val);
		p_tree->freq = present->freq + next->freq;
		present->before = 0;
		next->after = 0;
		//printf("UNSORTED\n");
		//print_t(p_tree);
		//printf("\nSORTED\n");
		sort(p_tree);
		//print_t(p_tree);
		//putchar('\n');
		//printf("SUCCESS\n");
		present = p_tree;
		//printf("Next val %d:%c\n",present->val,present->val);	
		
	}
	return p_tree;
}

void create_list(char bin[],int * j,struct list * encoded,struct tuple *tree){
	//printf("HI\n");
	int i = 0, b = 0;
	for( i = 0; bin[i] != NULL; i++);
	struct tuple * branch = tree;
	while(branch != 0){
		bin[i] = '0' + b;
		bin[i+1] = NULL;
		if(branch->val < 256){
			(encoded+*j)->c = branch->val;
			bin[i+1] = NULL;
			strcpy((encoded+*j)->e, bin);
			//printf("%d\t%c:%s\n",*j,(encoded+*j)->c,(encoded+*j)->e);
			(*j)++;
		}
		else{
			create_list(bin,j,encoded,branch->val);
		}
		b = 1;
		branch = branch->after;
	}
}
			
void print_l(struct list *encoded,int size){
	int i=0;
	for(i=0;i<size;i++){
		printf("%c:%s\n",(encoded+i)->c,(encoded+i)->e);
	}
}


int encode(char * msg,struct list *encoded,int size,char *e_msg){
	int i=0,j,l=0;
	for(i = 0; *(msg+i) != NULL; i++){
		for(j=0;j<size;j++){
			if(*(msg+i) == (encoded+j)->c){
				int k = 0;
				while((encoded+j)->e[k]!=NULL){
					//printf("%d\n",k);
					*(e_msg + l++) = (encoded+j)->e[k++];
				}
				break;
			}
		}
	}
	return l;
}

main(){
	char message[] = "Perhaps not so easy to see is that there is no ambiguity in this encoding. Any string of zero's and one's will map to a unique string of the characters 'a' to 'g'. This is much more intuitive if we draw a tree for the code table.";
	printf("%d\n",sizeof(message));
	char * e_message = malloc(sizeof(message)*8);
	printf("%d\n",sizeof(e_message));
	struct tuple * msg = read_msg(&message);
//	print_t(msg);
//	putchar('\n');
	sort(msg);
	int size = *msg->t_size;
//	print_t(msg);
//	putchar('\n');
	struct tuple * tree = build_tree(msg);
	print_t(tree);
	putchar('\n');
	struct list * encoded = malloc(sizeof(struct list)*size);
	char bin[8];
	bin[0] = NULL;
	int j = 0;
	create_list(bin,&j,encoded,tree);
	print_l(encoded,size);
	int e_size = encode(&message,encoded,size,e_message);
	printf("%s\n%d,%d\n",e_message,sizeof(message),e_size/8);
}




