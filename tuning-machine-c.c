#include<stdio.h>
#include<string.h>

#define TAPE_SIZE 100
#define STATE_HALTED -1

typedef struct {
	char tape[TAPE_SIZE];
	int head;
	int state;
}TuringMachine;
void initialise_machine(TuringMachine *tm,const char *input)
{
	memset(tm->tape,'_',TAPE_SIZE);
	strncpy(tm->tape,input,strlen(input));
	tm->head = 0;
	tm->state = 0;
}
void step(TuringMachine* tm) {
	char symbol = tm->tape[tm->head];
	switch (tm->state)
	{
	case 0:
		if (symbol == '1') {
			tm->tape[tm->head] = 'x';//'1'を'x'に置き換え
			tm->head++;
			tm->state = 1;
		}
		else {
			tm->state = STATE_HALTED;//終了
		}
		break; 
	case 1:
		if (symbol == '_') {
			tm->state = STATE_HALTED;//空白なら停止
		}
		else {
			tm->head++;//右へ移動
		}
		break;
	}
}
void run(TuringMachine* tm) {
	while (tm->state != STATE_HALTED) {
		step(tm);
	}
}
int main(void) {
	TuringMachine tm;
	initialise_machine(&tm, "111");
	printf("開始状態:%s\n", tm.tape);
	run(&tm);
	printf("終了状態:%s\n", tm.tape);
	return 0;
}
