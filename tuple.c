#include<stdio.h>

struct tuple {
	int val;
	int freq;
	int *t_size;
	struct tuple * before;
	struct tuple * after; 
};

struct tuple *create(int * size, int val, struct tuple * before, struct tuple * after){
	struct tuple *new = malloc(sizeof(struct tuple));
	*size++;
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
		if(next->val < 256)
			printf("(%d,%c),",next->freq,next->val);
		else{
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
	}
}


		

main(){
	char msg[] = "Hello";// how are you? How do you do?";
	int size = 0;
	int i;
	struct tuple *first = create(&size, msg[0], 0, 0);
	struct tuple *last = first;
	for(i = 1; i < sizeof(msg)-1; i++){
		struct tuple * check = check_t(msg[i], first);
		if(check != 0)
			check->freq ++;
		else{
			struct tuple *next = create(&size, msg[i], last, 0);
			last->after = next;
			last = next;
		}
	}
	print_t(first);
	putchar('\n');
	sort(first);
	print_t(first);
	putchar('\n');
}









